/*
 * Copyright (c) 1992-1999 by Symantec
 * Copyright (c) 1999-2011 by Digital Mars
 * All Rights Reserved
 * http://www.digitalmars.com
 * http://www.dsource.org/projects/dmd/browser/branches/dmd-1.x/src/iasm.c
 * http://www.dsource.org/projects/dmd/browser/trunk/src/iasm.c
 * Written by Mike Cote, John Micco, Walter Bright and Brad Roberts
 * D version by Walter Bright
 * ARM version by Brad Roberts
 *
 * This source file is made available for personal use
 * only. The license is in /dmd/src/dmd/backendlicense.txt
 * For any other uses, please contact Digital Mars.
 */

// ARM Inline assembler for the D programming language compiler

#include        <ctype.h>
#include        <math.h>
#include        <stdlib.h>
#include        <stdio.h>
#include        <string.h>
#include        <time.h>
#include        <assert.h>
#include        <setjmp.h>
#if __DMC__
#undef setjmp
#include        <limits.h>
#endif

// D compiler
#include        "mars.h"
#include        "lexer.h"
#include        "mtype.h"
#include        "statement.h"
#include        "id.h"
#include        "declaration.h"
#include        "scope.h"
#include        "init.h"
#include        "enum.h"
#include        "module.h"

// C/C++ compiler
#define SCOPE_H 1               // avoid conflicts with D's Scope
#include        "cc.h"
#include        "token.h"
#include        "global.h"
#include        "el.h"
#include        "type.h"
#include        "oper.h"
#include        "code.h"
#include        "arm_tables.h"
#include        "iasm_arm.h"

static char __file__[] = __FILE__;      /* for tassert.h                */
#include        "tassert.h"

// From expressionsem.c
Expression *expressionSemantic(Expression *e, Scope *sc);

// InlineAsmStatement:
//   "naked"            |
//   "align" Integer    |
//   OddInstruction     |
//   OpCode OperandList
//
// OddInstruction
//   See source code: CDP, CDP2
//
// OpCode:
//   OpCodeName[SetCC][ConditionCode][EncodingQualifier][ValueSize]
//
// SetCC:
//   <omitted> | "S"
//
// ConditionCode:
//   <omitted> |
//     "EQ" | "NE" | "CS" | "CC" | "MI" | "PL" | "VS" | "VC" |
//     "HI" | "LS" | "GE" | "LT" | "GT" | "LE" | "AL" |
//     "HS" | "LO"
//
// EncodingQualifier:
//   <omitted> | ".N" | ".W"
//
// ValueSize:
//   <omitted>  |
//   ".8"   | ".16"  | ".32"  |
//   ".F32" | ".F64" |
//   ".I8"  | ".I16" | ".I32" |
//   ".S8"  | ".S16" | ".S32" |
//   ".U8"  | ".U16" | ".U32
//
// OperandList:
//   Operand
//   Operand | OperandList
//
// Operand:
//   IndexExpression  |
//   RegisterList     |
//   RegisterName     |
//   RegisterName "!" |
//   ShiftExpression  |
//   "#" Integer
//
// IndexExpression:
//   "[" CoreRegister "]"                                // offset
//   "[" CoreRegister "]" "!"                            // pre-index, vrr syntax
//   "[" CoreRegister "]" "," OffsetExpression           // post-index
//   "[" CoreRegister "," OffsetExpression "]"           // offset
//   "[" CoreRegister "," OffsetExpression "]" "!"       // pre-index
//
// OffsetExpression:
//   IntegerOffset      |
//   CoreRegisterOffset |
//   CoreRegisterOffset "," ShiftExpression
//
// IntegerOffset:
//   "#" Integer
//   "#" "+" Integer
//   "#" "-" Integer
//
// CoreRegisterOffset:
//   CoreRegister
//   "+" CoreRegister
//   "-" CoreRegister
//
// RegisterList:
//   "{" Registers "}"
//
// Registers:
//   RegisterListName                  |
//   RegisterListName "," Registers    |
//   RegisterName "-" RegisterName |
//   RegisterName "-" RegisterName "," Registers
//
// RegisterListName:
//   RegisterName |
//   RegisterName "[" Integer "]"
//
// RegisterName:
//   CoreRegister |
//   VRPRegister  |
//   VRRRegister "[" Integer "]"
//
// CoreRegister:
//   R0 .. R15 | LP | PC | SP | FP
//
// VRPRegister:
//   S0 .. S31 |
//   D0 .. D15 |
//   Q0 .. Q7
//
// ShiftExpression:
//   LSL ShiftAmount |
//   LSR ShiftAmount |
//   ASR ShiftAmount |
//   ROR ShiftAmount |
//   RRX
//
// ShiftAmount:
//   CoreRegister |
//   "#" Integer

// Error numbers

enum ASMERRMSGS
{
    EM_bad_register,
    EM_num,
    EM_opcode_exp,
    EM_undefined_identifier,
    EM_unexpected_token,
    EM_wrong_reg_type,
};

const char *asmerrmsgs[] =
{
    "expected a register, not: %s",
    "integer expected",
    "opcode expected, not: %s",
    "undefined identifier: %s",
    "unexpected token: %s",
    "wrong register type, expected: %s",
};

// Additional tokens for the inline assembler
enum ASMTK
{
    ASMTKnaked = TOKMAX + 1,
    ASMTKptr,
    ASMTKmax = ASMTKptr-(TOKMAX+1)+1
};

static const char *apszAsmtk[ASMTKmax] =
{
    "naked",
    "ptr",
};

enum ASM_JUMPTYPE
{
    ASM_JUMPTYPE_UNSPECIFIED,
    ASM_JUMPTYPE_SHORT,
};

struct ASM_STATE
{
    uint8_t ucItype;  // shared with struct OP.type
        #define ITjump     1
        #define ITdataproc 2
        #define IToddinstr 3
    uint8_t ucIcc;    // instruction condition code
    uint8_t ucIccIdx; // index of cc (only used to get original string for debugging)
    uint8_t sizeIdx;  // index into the sizes table
    uint16_t flags;
        #define Fsetcc          (1 << 0)
        #define FhasSizePrefix  (1 << 1)
        #define Fsubtract       (1 << 2)

    Loc loc;
    unsigned char bInit;
    LabelDsymbol *psDollar;
    Dsymbol *psLocalsize;
    jmp_buf env;
    unsigned char bReturnax;
    InlineAsmStatement *statement;
    Scope *sc;
};

ASM_STATE asmstate;

static Token *asmtok;
static enum TOK tok_value;

static const char* regstrs[] =
{
#define X(a, b, c) a,
    REGISTERS
#undef X
};

struct REG
{
    const char* regstr;
    uint8_t val;
    uint8_t type;
      #define REGTcore   0x01
      #define REGTsingle 0x02
      #define REGTdouble 0x04
      #define REGTquad   0x08
};

static REG regs[] =
{
#define X(a, b, c) a, b, c,
    REGISTERS
#undef X
};

static const char* condcodestrs[] =
{
#define X(a, b) #a,
    CONDCODES
#undef X
};
static int condcodes[] =
{
#define X(a, b) b,
    CONDCODES
#undef X
};

static const char* sizesuffixstrs[] =
{
#define X(a) a,
    SIZESUFFIXES
#undef X
};

static const char* shiftcodestrs[] =
{
#define X(a) #a,
    SHIFTCODES
#undef X
};

struct OP;
struct OPND;

static code *asm_emit_unimplemented(Loc loc, OP *op, size_t numargs, OPND* o[]) { /*assert(false);*/ return NULL; }
static code *asm_emit_unsupported(Loc loc, OP *op, size_t numargs, OPND* o[]);

static code *asm_emit_dataproc(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_dataproc2(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_mult_accum(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_sat_add_sub(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_half_mult_accum(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_extra_ldst(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_sync(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_hints(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_MSR(Loc loc, OP* op, size_t numargs, OPND* opnds[]);
static code *asm_emit_MRS(Loc loc, OP* op, size_t numargs, OPND* opnds[]);
static code *asm_emit_misc(Loc loc, OP* op, size_t numargs, OPND* opnds[]);
static code *asm_emit_wordbyte_ldst(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_media_misc(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_p_addsub(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_packing(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_smult(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_blockdata(Loc loc, OP *op, size_t numargs, OPND* o[]);

static code *asm_emit_branch(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_cdp(Loc loc, OP *op, size_t numargs, OPND* o[]);
static code *asm_emit_odd(Loc loc, OP *op, size_t numargs, OPND* o[]);

static const char* opcodestrs[] =
{
#define X(a, b, c, d, e, f) #a,
    OPCODES
#undef X
};

typedef code* (*AsmEmitter)(Loc loc, OP *op, size_t numargs, OPND* o[]);

struct OP
{
    unsigned   Iop;
    const char* name;
    unsigned   minargs;
    unsigned   maxargs;
    unsigned   type;
    AsmEmitter fp;
};

static OP opcodes[] =
{
#define X(a, b, c, d, e, f) {b, #a, c, d, e, f},
    OPCODES
#undef X
};

struct OPND
{
    uint8_t      type;
      #define OPNDTreg      0x01
      #define OPNDTregwrite 0x02
      #define OPNDTregidx   0x03
      #define OPNDTreglist  0x04
      #define OPNDTregdlist 0x05
      #define OPNDTshift    0x06
      #define OPNDTliteral  0x07
      #define OPNDToffset   0x08
      #define OPNDTpreidx   0x09
      #define OPNDTpostidx  0x0A

    REG*         base;              // register
    REG*         regDisp1;          // index reg1
    targ_llong   disp;              // literal or displacement
    Dsymbol*     s;                 // symbol

    uint8_t      shiftcode;         // only used with type OPNDTshift
    OPND*        next;              // used for shiftcode and list operands

#if 0
    REG*         regDisp2;          // index reg2
    Type*        ptype;             // if bPtr, then ptype points to the type object
    ASM_JUMPTYPE ajt;               // type of jump: should this become a bool for ARM?
    bool         bOffset;           // if 'offset' keyword
    bool         bPtr;              // if 'ptr' keyword
#endif
};

// Exported functions called from the compiler

int asm_state(int iFlags);
void iasm_term();

//
// Local functions defined and only used here
//
static OPND *opnd_calloc();
static void opnd_free(OPND *popnd);
static code *asm_genloc(Loc loc, code *c);
static int asm_getnum();

static void asmerr(const char *, ...);

#if __clang__
static void asmerr(int, ...) __attribute__((analyzer_noreturn));
#else
static void asmerr(int, ...);
#if __DMC__
#pragma SC noreturn(asmerr)
#endif
#endif

static void asm_token();
static void asm_token_trans(Token *tok);
static void asm_chktok(enum TOK toknum);

static void asm_output_op(OP *op);
static void asm_output_popnd(OPND *popnd);

static REG *asm_reg_lookup(const char *s, ssize_t len = -1);
static OPND* asm_parse_reg(bool required);

/********************************/

static OPND *opnd_calloc()
{
    OPND *o = (OPND*)calloc(1, sizeof(OPND));
    return o;
}

/********************************/

static void opnd_free(OPND *o)
{
    if (o)
    {
        opnd_free(o->next);
        free(o);
    }
}

/********************************/

static OPND* opnd_dup(OPND *o)
{
    OPND* onew = opnd_calloc();

    onew->type      = o->type;
    onew->base      = o->base;
    onew->regDisp1  = o->regDisp1;
    onew->disp      = o->disp;
    onew->s         = o->s;
    onew->shiftcode = o->shiftcode;
    onew->next      = o->next ? opnd_dup(o->next) : NULL;

    return onew;
}

/********************************/

static void asm_chktok(enum TOK toknum)
{
    if (tok_value == toknum)
        asm_token();                    // scan past token
    else
    {
        /* When we run out of tokens, asmtok is NULL.
         * But when this happens when a ';' was hit.
         */
        asmerr("expected '%s', not: '%s'", Token::toChars(toknum), asmtok ? asmtok->toChars() : ";");
    }
}

static void dupFirstOpnd(size_t& numargs, OPND* o[])
{
    for (size_t i = numargs; i > 1; i--)
    {
        o[i] = o[i-1];
    }
    o[1] = opnd_dup(o[0]);
    ++numargs;
}

static void symbol_to_fl(Dsymbol *s, size_t disp, code* c)
{
    LabelDsymbol *label = s->isLabel();
    Declaration  *d = s->isDeclaration();
    if (label)
    {
        if (s == asmstate.psDollar)
        {
            c->IFL1 = FLconst;
            c->IEVsize_t1 = disp;
        }
        else
        {
            c->IFL1 = FLblockoff;
            c->IEVlsym1 = label;
        }
    }
    else if (s == asmstate.psLocalsize)
    {
        c->IFL1 = FLlocalsize;
        c->IEVdsym1 = NULL;
        c->Iflags |= CFoff;
        c->IEVoffset1 = disp;
    }
    else if (s->isFuncDeclaration())
    {
        c->IFL1 = FLfunc;
        c->Iflags |= CFoff;
        c->IEVdsym1 = d;
        c->IEVoffset1 = disp;
    }
    else
    {
#ifdef DEBUG
        if (debuga)
            printf("Setting up symbol %s\n", d->ident->toChars());
#endif
        c->IFL1 = FLdsymbol;
        c->IEVdsym1 = d;
        c->Iflags |= CFoff;
        c->IEVoffset1 = disp;
    }
}

static void _encode_shift(code *c, OPND* o)
{
    assert(o->type == OPNDTshift);

    switch (o->shiftcode)
    {
        case 0: c->setShiftType(2); break;
        case 1: c->setShiftType(0); break;
        case 2: c->setShiftType(1); break;
        case 3: c->setShiftType(3); break;
        case 4: c->setShiftType(3); break;
        default: assert(0);
    }

    if (o->base)
    {
        if (o->base->type != REGTcore) asmerr("shift register must be a core register");

        c->setShiftImm(1);
        c->setShiftReg(o->base->val);
    }
    else if (o->disp != 0)
    {
        // TODO: range check disp
        c->setShiftImm(0);
        c->setShiftOffset(o->disp);
    }
}

static code *asm_emit_dataproc(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    assert(numargs >= 2 && numargs <= 4);

    if (!o[0]->base)
    {
        asmerr("operand 1 of MOV must be a register");
        return NULL;
    }

    if (op->Iop == OP_NEG)
    {
        assert(numargs == 2);
        // the equivalent of RSBS Rd, Rm, #0
        int opidx = binary("RSB", 3, opcodestrs, sizeof(opcodestrs) / sizeof(opcodestrs[0]));
        if (opidx < 0) assert(0);
        op = &opcodes[opidx];

        asmstate.flags |= Fsetcc;
        o[2] = opnd_calloc();
        o[2]->type = OPNDTliteral;
        o[2]->disp = 0;
        numargs++;
    }
    else if (op->Iop == OP_ADR)
    {
        // convert from ADR Rn, <label> to ADR Rn, PC, <label>
        assert(numargs == 2);

        o[2] = o[1];
        o[1] = opnd_calloc();
        o[1]->base = asm_reg_lookup("PC");
        numargs++;
    }

    // expand optional Rd to explicit Rd
    // this results in less conditions:
    //     OP Rd, Rn, #const
    //     OP Rd, Rn, Rm
    //     OP Rd, Rn, Rm, <shift>
    if (numargs == 2 || (numargs == 3 && o[2]->type == OPNDTshift))
        dupFirstOpnd(numargs, o);

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (asmstate.flags & Fsetcc ||
        op->Iop == OP_CMN       ||
        op->Iop == OP_CMP       ||
        op->Iop == OP_TEQ       ||
        op->Iop == OP_TST)
        c->Iflags |= CFpsw;

    c->setRegD(o[0]->base->val);
    c->setRegN(o[1]->base->val);

    if (o[2]->base)
    {
        c->setRegM(o[2]->base->val);

        if (numargs == 4)
            _encode_shift(c, o[3]);
    }
    else if (o[2]->s)
    {
        c->Iflags |= CFimmed;
        symbol_to_fl(o[2]->s, o[2]->disp, c);
    }
    else
    {
        c->Iflags |= CFimmed;
        c->IFL1 = FLconst;
        c->IEVsize_t1 = o[2]->disp;
    }

    if (c->Iop == OP_NEG || c->Iop == OP_ADR)
        opnd_free(o[2]);

    return c;
}

// rewrite to MOV form of instruction
static code *asm_emit_dataproc2(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    assert(numargs >= 1 && numargs <= 3);

    unsigned origIop = op->Iop;

    int opidx = binary("MOV", 3, opcodestrs, sizeof(opcodestrs) / sizeof(opcodestrs[0]));
    if (opidx < 0) assert(0);
    op = &opcodes[opidx];

    // expand optional Rd to explicit Rd
    // this results in less conditions:
    //     OP Rd, Rn, #const
    //     OP Rd, Rn, Rm
    if (numargs == 1 || (numargs == 2 && origIop != OP_RRX))
        dupFirstOpnd(numargs, o);

    switch (origIop)
    {
        case OP_ASR:
            if (numargs != 3) error(loc, "ASR requires just 3 operands");
            o[2]->shiftcode = 0;
            break;

        case OP_LSL:
            if (numargs != 3) error(loc, "LSL requires just 3 operands");
            o[2]->shiftcode = 1;
            break;

        case OP_LSR:
            if (numargs != 3) error(loc, "LSR requires just 3 operands");
            o[2]->shiftcode = 2;
            break;

        case OP_ROR:
            if (numargs != 3) error(loc, "ROR requires just 3 operands");
            o[2]->shiftcode = 3;
            break;

        case OP_RRX:
            if (numargs != 2) error(loc, "RRX requires just 2 operands, both core registers");
            ++numargs;
            o[2] = opnd_calloc();
            o[2]->shiftcode = 4;
            break;

        default:
            assert(0);
    }

    o[2]->type = OPNDTshift;

    code* c = asm_emit_dataproc(loc, op, numargs, o);

    if (origIop == OP_RRX)
    {
        opnd_free(o[2]);
        o[2] = NULL;
    }

    return c;
}

static code *asm_emit_mult_accum(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    assert(numargs >= 2 && numargs <= 4);

    if ((op->Iop == OP_MLS || op->Iop == OP_UMAAL) && (asmstate.flags & Fsetcc))
        asmerr("S flag not supported");

    for (size_t i = 0; i < numargs; ++i)
    {
        if (!o[i]->base || o[i]->base->type != REGTcore)
            asmerr("expected a core register for opcode #%zd", i);
    }

    // expand optional Rd to explicit Rd
    // this results in less conditions:
    //     OP Rd, Rn, Rm
    if (numargs == 2)
        dupFirstOpnd(numargs, o);

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (asmstate.flags & Fsetcc) c->Iflags |= CFpsw;

    if (c->Iop == OP_SMLAL ||
        c->Iop == OP_SMULL ||
        c->Iop == OP_UMAAL ||
        c->Iop == OP_UMLAL ||
        c->Iop == OP_UMULL)
    {
        c->setRegD(o[1]->base->val);
        c->setRegS(o[0]->base->val);
        c->setRegM(o[3]->base->val);
        c->setRegN(o[2]->base->val);
    }
    else
    {
        c->setRegD(o[0]->base->val);
        c->setRegN(o[1]->base->val);
        c->setRegM(o[2]->base->val);

        if (numargs == 4)
            c->setRegS(o[3]->base->val);
    }

    return c;
}

static code *asm_emit_sat_add_sub(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    assert(numargs == 2 || numargs == 3);

    if (asmstate.flags & Fsetcc)
        asmerr("S flag not supported");

    // expand optional Rd to explicit Rd
    // this results in less conditions:
    //     OP Rd, Rn, Rm
    if (numargs == 2)
        dupFirstOpnd(numargs, o);

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    c->setRegD(o[0]->base->val);
    c->setRegM(o[1]->base->val);
    c->setRegN(o[2]->base->val);

    return c;
}

static code *asm_emit_half_mult_accum(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    assert(numargs >= 2 && numargs <= 4);

    if (asmstate.flags & Fsetcc)
        asmerr("S flag not supported");

    // expand optional Rd to explicit Rd
    // this results in less conditions:
    //     OP Rd, Rn, Rm
    if (numargs == 2)
        dupFirstOpnd(numargs, o);

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (c->Iop == OP_SMLALBB || c->Iop == OP_SMLALBT || c->Iop == OP_SMLALTB || c->Iop == OP_SMLALTT)
    {
        c->setRegD(o[1]->base->val);
        c->setRegS(o[0]->base->val);
        c->setRegM(o[3]->base->val);
        c->setRegN(o[2]->base->val);
    }
    else
    {
        c->setRegD(o[0]->base->val);
        c->setRegN(o[1]->base->val);
        c->setRegM(o[2]->base->val);
        if (numargs == 4)
            c->setRegS(o[3]->base->val);
    }

    return c;
}

static code *asm_emit_extra_ldst(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (c->Iop == OP_LDRD || c->Iop == OP_STRD)
    {
        assert(numargs == 3);
        if (!(o[0]->base && o[0]->base->type == REGTcore &&
              o[1]->base && o[1]->base->type == REGTcore &&
              (o[0]->base->val + 1) == o[1]->base->val   &&
              (o[0]->base->val & 1) == 0))
            asmerr("Rt and Rt2 must be core registers, Rt must be even, and Rt2 must be Rt+1");
    }
    else
    {
        assert(numargs == 2);
        if (!(o[0]->base && o[0]->base->type == REGTcore))
            asmerr("Rt must be a core register");
    }

    size_t r = numargs-1;

    if (c->Iop == OP_LDRHT || c->Iop == OP_STRHT || c->Iop == OP_LDRSBT || c->Iop == OP_LDRSHT)
    {
        assert(numargs == 2);

        if (o[r]->s)
            asmerr("no literals allowed");

        if (o[r]->type != OPNDTpostidx && o[r]->type != OPNDToffset)
            asmerr("only offset and postidx addressing allowed");

        if (o[r]->type == OPNDToffset && o[r]->disp != 0)
            asmerr("#imm offset not allowed");
    }

    switch (o[r]->type)
    {
        case OPNDTliteral: break;
        case OPNDToffset:  c->setAddrMode(IAMMode_off);     break;
        case OPNDTpreidx:  c->setAddrMode(IAMMode_preidx);  break;
        case OPNDTpostidx: c->setAddrMode(IAMMode_postidx); break;
        default:
            assert(0);
    }

    c->setRegD(o[0]->base->val);

    if (o[r]->regDisp1)
    {
        c->setRegN(o[r]->base->val);
        c->setRegM(o[r]->regDisp1->val);
        c->setShiftSubtract(asmstate.flags & Fsubtract);
    }
    else if (o[r]->s)
    {
        if (c->Iop == OP_STRH)
            asmerr("STRH doesn't support literals");

        c->setRegN(0xf);
        symbol_to_fl(o[r]->s, o[r]->disp, c);
    }
    else
    {
        c->setRegN(o[r]->base->val);
        c->IFL1 = FLconst;
        c->IEVsize_t1 = o[r]->disp;
    }
    
    return c;
}

static code *asm_emit_sync(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    int r = numargs-1;

    if (o[r]->type == OPNDToffset)
    {
        if (o[r]->disp != 0)
            asmerr("#imm offset not allowed");
    }
    else
        asmerr("only offset addressing allowed");

    if (c->Iop == OP_STREXD)
    {
        assert(numargs == 4);
        if (((o[1]->base->val + 1) != o[2]->base->val) && ((o[1]->base->val & 1) != 0))
            asmerr("Rt1 must be even and Rt2 must be Rt1+1");
    }

    c->setRegD(o[0]->base->val);
    c->setRegN(o[r]->base->val);
    if (numargs == 2 || c->Iop == OP_LDREXD)
    {
        c->setRegS(0xf);
        c->setRegM(0xf);
    }
    else
    {
        if (!(c->Iop == OP_SWP || c->Iop == OP_SWPB))
            c->setRegS(0xf);
        //regS == 0
        c->setRegM(o[1]->base->val);
    }

    return c;
}

static code *asm_emit_hints(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    assert(numargs == 0 || (op->Iop == OP_DBG && numargs == 1));

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (c->Iop == OP_DBG)
    {
        if (o[0]->s || o[0]->base || o[0]->disp < 0 || o[0]->disp > 15)
            asmerr("#imm in the range 0..15 expected");

        c->setRegN(o[0]->disp);
    }

    return c;
}

static uint32_t _parse_special_register(const char * id, bool isMSR)
{
    static const char * prefixes[] = { "APSR", "CPSR", "ELR", "SPSR" };
    static const char * apsrsuffixes[] = { "g", "nzcvq", "nzcvqg" };
    static const char * modes[] = { "abt", "fiq", "hyp", "irq", "mon", "svc", "und", "usr" };

    const char* us = strchr(id, '_');
    if (us == NULL)
    {
        if (isMSR)
            asmerr("special register must be of the form: <prefix>_<suffix>");
        else
            us = id+strlen(id); // point to the null
    }

    int prefixidx = binary(id, us-id, prefixes, sizeof(prefixes)/sizeof(prefixes[0]));
    int regidx = -1;
    if (prefixidx == -1)
    {
        REG* reg = asm_reg_lookup(id, us-id);
        if (reg && reg->type == REGTcore && reg->val > 7 && reg->val < 15)
        {
            prefixidx = 4;
            regidx = reg->val;
        }
        else
            asmerr("special register prefix (%.*s) must be one of: APSR, CPSR, ELR, SPSR, or R8-R14", us-id, id);
    }

    int32_t mask = 0;
    switch (prefixidx)
    {
        case 0: // APSR
        {
            if (isMSR)
            {
                int suffixidx = binary(us+1, apsrsuffixes, sizeof(apsrsuffixes)/sizeof(apsrsuffixes[0]));
                if (suffixidx == -1) asmerr("special register suffix (%s) must be one of: g, nzcvq, or nzcvqg", us+1);

                switch (suffixidx)
                {
                    case 0: mask = 0x4; break;
                    case 1: mask = 0x8; break;
                    case 2: mask = 0xc; break;
                    default: assert(0);
                }
            }
            else
            {
                if (*us != '\0') asmerr("MRS should have no suffix for APSR");
                mask = 0xf;
            }

            break;
        }

        case 1: // CPSR
LtrySPSRagain:
        {
            if (isMSR)
            {
                for (const char * p = us+1; *p != '\0'; ++p)
                {
                    switch(*p)
                    {
                        case 'c': mask |= 1; break;
                        case 'x': mask |= 2; break;
                        case 's': mask |= 4; break;
                        case 'f': mask |= 8; break;
                        default:
                          if (prefixidx == 1)
                              asmerr("special register suffix (%s) must be one of: c, f, s, or x", us+1);
                          else
                              asmerr("special register suffix (%s) must be one of: c, f, s, x, abt, fiq, hyp, irq, mon, svc, und, usr");
                    }
                }
            }
            else
            {
                if (*us != '\0') asmerr("MRS should have no suffix for CPSR or SPSR");
                mask = 0xf;
            }
            if (prefixidx == 3) mask |= 0x40;
            break;
        }

        case 2: // ELR
        case 3: // SPSR
        case 4: // register
        {
            if (!isMSR && prefixidx == 3 && *us == '\0') goto LtrySPSRagain;

            int suffixidx = binary(us+1, modes, sizeof(modes)/sizeof(modes[0]));
            if (suffixidx == -1)
            {
                if (prefixidx == 3)
                    goto LtrySPSRagain;
                asmerr("special register suffix must be one of: abt, fiq, hyp, irq, mon, svc, und, usr");
            }
            switch (prefixidx)
            {
                case 2:
                    if (suffixidx != 2) asmerr("ELR only allows hyp as a valid suffix");
                    mask = (1 << 5) | (3 << 3) | 6;
                    break;
                case 3:
                    switch (suffixidx)
                    {
                        case 0: mask = 0x14; break; // abt
                        case 1: mask = 0x0e; break; // fiq
                        case 2: mask = 0x1e; break; // hyp
                        case 3: mask = 0x10; break; // irq
                        case 4: mask = 0x1c; break; // mon
                        case 5: mask = 0x12; break; // svc
                        case 6: mask = 0x16; break; // und
                        case 7: asmerr("SPSR doesn't allow usr as a suffix"); break; // usr
                        default: assert(0);
                    }
                    mask |= (3 << 5); // set R bit
                    break;
                case 4:
                    if (suffixidx == 2)
                    {
                        if (regidx != 13)
                            asmerr("hyp suffix only allowed with R13/SP or SPSR prefixes");
                    }
                    else if (suffixidx != 1 && suffixidx != 7)
                    {
                        if (regidx != 13 && regidx != 14)
                            asmerr("%s suffix only allows R13/R14/SP/LR or SPSR prefixes", us+1);
                    }

                    switch (suffixidx)
                    {
                        case 0: mask = (1 << 4) | (regidx == 13 ? 0x5 : 0x4); break; // abt
                        case 1: mask = (1 << 3) | (regidx-8); break; // fiq
                        case 2: mask = 0x1f; break; // hyp
                        case 3: mask = (1 << 4) | (regidx == 13 ? 0x1 : 0x0); break; // irq
                        case 4: mask = (1 << 4) | (regidx == 13 ? 0xd : 0xc); break; // mon
                        case 5: mask = (1 << 4) | (regidx == 13 ? 0x3 : 0x2); break; // svc
                        case 6: mask = (1 << 4) | (regidx == 13 ? 0x7 : 0x6); break; // und
                        case 7: mask = regidx-8; break; // usr
                        default: assert(0);
                    }
                    mask |= (1 << 5);
                    break;
                default: assert(0);
            }
            break;
        }

        default: assert(0);
    }

    return mask;
}

static code *asm_emit_MSR(Loc loc, OP* op, size_t numargs, OPND* opnds[])
{
    if (tok_value != TOKidentifier) asmerr ("invalid syntax for MSR");

    uint32_t mask = _parse_special_register(asmtok->ident->toChars(), true);

    asm_token();

    asm_chktok(TOKcomma);

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);
    c->setRegS(mask & 0xf);         // not really a register, but need a way to plumb the bits through
    c->setRegM((mask & 0xf0) >> 4); // not really a register, but need a way to plumb the bits through

    if (tok_value == TOKidentifier)
    {
        OPND* o = asm_parse_reg(true);
        if (!o || o->type != OPNDTreg) asmerr("only bare register allowed in MSR operands");
        c->setRegN(o->base->val);
        opnd_free(o);
    }
    else
    {
        asm_chktok(TOKpound);

        int value = asm_getnum();
        c->IFL1 = FLconst;
        c->IEVsize_t1 = value;
    }

    return c;
}

static code *asm_emit_MRS(Loc loc, OP* op, size_t numargs, OPND* opnds[])
{
    OPND* o = asm_parse_reg(true);
    if (!o || o->type != OPNDTreg) asmerr("only bare register allowed in MRS operands");

    asm_chktok(TOKcomma);

    if (tok_value != TOKidentifier) asmerr ("invalid syntax for MSR");

    uint32_t mask = _parse_special_register(asmtok->ident->toChars(), false);
    asm_token();

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    c->setRegD(o->base->val);
    c->setRegS(mask & 0xf);         // not really a register, but need a way to plumb the bits through
    c->setRegM((mask & 0xf0) >> 4); // not really a register, but need a way to plumb the bits through

    opnd_free(o);
    return c;
}

static code *asm_emit_misc(Loc loc, OP* op, size_t numargs, OPND* opnds[])
{
    if ((op->Iop == OP_BKPT || op->Iop == OP_HVC) && asmstate.ucIcc != COND_AL)
        asmerr("%s must be unconditional", op->name);

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (numargs == 1)
    {
        if (c->Iop == OP_BKPT || c->Iop == OP_HVC || c->Iop == OP_SMC)
        {
            if (opnds[0]->base || opnds[0]->regDisp1)
                asmerr("%s expects #imm", op->name);

            c->IFL1 = FLconst;
            c->IEVsize_t1 = opnds[0]->disp;
        }
        else
        {
            if (!opnds[0]->base || opnds[0]->base->type != REGTcore)
                asmerr("%s expects Rn", op->name);

            c->setRegM(opnds[0]->base->val);
        }
    }
    else if (numargs == 2)
    {
        if (!opnds[0]->base || opnds[0]->base->type != REGTcore ||
            !opnds[1]->base || opnds[1]->base->type != REGTcore)
            asmerr("%s expects Rn, Rn", op->name);

        c->setRegD(opnds[0]->base->val);
        c->setRegM(opnds[1]->base->val);
    }

    return c;
}

static code *asm_emit_wordbyte_ldst(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    assert(numargs == 2);

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    switch (o[1]->type)
    {
        case OPNDTliteral: break;
        case OPNDToffset:  c->setAddrMode(IAMMode_off);     break;
        case OPNDTpreidx:  c->setAddrMode(IAMMode_preidx);  break;
        case OPNDTpostidx: c->setAddrMode(IAMMode_postidx); break;
        default:
            assert(0);
    }

    c->setRegD(o[0]->base->val);

    if (o[1]->regDisp1)
    {
        c->setRegN(o[1]->base->val);
        c->setRegM(o[1]->regDisp1->val);
        c->setShiftSubtract(asmstate.flags & Fsubtract);

        if (o[1]->next)
            _encode_shift(c, o[1]->next);
    }
    else if (o[1]->s)
    {
        if (c->Iop == OP_STR)
            asmerr("STR doesn't support literals");

        c->setRegN(0xf);
        symbol_to_fl(o[1]->s, o[1]->disp, c);
    }
    else
    {
        c->setRegN(o[1]->base->val);
        c->setShiftImm(1);
        c->IFL1 = FLconst;
        c->IEVsize_t1 = o[1]->disp;
    }

    return c;
}

static code *asm_emit_media_misc(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (numargs == 2 && c->Iop == OP_USAD8)
        dupFirstOpnd(numargs, o);

    switch (c->Iop)
    {
        case OP_BFC:
            c->setRegD(o[0]->base->val);
            c->setRegN(0xf);
            c->IFL1 = FLconst;
            c->IEVsize_t1 = (((o[2]->disp) & 0x1f) << 5) | (o[1]->disp & 0x1f);
            break;
        case OP_BFI:
            c->setRegD(o[0]->base->val);
            c->setRegN(o[1]->base->val);
            c->IFL1 = FLconst;
            c->IEVsize_t1 = (((o[3]->disp + o[2]->disp - 1) & 0x1f) << 5) | (o[2]->disp & 0x1f);
            break;
        case OP_SBFX:
        case OP_UBFX:
            c->setRegD(o[0]->base->val);
            c->setRegN(o[1]->base->val);
            c->IFL1 = FLconst;
            c->IEVsize_t1 = (((o[3]->disp - 1) & 0x1f) << 5) | (o[2]->disp & 0x1f);
            break;
        case OP_UDF:
            if (o[0]->type != OPNDTliteral || o[0]->s) asmerr("UDF takes one #imm parameter");
            c->IFL1 = FLconst;
            c->IEVsize_t1 = o[0]->disp;
            break;
        case OP_USADA8:
            c->setRegD(o[0]->base->val);
            c->setRegN(o[1]->base->val);
            c->setRegM(o[2]->base->val);
            c->setRegS(o[3]->base->val);
            break;
        case OP_USAD8:
            c->setRegD(o[0]->base->val);
            c->setRegN(o[1]->base->val);
            c->setRegM(o[2]->base->val);
            c->setRegS(0xf);
            break;
        default: assert(0);
    }

    return c;
}

static code *asm_emit_p_addsub(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (numargs == 2)
        dupFirstOpnd(numargs, o);

    c->setRegD(o[0]->base->val);
    c->setRegN(o[1]->base->val);
    c->setRegM(o[2]->base->val);

    return c;
}

static code *asm_emit_packing(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    // num == number of regs after optional default arg duplication
    // expectedshift == -1 (none), 0..4
    int num = 0, expectedshift = 0;
    switch (c->Iop)
    {
        case OP_PKHBT:
            num = 3; expectedshift =  1; break;
        case OP_PKHTB:
            num = 3; expectedshift =  0; break;
        case OP_RBIT: case OP_REV: case OP_REV16: case OP_REVSH:
            num = 2; expectedshift = -1; break;
        case OP_SEL:
            num = 3; expectedshift = -1; break;
        case OP_SSAT: case OP_USAT:
            num = 3; expectedshift =  0; break;
        case OP_SSAT16: case OP_USAT16:
            num = 3; expectedshift = -1; break;
        case OP_SXTAB: case OP_SXTAB16: case OP_SXTAH: case OP_UXTAB: case OP_UXTAB16: case OP_UXTAH:
            num = 3; expectedshift =  3; break;
        case OP_SXTB: case OP_SXTB16: case OP_SXTH: case OP_UXTB: case OP_UXTB16: case OP_UXTH:
            num = 2; expectedshift =  3; break;
        default: assert(0);
    }

    if (numargs == (num-1) || (numargs == (num) && o[num-1]->type == OPNDTshift))
        dupFirstOpnd(numargs, o);

    if (c->Iop == OP_SSAT || c->Iop == OP_SSAT16 || c->Iop == OP_USAT || c->Iop == OP_USAT16)
    {
        c->setRegD(o[0]->base->val);
        c->setRegM(o[2]->base->val); // docs label this Rn, but the rest of the instructions label this block as Rm.
        c->IFL1 = FLconst;
        if (c->Iop == OP_SSAT || c->Iop == OP_SSAT16)
            c->IEVsize_t1 = o[1]->disp - 1;
        else
            c->IEVsize_t1 = o[1]->disp;
        if (numargs == (num+1) && expectedshift != -1)
        {
            if (o[num]->shiftcode != 1 && o[num]->shiftcode != 0)
                asmerr("%s only supports shifting with ASR or LSL", op->name);
            goto Lencodeshift;
        }
    }
    else if (num == 2)
    {
        c->setRegD(o[0]->base->val);
        c->setRegM(o[1]->base->val);
    }
    else
    {
        c->setRegD(o[0]->base->val);
        c->setRegN(o[1]->base->val);
        c->setRegM(o[2]->base->val);
    }

    if (numargs == (num+1) && expectedshift != -1)
    {
        assert(o[num]->type == OPNDTshift);
        if (o[num]->shiftcode != expectedshift)
            asmerr("%s only supports shifting with %s", op->name, shiftcodestrs[expectedshift]);

Lencodeshift:

        if (o[num]->base)
            asmerr("%s only allows #imm shift, not register shift", op->name);

        switch (o[num]->shiftcode)
        {
            case 0: c->setShiftType(2); break;
            case 1: c->setShiftType(0); break;
            case 2: c->setShiftType(1); break;
            case 3: // fallthrough
            case 4: c->setShiftType(3); break;
            default: assert(0);
        }
        c->setShiftOffset(o[num]->disp);
    }
    else if (c->Iop == OP_PKHTB && numargs == num)
    {
        c->setRegN(o[2]->base->val);
        c->setRegM(o[1]->base->val);
    }

    return c;
}

static code *asm_emit_smult(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (numargs == 2)
        dupFirstOpnd(numargs, o);

    if (c->Iop == OP_SMLALD || c->Iop == OP_SMLALDX ||
        c->Iop == OP_SMLSLD || c->Iop == OP_SMLSLDX)
    {
        c->setRegS(o[0]->base->val);
        c->setRegD(o[1]->base->val);
        c->setRegN(o[2]->base->val);
        c->setRegM(o[3]->base->val);
    }
    else
    {
        c->setRegD(o[0]->base->val);
        c->setRegN(o[1]->base->val);
        c->setRegM(o[2]->base->val);
        if (numargs == 4)
            c->setRegS(o[3]->base->val);
    }

    return c;
}

static size_t encodeRegList(OPND* o)
{
    uint32_t regmask = 0;

    // outer layer is just a holder, hop to the list itself
    o = o->next;
    while (o)
    {
        regmask |= 1 << o->base->val;
        if (o->next && o->next->type == OPNDTregdlist)
        {
            // o and o->next form a list pair
            for (size_t i = o->base->val; i <= o->next->base->val; ++i)
                regmask |= 1 << i;

            o = o->next; // skip regdlist node
        }

        o = o->next;
    }

    return regmask;
}

static code *asm_emit_blockdata(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    int list;
    if (c->Iop == OP_PUSH || c->Iop == OP_POP)
    {
        list = 0;
        c->setAddrMode(IAMMode_postidx);
        c->setRegN(SP);
    }
    else
    {
        list = 1;
        c->setRegN(o[0]->base->val);
        if (o[0]->type == OPNDTregwrite)
            c->setAddrMode(IAMMode_postidx);
    }

    c->IFL1 = FLconst;
    c->IEVsize_t1 = encodeRegList(o[list]);

    return c;
}

static code *asm_emit_branch(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    assert(numargs == 1);
    assert(o[0]->s != NULL || o[0]->base != NULL); // for now, only supports branch to symbol

    code* c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    if (o[0]->s)
    {
        if (op->Iop == OP_BLX && asmstate.ucIcc != COND_AL)
            asmerr("BLX immediate must be unconditional");

        symbol_to_fl(o[0]->s, o[0]->disp, c);
    }
    else if (o[0]->base)
    {
        if (o[0]->base->type != REGTcore)
            asmerr(EM_wrong_reg_type, "core");

        c->setRegM(o[0]->base->val);
    }
    return c;
}

// begin old code

// needs to parse it's operands since they're not handled by the regular parser
static code *asm_emit_cdp(Loc loc, OP *op, size_t numargs, OPND* opnds[])
{
    if (asmstate.ucIcc != COND_AL)
        asmerr("illegal condition code: %d", asmstate.ucIcc);

    assert(0); // incomplete
    code*c = code_calloc();
    c->Iop = op->Iop;
    c->setCC(asmstate.ucIcc);

    // way temporary
    while (tok_value != TOKeof) asm_token();
}

// end old code

static code *asm_emit_odd(Loc loc, OP* op, size_t numargs, OPND* opnds[])
{
    assert(0); // incomplete
    // way temporary
    while (tok_value != TOKeof) asm_token();

    return NULL;
}

static code *asm_emit_unsupported(Loc loc, OP *op, size_t numargs, OPND* o[])
{
    asmerr("unsupported opcode: %s", op->name);
    return NULL;
}

static code *asm_emit(Loc loc, OP *op, size_t numargs, OPND* opnds[])
{
#if DEBUG
    printf("asm_emit(%d): ", loc.linnum);
    asm_output_op(op);
    if (op->type == IToddinstr)
        printf(" -- operands parsed later\n");
    else
    {
        printf(" ");
        for (size_t i=0; i<numargs; ++i)
        {
            if (opnds[i])
            {
                if (i != 0)
                    printf(", ");
                asm_output_popnd(opnds[i]);
            }
        }
        printf("\n");
    }
#endif

    if (numargs < op->minargs || numargs > op->maxargs)
    {
        error(loc, "%s requires %zu-%zu args, not %zu",
              op->name, op->minargs, op->maxargs, numargs);
        return NULL;
    }

    code* c = op->fp(loc, op, numargs, opnds);
    if (c)
        c = asm_genloc(loc, c);
    return c;
}

/*******************************
 * Prepend line number to c.
 */

code *asm_genloc(Loc loc, code *c)
{
    if (global.params.symdebug)
    {
        Srcpos srcpos;
        memset(&srcpos, 0, sizeof(srcpos));
        srcpos.Slinnum = loc.linnum;
        srcpos.Sfilename = (char *)loc.filename;

        code* pcLin = genlinnum(NULL, srcpos);
        c = cat(pcLin, c);
    }
    return c;
}


/********************************/

static void asmerr(int errnum, ...)
{
    const char *p = asmstate.loc.toChars();
    if (*p)
        printf("%s: ", p);

    const char *format = asmerrmsgs[errnum];
    va_list ap;
    va_start(ap, errnum);
    vprintf(format, ap);
    va_end(ap);

    printf("\n");
    fflush(stdout);
    longjmp(asmstate.env,1);
}

/********************************/

static void asmerr(const char *format, ...)
{
    const char *p = asmstate.loc.toChars();
    if (*p)
        printf("%s: ", p);

    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);

    printf("\n");
    fflush(stdout);

    longjmp(asmstate.env,1);
}

/********************************/

static bool asm_isint(OPND *o)
{
    if (!o || o->base || o->s)
        return 0;
    return 1;
}

static bool asm_isNonZeroInt(OPND *o)
{
    if (!o || o->base || o->s)
        return 0;
    return o->disp != 0;
}

/*******************************
 * Merge operands o1 and o2 into a single operand.
 */

static OPND *asm_merge_opnds(OPND * o1, OPND * o2)
{
    const char *psz;

#ifdef DEBUG
    if (debuga)
    {
        printf("asm_merge_opnds(o1 = ");
        if (o1) asm_output_popnd(o1);
        printf(", o2 = ");
        if (o2) asm_output_popnd(o2);
        printf(")\n");
    }
#endif

    if (!o1)
        return o2;
    if (!o2)
        return o1;

    // combine the OPND's symbol field
    if (o1->s && o2->s)
    {
        psz = "o1->s && os->s";
Lunexpected:
        asmerr("internal error: %s", psz);
    }
    else if (o2->s)
        o1->s = o2->s;
    else if (o1->s && o1->s->isTupleDeclaration())
    {
        TupleDeclaration *tup = o1->s->isTupleDeclaration();

        size_t index = o2->disp;
        if (index >= tup->objects->length)
            error(asmstate.loc, "tuple index %u exceeds length %u", index, tup->objects->length);
        else
        {
            RootObject *o = tup->objects->tdata()[index];
            if (o->dyncast() == DYNCAST_DSYMBOL)
            {
                o1->s = (Dsymbol *)o;
                return o1;
            }
            else if (o->dyncast() == DYNCAST_EXPRESSION)
            {
                Expression *e = (Expression *)o;
                if (e->op == TOKvar)
                {
                    o1->s = ((VarExp *)e)->var;
                    return o1;
                }
                else if (e->op == TOKfunction)
                {
                    o1->s = ((FuncExp *)e)->fd;
                    return o1;
                }
            }
            error(asmstate.loc, "invalid asm operand %s", o1->s->toChars());
        }
    }

    if (o1->disp && o2->disp)
        o1->disp += o2->disp;
    else if (o2->disp)
        o1->disp = o2->disp;

    /* combine the OPND's base field */
    if (o1->base != NULL && o2->base != NULL)
    {
        psz = "o1->base != NULL && o2->base != NULL";
        goto Lunexpected;
    }
    else if (o2->base)
        o1->base = o2->base;

    /* Combine the displacement register fields */
    if (o2->regDisp1)
    {
        if (o1->regDisp1)
        {
            psz = "o2->regDisp1 && o1->regDisp1";
            goto Lunexpected;
        }

        o1->regDisp1 = o2->regDisp1;
        if (o2->next)
        {
            if (o1->next)
            {
                psz = "o2->next && o1->next";
                goto Lunexpected;
            }

            o1->next = o2->next;
            o2->next = NULL;
        }
    }

#if 0
    if (o2->ptype && !o1->ptype)
        o1->ptype = o2->ptype;
    if (o2->bOffset)
        o1->bOffset = o2->bOffset;

    if (o2->ajt && !o1->ajt)
        o1->ajt = o2->ajt;
#endif

    opnd_free(o2);
    o2 = NULL;
#ifdef DEBUG
    if (debuga)
    {
        printf("Merged result = /");
        asm_output_popnd(o1);
        printf("/\n");
    }
#endif
    return o1;
}

#if 0
/****************************************/

static void asm_merge_symbol(OPND *o1, Dsymbol *s)
{
    //printf("asm_merge_symbol(s = %s %s)\n", s->kind(), s->toChars());
    s = s->toAlias();
    //printf("s = %s %s\n", s->kind(), s->toChars());
    if (s->isLabel())
    {
        o1->s = s;
        return;
    }

    VarDeclaration *v = s->isVarDeclaration();
    if (v)
    {
        if (v->isParameter())
            asmstate.statement->refparam = TRUE;

        v->checkNestedReference(asmstate.sc, asmstate.loc);
        if (v->storage_class & STCfield)
        {
            o1->disp += v->offset;
            goto L2;
        }
        if ((v->isConst()
#if DMDV2
                || v->isImmutable() || v->storage_class & STCmanifest
#endif
            ) && !v->type->isfloating() && v->_init)
        {
            ExpInitializer *ei = v->_init->isExpInitializer();

            if (ei)
            {
                o1->disp = ei->exp->toInteger();
                return;
            }
        }
    }

    {
        EnumMember *em = s->isEnumMember();
        if (em)
        {
            o1->disp = em->value()->toInteger();
            return;
        }
    }

    // else a C identifier
    o1->s = s;

L2:
    Declaration *d = s->isDeclaration();
    if (!d)
    {
        asmerr("%s %s is not a declaration", s->kind(), s->toChars());
    }
    else if (d->getType())
        asmerr(EM_type_as_operand, d->getType()->toChars());
    else if (d->isTupleDeclaration())
        ;
    else
        o1->ptype = d->type->toBasetype();
}
#endif

#ifdef DEBUG

/*******************************
 */

static void asm_output_op(OP *op)
{
    printf("%s", op->name);
    if (asmstate.flags & Fsetcc)
        printf("S");
    if (asmstate.ucIcc != COND_AL)
        printf("%s", condcodestrs[asmstate.ucIccIdx]);
    if (asmstate.flags & FhasSizePrefix)
        printf(".%s", sizesuffixstrs[asmstate.sizeIdx]);
}

static void asm_output_popnd(OPND *o)
{
    assert(o);
    switch (o->type)
    {
        case OPNDTreg:
        case OPNDTregwrite:
        case OPNDTregidx:
        case OPNDTregdlist:
            assert(o->base);
            printf("%s", o->base->regstr);
            if (o->type == OPNDTregwrite)
                printf("!");
            else if (o->type == OPNDTregidx)
                printf("[%lld]", o->disp);
            break;

        case OPNDTreglist:
        {
            printf("{ ");
            OPND* list = o->next;
            while (list)
            {
                asm_output_popnd(list);
                list = list->next;
                printf(list ? (list->type == OPNDTregdlist ? "-" : ", ") : " ");
            }
            printf("}");
            break;
        }

        case OPNDTshift:
            printf("%s ", shiftcodestrs[o->shiftcode]);
            if (o->base)
                printf("%s", o->base->regstr);
            else if (o->disp != 0)
                printf("#%lld", o->disp);
            break;

        case OPNDTliteral:
            if (o->s)
                printf("%s", o->s->ident->toChars());
            else
                printf("#%lld", o->disp);
            break;

        case OPNDToffset:
        case OPNDTpreidx:
        case OPNDTpostidx:
            assert(o->base);
            printf("[%s", o->base->regstr);
            if (o->type == OPNDToffset || o->type == OPNDTpreidx)
            {
                if (o->regDisp1)
                {
                    printf(", %s", o->regDisp1->regstr);
                    if (o->next)
                    {
                        printf(", ");
                        if (asmstate.flags & Fsubtract)
                            printf("-");
                        asm_output_popnd(o->next);
                    }
                }
                else if (o->disp != 0)
                    printf(", #%lld", o->disp);
            }
            printf("]");
            if (o->type == OPNDTpreidx)
                printf("!");
            if (o->type == OPNDTpostidx)
            {
                if (o->regDisp1)
                {
                    printf(", %s", o->regDisp1->regstr);
                    if (o->next)
                    {
                        printf(", ");
                        if (asmstate.flags & Fsubtract)
                            printf("-");
                        asm_output_popnd(o->next);
                    }
                }
                else if (o->disp != 0)
                    printf(", #%lld", o->disp);
            }
            break;

        default:
            assert(0);
    }

#if 0
    if (popnd->base)
        printf("%s", popnd->base->regstr);
    else if (popnd->s)
        printf("%s", popnd->s->ident->toChars());

    if (popnd->regDisp1)
    {
        if (popnd->regDisp2)
        {
            printf("[%s+%s]",
                    popnd->regDisp1->regstr,
                    popnd->regDisp2->regstr);
        }
        else
        {
            printf("[%s]",
                    popnd->regDisp1->regstr);
        }
    }
    if (popnd->disp)
        printf("+%llxh", popnd->disp);
#endif
}

#endif

/*******************************
 */

static int asm_shiftcode_lookup(const char *s)
{
    //dbg_printf("asm_shiftcode_lookup('%s')\n",s);

    int i = binary(s, shiftcodestrs, sizeof(shiftcodestrs) / sizeof(shiftcodestrs[0]));

    return i;
}

static REG *asm_reg_lookup(const char *s, ssize_t len) // len defaults to -1
{
    //dbg_printf("asm_reg_lookup('%s')\n",s);

    if (len == -1) len = strlen(s);
    int i = binary(s, len, regstrs, sizeof(regstrs) / sizeof(regstrs[0]));
    if (i >= 0) return &regs[i];

    return NULL;
}

/********************************/

static void asm_token()
{
    if (asmtok)
        asmtok = asmtok->next;
    asm_token_trans(asmtok);
}

/********************************/

static void asm_token_trans(Token *tok)
{
    tok_value = TOKeof;
    if (tok)
    {
        tok_value = tok->value;
        if (tok_value == TOKidentifier)
        {
            const char *id = tok->ident->toChars();
            size_t len = strlen(id);
            if (len < 20)
            {
                ASMTK asmtk = (ASMTK) binary(id, apszAsmtk, ASMTKmax);
                if ((int)asmtk >= 0)
                    tok_value = (enum TOK) (asmtk + TOKMAX + 1);
            }
        }
    }
}

#if 0

/*******************************************
 * Parse DA expression
 *
 * Very limited define address to place a code
 * address in the assembly
 * Problems:
 *      o       Should use dw offset and dd offset instead,
 *              for near/far support.
 *      o       Should be able to add an offset to the label address.
 *      o       Blocks addressed by DA should get their Bpred set correctly
 *              for optimizer.
 */

static code *asm_da_parse(OP *pop)
{
    code *clst = NULL;

    while (1)
    {
        code *c;

        if (tok_value == TOKidentifier)
        {
            LabelDsymbol *label;

            label = asmstate.sc->func->searchLabel(asmtok->ident);
            if (!label)
                error(asmstate.loc, "label '%s' not found\n", asmtok->ident->toChars());

            c = code_calloc();
            c->Iop = ASM;
            c->Iflags = CFaddrsize;
            c->IFL1 = FLblockoff;
            c->IEVlsym1 = label;
            c = asm_genloc(asmstate.loc, c);
            clst = cat(clst,c);
        }
        else
            asmerr(EM_bad_addr_mode);   // illegal addressing mode
        asm_token();
        if (tok_value != TOKcomma)
            break;
        asm_token();
    }

    asmstate.statement->regs |= mES|ALLREGS;
    asmstate.bReturnax = TRUE;

    return clst;
}

/*******************************************
 * Parse DB, DW, DD, DQ and DT expressions.
 */

static code *asm_db_parse(OP *pop)
{
    unsigned usSize;
    unsigned usMaxbytes;
    unsigned usBytes;
    union DT
    {
        targ_ullong ul;
        targ_float f;
        targ_double d;
        targ_ldouble ld;
        char value[10];
    } dt;
    code *c;
    unsigned op;
    static unsigned char opsize[] = { 1,2,4,8,4,8,10 };

    op = pop->usNumops & ITSIZE;
    usSize = opsize[op];

    usBytes = 0;
    usMaxbytes = 0;
    c = code_calloc();
    c->Iop = ASM;

    while (1)
    {
        size_t len;
        unsigned char *q;

        if (usBytes+usSize > usMaxbytes)
        {
            usMaxbytes = usBytes + usSize + 10;
            c->IEV1.as.bytes = (char *)mem_realloc(c->IEV1.as.bytes,usMaxbytes);
        }
        switch (tok_value)
        {
            case TOKint32v:
                dt.ul = (d_int32)asmtok->int64value;
                goto L1;
            case TOKuns32v:
                dt.ul = (d_uns32)asmtok->uns64value;
                goto L1;
            case TOKint64v:
                dt.ul = asmtok->int64value;
                goto L1;
            case TOKuns64v:
                dt.ul = asmtok->uns64value;
                goto L1;
            L1:
                switch (op)
                {
                    case OPdb:
                    case OPds:
                    case OPdi:
                    case OPdl:
                        break;
                    default:
                        asmerr(EM_float);
                }
                goto L2;

            case TOKfloat32v:
            case TOKfloat64v:
            case TOKfloat80v:
                switch (op)
                {
                    case OPdf:
                        dt.f = asmtok->floatvalue;
                        break;
                    case OPdd:
                        dt.d = asmtok->floatvalue;
                        break;
                    case OPde:
                        dt.ld = asmtok->floatvalue;
                        break;
                    default:
                        asmerr(EM_num);
                }
                goto L2;

            L2:
                memcpy(c->IEV1.as.bytes + usBytes,&dt,usSize);
                usBytes += usSize;
                break;

            case TOKstring:
                len = asmtok->len;
                q = asmtok->ustring;
            L3:
                if (len)
                {
                    usMaxbytes += len * usSize;
                    c->IEV1.as.bytes =
                        (char *)mem_realloc(c->IEV1.as.bytes,usMaxbytes);
                    memcpy(c->IEV1.as.bytes + usBytes,asmtok->ustring,len);

                    char *p = c->IEV1.as.bytes + usBytes;
                    for (size_t i = 0; i < len; i++)
                    {
                        // Be careful that this works
                        memset(p, 0, usSize);
                        switch (op)
                        {
                            case OPdb:
                                *p = (unsigned char)*q;
                                if (*p != *q)
                                    asmerr(EM_char);
                                break;

                            case OPds:
                                *(short *)p = *(unsigned char *)q;
                                if (*(short *)p != *q)
                                    asmerr(EM_char);
                                break;

                            case OPdi:
                            case OPdl:
                                *(long *)p = *q;
                                break;

                            default:
                                asmerr(EM_float);
                        }
                        q++;
                        p += usSize;
                    }

                    usBytes += len * usSize;
                }
                break;

            case TOKidentifier:
            {
                Expression *e = new IdentifierExp(asmstate.loc, asmtok->ident);
                e = expressionSemantic(e, asmstate.sc);
                e = e->optimize(WANTvalue | WANTinterpret);
                if (e->op == TOKint64)
                {
                    dt.ul = e->toInteger();
                    goto L2;
                }
                else if (e->op == TOKfloat64)
                {
                    switch (op)
                    {
                        case OPdf:
                            dt.f = e->toReal();
                            break;
                        case OPdd:
                            dt.d = e->toReal();
                            break;
                        case OPde:
                            dt.ld = e->toReal();
                            break;
                        default:
                            asmerr(EM_num);
                    }
                    goto L2;
                }
                else if (e->op == TOKstring)
                {
                    StringExp *se = (StringExp *)e;
                    q = (unsigned char *)se->string;
                    len = se->len;
                    goto L3;
                }
                goto Ldefault;
            }

            default:
            Ldefault:
                asmerr(EM_const_init);          // constant initializer
                break;
        }
        c->IEV1.as.len = usBytes;

        asm_token();
        if (tok_value != TOKcomma)
            break;
        asm_token();
    }

    c = asm_genloc(asmstate.loc, c);

    asmstate.statement->regs |= /* mES| */ ALLREGS;
    asmstate.bReturnax = TRUE;

    return c;
}
#endif

/**********************************
 * Parse and get integer expression.
 */

int asm_getnum()
{
    int v;
    dinteger_t i;

    switch (tok_value)
    {
        case TOKint32v:
            v = (d_int32)asmtok->int64value;
            break;

        case TOKuns32v:
            v = (d_uns32)asmtok->uns64value;
            break;

        case TOKidentifier:
        {
            Expression *e = IdentifierExp::create(asmstate.loc, asmtok->ident);
            Scope *sc = asmstate.sc->startCTFE();
            e = expressionSemantic(e, asmstate.sc);
            sc->endCTFE();
            e = e->ctfeInterpret();
            i = e->toInteger();
            v = (int) i;
            if (v != i)
                asmerr(EM_num);
            break;
        }

        default:
            asmerr(EM_num);
            v = 0;              // no uninitialized values
            break;
    }
    asm_token();
    return v;
}

/********************************/

void iasm_term()
{
    if (asmstate.bInit)
    {
        asmstate.psDollar = NULL;
        asmstate.psLocalsize = NULL;
        asmstate.bInit = 0;
    }
}

/**********************************
 * Return mask of registers used by block bp.
 */

regm_t iasm_regs(block *bp)
{
#ifdef DEBUG
    if (debuga)
        printf("Block iasm regs = 0x%X\n", bp->usIasmregs);
#endif

    refparam |= bp->bIasmrefparam;
    return bp->usIasmregs;
}

static void asm_lookup_symbol(OPND* o)
{
    assert(o);

    Dsymbol *s = NULL;

    if (asmstate.sc->func->labtab)
        s = asmstate.sc->func->labtab->lookup(asmtok->ident);

    if (!s)
    {
        Dsymbol *scopesym; // junk
        s = asmstate.sc->search(Loc(), asmtok->ident, &scopesym);
    }

    if (!s) s = asmstate.sc->func->searchLabel(asmtok->ident);

    if (!s) asmerr(EM_undefined_identifier, asmtok->ident->toChars());

    asm_token();

    s = s->toAlias();
    //printf("s = %s %s\n", s->kind(), s->toChars());

    if (s->isLabel())
    {
        o->s = s;
        return;
    }

    VarDeclaration *v = s->isVarDeclaration();
    if (v)
    {
        if (v->isParameter())
            asmstate.statement->refparam = TRUE;

        v->checkNestedReference(asmstate.sc, asmstate.loc);
        if (v->storage_class & STCfield)
        {
            o->disp += v->offset;
            goto L2;
        }
        if ((v->isConst() || v->isImmutable() || v->storage_class & STCmanifest) &&
            !v->type->isfloating() && v->_init)
        {
            ExpInitializer *ei = v->_init->isExpInitializer();

            if (ei)
            {
                o->disp = ei->exp->toInteger();
                return;
            }
        }
    }

    {
        EnumMember *em = s->isEnumMember();
        if (em)
        {
            o->disp = em->value()->toInteger();
            return;
        }
    }

    // else a C identifier
    o->s = s;

L2:
    Declaration *d = s->isDeclaration();
    if (!d)
        asmerr("%s %s is not a declaration", s->kind(), s->toChars());
    else if (d->getType())
        asmerr("cannot use type %s as an operand", d->getType()->toChars());
    else if (d->isTupleDeclaration())
        ;
    else
    {
        // TODO: ?
        //assert(0);
        //o->ptype = d->type->toBasetype();
    }
}

static OPND* asm_parse_reg(bool required)
{
    if (tok_value != TOKidentifier) asmerr(EM_bad_register, asmtok->toChars());

    REG* regp = asm_reg_lookup(asmtok->ident->toChars());
    if (!regp)
    {
        if (required) asmerr("expected a register, not: %s", asmtok->ident->toChars());
        return NULL;
    }

    asm_token();

    OPND* o = opnd_calloc();
    o->type = OPNDTreg;
    o->base = regp;

    if (tok_value == TOKnot)
    {
        asm_token();
        o->type = OPNDTregwrite;
    }
    else if (tok_value == TOKlbracket)
    {
        asm_token();
        o->disp = asm_getnum();
        o->type = OPNDTregidx;

        asm_chktok(TOKrbracket);
    }

    return o;
}

static OPND* asm_parse_shift(bool required)
{
    int scidx = asm_shiftcode_lookup(asmtok->ident->toChars());
    if (scidx < 0)
    {
        if (required) asmerr("unknown shift operation: %s", asmtok->ident->toChars());
        return NULL;
    }

    asm_token();

    OPND* o = opnd_calloc();
    o->type = OPNDTshift;
    o->shiftcode = scidx;

    if (scidx == 4) // RXX doesn't have an amount
        return o;

    switch (tok_value)
    {
        case TOKidentifier:
        {
            const char * tokstr = asmtok->ident->toChars();
            REG* shiftreg = asm_reg_lookup(tokstr);
            if (!shiftreg) asmerr("unknown shift register: %s", tokstr);

            asm_token();
            o->base = shiftreg;
            break;
        }

        case TOKpound:
            asm_token();

            o->disp = asm_getnum();
            break;

        default:
            asmerr("expected a register or immediate value, not: %s", asmtok->toChars());
            break;
    }
    return o;
}

static OPND* asm_parse_literal()
{
    OPND* o = opnd_calloc();
    o->type = OPNDTliteral;

    asm_lookup_symbol(o);

    return o;
}

static OPND* asm_parse_reg_or_shift()
{
    OPND* o;

    o = asm_parse_reg(false);
    if (o) return o;

    o = asm_parse_shift(false);
    if (o) return o;

    o = asm_parse_literal(); // maybe rename to label?
    if (o) return o;

    return NULL;
}

static OPND* asm_parse_offset()
{
    OPND* o = opnd_calloc();

    switch (tok_value)
    {
        case TOKpound:
        {
            asm_token();

            bool neg = false;
            if (tok_value == TOKadd)
                asm_token();
            else if (tok_value == TOKmin)
            {
                asm_token();
                neg = true;
            }
            o->disp = asm_getnum();
            if (neg)
                o->disp = - o->disp;
            break;
        }

        case TOKmin:
            asmstate.flags |= Fsubtract;
            // fall through

        case TOKadd:
            asm_token();
            // fall through

        case TOKidentifier:
        {
            OPND* opreg = asm_parse_reg(true);

            o->regDisp1 = opreg->base;
            opnd_free(opreg);

            if (tok_value == TOKcomma)
            {
                asm_token();

                o->next = asm_parse_shift(true);
            }
            break;
        }

        default:
            asmerr(EM_unexpected_token, asmtok->toChars());
    }

    return o;
}

static OPND* asm_parse_index()
{
    OPND* o = opnd_calloc();
    o->type = OPNDToffset;

    assert(tok_value == TOKlbracket);
    asm_token();

    {
        OPND* o1 = asm_parse_reg(true);
        o = asm_merge_opnds(o, o1);
    }

    switch (tok_value)
    {
        case TOKrbracket:
            asm_token();

            switch (tok_value)
            {
                case TOKcomma:
                {
                    asm_token();

                    o->type = OPNDTpostidx;

                    OPND* o1 = asm_parse_offset();
                    o = asm_merge_opnds(o, o1);
                    break;
                }

                case TOKnot:
                    asm_token();
                    o->type = OPNDTpreidx;
                    break;
            }
            break;

        case TOKcomma:
        {
            asm_token();

            OPND* o1 = asm_parse_offset();
            o = asm_merge_opnds(o, o1);

            asm_chktok(TOKrbracket);

            if (tok_value == TOKnot)
            {
                asm_token();
                o->type = OPNDTpreidx;
            }
            break;
        }

        default:
            asmerr(EM_unexpected_token, asmtok->toChars());
            break;
    }

    return o;
}

static OPND* asm_parse_list()
{
    assert(tok_value == TOKlcurly);

    asm_token();

    OPND* o = opnd_calloc();
    o->type = OPNDTreglist;

    OPND* last = o;
    while (tok_value != TOKrcurly)
    {
        OPND *o1 = asm_parse_reg(true);

        last->next = o1;
        last = o1;

        switch (tok_value)
        {
            case TOKcomma:
                asm_token();
                continue;

            case TOKmin:
            {
                asm_token();

                OPND *o2 = asm_parse_reg(true);
                o2->type = OPNDTregdlist;

                last->next = o2;
                last = o2;

                if (strcmp(o1->base->regstr, o2->base->regstr) > 0)
                    asmerr("register %s and %s must be in increasing order", o1->base->regstr, o2->base->regstr);

                switch (tok_value)
                {
                    case TOKcomma:
                        asm_token();
                        continue;

                    case TOKrcurly:
                        break;

                    default:
                        asmerr(EM_unexpected_token, asmtok->toChars());
                        break;
                }
            }

            case TOKrcurly:
                break;

            default:
                asmerr(EM_unexpected_token, asmtok->toChars());
        }
    }

    asm_token(); // swallow rcurly

    return o;
}

static OPND* asm_parse_operand()
{
    OPND *o = NULL;
    switch (tok_value)
    {
        case TOKlbracket:
            o = asm_parse_index();
            break;

        case TOKlcurly:
            o = asm_parse_list();
            break;

        case TOKidentifier:
            o = asm_parse_reg_or_shift();
            break;

        case TOKpound:
            asm_token();
            o = opnd_calloc();
            o->type = OPNDTliteral;
            o->disp = asm_getnum();
            break;

        default:
            asmerr(EM_unexpected_token, asmtok->toChars());
            break;
    }

    assert(o);
    assert(o->type != 0);
    return o;
}

static bool approx(longdouble value, longdouble expected)
{
    return (fabsl(value-expected) < 0.0001);
}

static void asm_parse_opcode_suffix()
{
    const char* s = NULL;
    if (tok_value == TOKidentifier)
    {
        s = asmtok->ident->toChars();
Lsize:
        // check for a length suffix (see SIZESUFFIXES)
        int idx = binary(s, sizesuffixstrs, sizeof(sizesuffixstrs) / sizeof(sizesuffixstrs[0]));
        if (idx >= 0)
        {
            asm_token();

            asmstate.sizeIdx = idx;
            asmstate.flags |= FhasSizePrefix;
            return;
        }

        // TODO: check for an encoding length suffix (.N or .W)

        // unknown suffix
        asmerr("unknown opcode suffix: %s", s);
        return;
    }

    // this makes me sad, but .8 style suffixes are parsed by the lexer as a float
    if (tok_value == TOKfloat64v)
    {
        s = NULL;
        if (approx(asmtok->floatvalue, 0.8))
            s = "8";
        else if (approx(asmtok->floatvalue, 0.16))
            s = "16";
        else if (approx(asmtok->floatvalue, 0.32))
            s = "32";
        else if (approx(asmtok->floatvalue, 0.64))
            s = "64";

        if (!s) asmerr("unknown opcode suffix: %s", asmtok->toChars());

        goto Lsize;
    }
    
    // unknown suffix
    asmerr("unknown opcode suffix: %s", s);
}

/**********************************
 * Parse opcode string.
 * Return pointer to the corresponding OP struct.
 * Also store the optional condition code in asmstate.ucIcc.
 */

OP* asm_parse_opcode(const char *s, size_t len)
{
    asmstate.ucIcc = COND_AL; // always execute instruction
    size_t toklen = len;

    // check for a condition code
    if (toklen > 2)
    {
        int ccidx = binary(s+len-2, condcodestrs, sizeof(condcodestrs) / sizeof(condcodestrs[0]));
        if (ccidx >= 0)
        {
            // validate that with the cc removed, the token is still valid (some opcodes end with matching values)
            OP* op = asm_parse_opcode(s, toklen-2);
            if (op)
            {
                asmstate.ucIcc = condcodes[ccidx];
                asmstate.ucIccIdx = ccidx;
                return op;
            }
        }
    }

    // check for a set condition code suffix
    if (s[toklen-1] == 'S')
    {
        int opidx = binary(s, toklen-1, opcodestrs, sizeof(opcodestrs) / sizeof(opcodestrs[0]));
        if (opidx >= 0)
        {
            asmstate.flags |= Fsetcc;
            return &opcodes[opidx];
        }
    }

    int opidx = binary(s, toklen, opcodestrs, sizeof(opcodestrs) / sizeof(opcodestrs[0]));
    if (opidx >= 0) return &opcodes[opidx];

    return NULL;
}

static code* asm_opcode_exp()
{
    code* c = NULL;
    OPND* operands[4];
    size_t numopers = 0;
    for (size_t i = 0; i < 4; i++) operands[i] = NULL;

    const char* str = asmtok->ident->toChars();
    OP* o = asm_parse_opcode(str, strlen(str));
    if (!o) asmerr(EM_opcode_exp, str);

    asm_token(); // swallow opcode

    asmstate.ucItype = o->type;

    // odd instructions are handled in their emitter
    if (o->type == IToddinstr)
        goto Lemit;

    while (tok_value == TOKdot || tok_value == TOKfloat64v)
    {
        if (tok_value == TOKdot)
            asm_token();

        asm_parse_opcode_suffix();
    }

    while (tok_value != TOKeof)
    {
        //printf("parsing operand #%d: %s\n", numopers, asmtok->toChars());
        operands[numopers] = asm_parse_operand();
        //printf("parsed: "); asm_output_popnd(operands[numopers]); printf("\n");
        ++numopers;
        if (numopers > 4) asmerr("too many operands");

        switch (tok_value)
        {
            case TOKcomma:
                asm_token(); // swallow comma
                break;

            case TOKeof:
                break;

            default:
                asmerr(EM_unexpected_token, asmtok->toChars());
        }
    }

Lemit:
    c = asm_emit(asmstate.loc, o, numopers, operands);

    for (size_t i = 0; i < numopers; ++i) opnd_free(operands[i]);

    return c;
}

/************************ InlineAsmStatement ***************************************/

Statement* inlineAsmSemantic(InlineAsmStatement *s, Scope *sc)
{
    //printf("InlineAsmStatement::semantic()\n");

    assert(sc->func);
#if DMDV2
    if (sc->func->setUnsafe())
        s->error("inline assembler not allowed in @safe function %s", sc->func->toChars());
#endif

    FuncDeclaration *fd = sc->parent->isFuncDeclaration();
    assert(fd);

    if (!s->tokens)
        return NULL;

    memset(&asmstate, 0, sizeof(asmstate));

    asmstate.statement = s;
    asmstate.sc = sc;

    // Assume assembler code takes care of setting the return value
    sc->func->hasReturnExp |= 8;

    if (!asmstate.bInit)
    {
        //debuga = 1;
        asmstate.bInit = TRUE;
        asmstate.psDollar = new LabelDsymbol(Id::_dollar);
        asmstate.psLocalsize = new Dsymbol(Id::__LOCAL_SIZE);
    }

    asmstate.loc = s->loc;

    asmtok = s->tokens;
    asm_token_trans(asmtok);
    if (setjmp(asmstate.env))
    {
        asmtok = NULL;                  // skip rest of line
        tok_value = TOKeof;

        // TODO: don't exit, return like any other semantic failure would
        exit(EXIT_FAILURE);
    }

    switch (tok_value)
    {
        case ASMTKnaked:
            s->naked = TRUE;
            sc->func->naked = TRUE;
            asm_token();
            break;

        case TOKalign:
        {
            asm_token();
            unsigned align = asm_getnum();
            if (ispow2(align) == -1)
                asmerr("illegal alignment, must be a power of 2: %d", align);
            else
                s->asmalign = align;
            break;
        }

        case TOKidentifier:
            s->asmcode = asm_opcode_exp();
            break;

        default:
        OPCODE_EXPECTED:
            asmerr(EM_opcode_exp, asmtok->toChars());
            break;
    }

    if (tok_value != TOKeof)
        asmerr("expected ;, not: %s", asmtok->toChars());

    return s;
}


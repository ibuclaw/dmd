// Copyright (C) 2011 by Digital Mars
// All Rights Reserved
// http://www.digitalmars.com
// Written by Brad Roberts
/*
 * This file is licensed under the Boost 1.0 license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "cc.h"
#include "code.h"
#include "global.h"
#include "melf.h"
#include "obj.h"
#include "oper.h"
#include "outbuf.h"
#include "type.h"

static char __file__[] = __FILE__;
#include        "tassert.h"

#if SCPP
#error SCPP not supported
#endif

#if TARGET_SEGMENTED
#error TARGET_SEGMENTED mode not supported on arm
#endif

// NOTES:
//   all functions have a frame for now, so hasframe doesn't exist and can be assumed to be true.

const unsigned dblreg[] = { ~0u };

// cod1.c
int clib_inited = 0;

code *tstresult (regm_t regm, tym_t tym, unsigned saveflag) { assert(0); return NULL; }
void buildEA(code *c,int base,int index,int scale,targ_size_t disp) { assert(0); }
void genEEcode() { assert(0); }
unsigned gensaverestore(regm_t regm,code **csave,code **crestore) { assert(0); return 0; }
unsigned gensaverestore2(regm_t regm,code **csave,code **crestore) { assert(0); return 0; }
void getlvalue_lsw(code *c) { assert(0); }

void cgsched_block(block* b) { }

// cod3.c

code *gensavereg(unsigned& reg, targ_uns slot) { assert(0); return NULL; }
void cod3_thunk(symbol *sthunk, symbol *sfunc, unsigned p, tym_t thisty, targ_size_t d, int i, targ_size_t d2) { assert(0); }
void doswitch(block *b) { assert(0); }
void makeitextern(symbol *s) { assert(0); }
void outjmptab(block *b) { assert(0); }
void outswitab(block *b) { assert(0); }
int cod3_EA(code *c) { assert(0); return 0; }
targ_size_t cod3_bpoffset(symbol *s) { assert(0); return 0; }
targ_size_t cod3_spoff() { assert(0); return 0; }
code *genmovreg(code *c,unsigned to,unsigned from) { assert(0); return NULL; }

void cod3_align_bytes(size_t nbytes) { assert(0); }
bool cse_simple(code* c, elem* e) { assert(0); return false; }
code* gen_testcse(code *c, unsigned sz, targ_uns i) { assert(0); return NULL; }
code* gen_loadcse(code *c, unsigned reg, targ_uns i) { assert(0); return NULL; }
bool isXMMstore(unsigned op) { assert(0); return false; }

regm_t cod3_useBP()
{
    // ARM TODO: currently we're alwasy constructing a full frame
    return 0;
}

void cod3_adjSymOffsets()
{
    //assert(0);
}

void cod3_align()
{
    //assert(0);
}

void simplify_code(code *c)
{
}

/*************************************************
 * Allocate register temporaries
 */

code *REGSAVE::save(code *c, int reg, unsigned *pidx)
{
    unsigned i = idx;
#if 0
    if (reg >= XMM0)
    {
        alignment = 16;
        idx = (idx + 15) & ~15;
        i = idx;
        idx += 16;
        // MOVD idx[RBP],xmm
        c = genc1(c,0xF20F11,modregxrm(2, reg - XMM0, BPRM),FLregsave,(targ_uns) i);
    }
    else
    {
        if (!alignment)
            alignment = REGSIZE;
        i = idx;
        idx += REGSIZE;
        // MOV idx[RBP],reg
        c = genc1(c,0x89,modregxrm(2, reg, BPRM),FLregsave,(targ_uns) i);
        if (I64)
            code_orrex(c, REX_W);
    }
#else
    assert(0);
#endif
    reflocal = TRUE;
    if (idx > top)
        top = idx;              // keep high water mark
    *pidx = i;
    return c;
}

code *REGSAVE::restore(code *c, int reg, unsigned idx)
{
#if 0
    if (reg >= XMM0)
    {
        assert(alignment == 16);
        // MOVD xmm,idx[RBP]
        c = genc1(c,0xF20F10,modregxrm(2, reg - XMM0, BPRM),FLregsave,(targ_uns) idx);
    }
    else
    {   // MOV reg,idx[RBP]
        c = genc1(c,0x8B,modregxrm(2, reg, BPRM),FLregsave,(targ_uns) idx);
        if (I64)
            code_orrex(c, REX_W);
    }
#else
    assert(0);
#endif
    return c;
}

void cgreg_dst_regs(unsigned *dst_integer_reg, unsigned *dst_float_reg)
{
    *dst_integer_reg = R0;
    *dst_float_reg   = R0;
}

void cgreg_set_priorities(tym_t ty, char **pseq, char **pseqmsw)
{
    unsigned sz = tysize(ty);
    if (tyxmmreg(ty))
    {
        assert(0);
    }
    else
    {
        if (sz == REGSIZE * 2)
        {
            // ARM TODO: what should this list be?
            static char seqlsw[] = {R0,R2,R4,NOREG};
            static char seqmsw[] = {R1,R3,R5,NOREG};
            *pseq = seqlsw;
            *pseqmsw = seqmsw;
        }
        else
        {
            // ARM TODO: what should this list be?
            static char sequence[] = {R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,FP,NOREG};
            *pseq = sequence;
        }
    }
}

void cod3_setdefault()
{
    fregsaved  = mR4|mR5|mR6|mR7|mR8|mR9|mR10|mR11; //|     mR13;
}

void cod3_set32()
{
    // what is this for, is it x86 specific?
    BPRM = 5;                       /* [EBP] addressing mode        */

    fregsaved  = mR4|mR5|mR6|mR7|mR8|mR9|mR10|mR11; //|     mR13;

    FLOATREGS  = 0;
    FLOATREGS2 = 0;
    DOUBLEREGS = 0;
}

void cod3_set64()
{
    // 64 bit arm code isn't supported, yet
    assert(0);
}

void cod3_initregs()
{
    ALLREGS    = mR0|mR1|mR2|mR3|mR4|mR5|mR6|mR7|mR8|mR9|mR10|mR11|mR12;
    BYTEREGS   = ALLREGS;
}

const unsigned char* getintegerparamsreglist(tym_t tyf, size_t* num)
{
    static const unsigned char reglist[] = { R0,R1,R2,R3 };
    *num = 4;
    return NULL;
}

const unsigned char* getfloatparamsreglist(tym_t tyf, size_t* num) {
    *num = 0;
    return NULL;
}

// nteh.c
code *nteh_patchindex(code* c, int sindex)       { assert(0); return NULL; }
void nteh_framehandler(symbol *scopetable)       { assert(0); }
code *nteh_prolog()                              { assert(0); return NULL; }
code *nteh_epilog()                              { assert(0); return NULL; }
code *nteh_setsp(int op)                         { assert(0); return NULL; }
code *nteh_filter(block *b)                      { assert(0); return NULL; }
code *nteh_gensindex(int sindex)                 { assert(0); return NULL; }
code *nteh_unwind(regm_t retregs,unsigned index) { assert(0); return NULL; }
code *nteh_monitor_prolog(Symbol *shandle)       { assert(0); return NULL; }
code *nteh_monitor_epilog(regm_t retregs)        { assert(0); return NULL; }

// cod4.c
code *longcmp(elem *e,bool jcond,unsigned fltarg,code *targ) { assert(0); return NULL; }

code* cdabs      (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdasm      (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdbt       (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdbswap    (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdbtst     (elem *e, regm_t *pretregs) { assert(0); return NULL; }

code* cdbyteint  (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdcmpxchg  (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdcnvt     (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdcom      (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdcond     (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdconvt87  (elem *e, regm_t *pretregs) { assert(0); return NULL; }

code* cdctor     (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cddctor    (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdddtor    (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cddtor     (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cderr      (elem *e, regm_t *pretregs) { assert(0); return NULL; }

code* cdfar16    (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdframeptr (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdgot      (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdhalt     (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdind      (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdinfo     (elem *e, regm_t *pretregs) { assert(0); return NULL; }

code* cdlngsht   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdmark     (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdmemcmp   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdmemcpy   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdmemset   (elem *e, regm_t *pretregs) { assert(0); return NULL; }

code* cdmulass   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdmul      (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdneg      (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdnot      (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdnullcheck(elem *e, regm_t *pretregs) { assert(0); return NULL; }

code* cdport     (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdpopcnt   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdpost     (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdrndtol   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdscale    (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdsetjmp   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdshass    (elem *e, regm_t *pretregs) { assert(0); return NULL; }

code* cdstrcmp   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdstrcpy   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdstreq    (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdstrlen   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdstrthis  (elem *e, regm_t *pretregs) { assert(0); return NULL; }

code* cdvecfill  (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdvecsto   (elem *e, regm_t *pretregs) { assert(0); return NULL; }
code* cdvector   (elem *e, regm_t *pretregs) { assert(0); return NULL; }

// forwrad decl, move to code_arm.h?
code *loadea (elem *e , code *cs , unsigned op , unsigned reg , targ_size_t offset , regm_t keepmsk , regm_t desmsk );

#include "arm_genfuncs.c"
#include "arm_block.c"
#include "arm_codout.c"
#include "arm_utils.c"

#include "arm_cdaddass.c"
#include "arm_cdbscan.c"
#include "arm_cdcmp.c"
#include "arm_cdcomma.c"
#include "arm_cdeq.c"

#include "arm_cdfunc.c"
#include "arm_cdloglog.c"
#include "arm_cdmsw.c"
#include "arm_cdorth.c"
#include "arm_cdpair.c"

#include "arm_cdrelconst.c"
#include "arm_cdshift.c"
#include "arm_cdshtlng.c"
#include "arm_cdvoid.c"


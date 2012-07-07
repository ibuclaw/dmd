// Copyright (C) 2011-2012 by Brad Roberts
// License: Boost 1.0

/* Register definitions */

enum
{
    R0       = 0,
    R1       = 1,
    R2       = 2,
    R3       = 3,
    R4       = 4,
    R5       = 5,
    R6       = 6,
    R7       = 7,
    R8       = 8,
    R9       = 9,
    R10      = 10,
    R11      = 11,
    R12      = 12,
    R13      = 13,
    R14      = 14,
    R15      = 15,
    PSW      = 16, // status word
    STACK    = 17, // stack
    NOREG    = 18, // no register
};

// alias a few registers
#define PICREG  R6
#define BP      R11  // aka frame pointer or base pointer
#define FP      R11  // aka frame pointer or base pointer
#define IP      R12  // used by the linker to calculate function addresses
#define SP      R13  // stack pointer
#define LR      R14  // link register aka return address
#define PC      R15  // program counter
#define mBP     mR11
#define mFP     mR11
#define mIP     mR12
#define mSP     mR13
#define mLR     mR14
#define mPC     mR15

#define NUMGENREGS 16
#define NUMREGS    16

enum
{
    mR0      = (1 << R0),
    mR1      = (1 << R1),
    mR2      = (1 << R2),
    mR3      = (1 << R3),
    mR4      = (1 << R4),
    mR5      = (1 << R5),
    mR6      = (1 << R6),
    mR7      = (1 << R7),
    mR8      = (1 << R8),
    mR9      = (1 << R9),
    mR10     = (1 << R10),
    mR11     = (1 << R11),
    mR12     = (1 << R12),
    mR13     = (1 << R13),
    mR14     = (1 << R14),
    mR15     = (1 << R15),
    mPSW     = (1 << PSW),
    mSTACK   = (1 << STACK),
};

// Flags for getlvalue (must fit in regm_t, and not overlap a register)
#define RMload  (1 << 30)
#define RMstore (1 << 31)

// parts of the code still use this sucker
#define mES     0
#define mBX     0
#define mCX     0
#define mSI     0
#define mDI     0

#define mMSW    (mR1|mR3|mR5|mR7|mR9|mR11)
#define mLSW    (mR0|mR2|mR4|mR6|mR8|mR10)

#define IDXREGS ALLREGS

extern regm_t ALLREGS;
extern regm_t BYTEREGS;
#define ALLREGS_INIT            (mR0|mR1|mR2|mR3|mR4|mR5|mR6|mR7|mR8|mR9|mR10|mR11|mR12)
#define ALLREGS_INIT_PIC        (mR0|mR1|mR2|mR3|mR4|mR5|mR7|mR8|mR9|mR10|mR11|mR12)
#define BYTEREGS_INIT           0
#define BYTEREGS_INIT_PIC       0

#define FLOATREGS_16 0
#define FLOATREGS2_16 0
#define DOUBLEREGS_16 0

#define XMMREGS 0

// Iop meanings

// 00 illegal to avoid accidental failure to initialize

#define ESCAPE  0x01    // marker that special information is here
                        // (Iop2 is the type of special information)
                        // (Same as DS:, but we will never generate
                        // a separate DS: opcode anyway)

// ESCAPE instruction bit masks (top 4 bits)
#define ESCAPEmask  0x00ffffff
#define ESClinnum   (1 << 28)       // line number information
#define ESCctor     (2 << 28)       // object is constructed
#define ESCdtor     (3 << 28)       // object is destructed
#define ESCmark     (4 << 28)       // mark eh stack
#define ESCrelease  (5 << 28)       // release eh stack
#define ESCoffset   (6 << 28)       // set code offset for eh
#define ESCadjesp   (7 << 28)       // adjust ESP by IEV2.Vint
#define ESCmark2    (8 << 28)       // mark eh stack
#define ESCrelease2 (9 << 28)       // release eh stack
#define ESCframeptr (10 << 28)      // replace with load of frame pointer
#define ESCdctor    (11 << 28)      // D object is constructed
#define ESCddtor    (12 << 28)      // D object is destructed
#define ESCadjfpu   (13 << 28)      // adjust fpustackused by IEV2.Vint (X86 only?)

#define ASM     0x02
#define NOP     0x03
#define PADDING 0x04    // space needed for indirect symbols (use an ESCAPE?)
// 0x05..0x0f reserved for future use

#include "arm_tables.h"

#define X(a, b, c, d, e, f) static const int OP_##a=b;
OPCODES
#undef X

#define X(a, b) static const int COND_##a=b;
CONDCODES
#undef X

struct code
{
    code* next;
    unsigned Iflags;
#define CFtarg       (1 << 0)    // this code is the target of a jump
//#define CFseg        (1 << 1)    // get segment of immediate value
#define CFoff        (1 << 2)    // get offset of immediate value
#define CFpsw        (1 << 3)    // we need the flags result after this instruction
#define CFselfrel    (1 << 4)    // if self-relative
#define CFunambig    (1 << 5)    // indicates cannot be accessed by other addressing modes
#define CFtarg2      (1 << 6)    // like CFtarg, but we can't optimize this away
#define CFvolatile   (1 << 7)    // volatile reference, do not schedule
#define CFclassinit  (1 << 8)    // class init code
#define CFoffset64   (1 << 9)    // offset is 64 bits // TODO: can this ever be set?  used in obj output code right now
#define CFcs         (1 << 10)   // need CS override
#define CFpc32       (1 << 11)   // I64: PC relative 32 bit fixup // TODO: needed for elfobj for now, needs work

#define CFimmed      (1 << 20)   // for data processing commands: immediate value vs register
#define CFswitch     (1 << 21)   // kludge for switch table fixups

    unsigned Iop;

    unsigned Iregs;
    // NOTE: for OP_PUSH and OP_POP is a mask and not 4 individual regs
    //       should I move it over to IFL1 instead?
#define IREGRnmask  0x0000000f  // Rn mask
#define IREGRdmask  0x000000f0  // Rd mask
#define IREGRsmask  0x00000f00  // Rs mask
#define IREGRmmask  0x0000f000  // Rm mask
#define IREGRnshift          0  // Rn shift
#define IREGRdshift          4  // Rd shift
#define IREGRsshift          8  // Rs shift
#define IREGRmshift         12  // Rm shift
#define IREGCCmask  0xf0000000  // CC bits
#define IREGCCshift         28  // CC shift

    unsigned Iaddrmode;
// 2 bits, addressing mode
#define IAMMode_none             0
#define IAMMode_off              1
#define IAMMode_preidx           2
#define IAMMode_postidx          3
#define IAMModeMask     0x00000003
#define IAMModeShift             0

// 2 bits, shift type: 0 == LSL, 1 == LSR, 2 == ASR, 3 == RRX/ROR
#define IAMSTypeMask    0x0000000c
#define IAMSTypeShift            2

// 1 bit, 0 == register, 1 == immediate
#define IAMSImmMask     0x00000010
#define IAMSImmShift             4

// 5 bits, shift offset || shift register
#define IAMSOffMask     0x000003e0
#define IAMSOffShift             5

// 1 bit, 1 == sub, 0 == add
#define IAMSsubMask     0x00000400
#define IAMSsubShift            10

    unsigned char IFL1,IFL2;     // FLavors of 1st, 2nd operands
    union evc IEV1;              // 1st operand, if any
#define IEVsize_t1  IEV1.Vsize_t
#define IEVdsym1    IEV1.dsp.Vsym
#define IEVsym1     IEV1.sp.Vsym
#define IEVoffset1  IEV1.sp.Voffset
#define IEVseg1     IEV1._EP.Vseg
#define IEVpointer1 IEV1._EP.Vpointer
#define IEVlsym1    IEV1.lab.Vsym
    union evc IEV2;              // 2st operand, if any
#define IEVsize_t2  IEV2.Vsize_t
#define IEVdsym2    IEV2.dsp.Vsym
#define IEVsym2     IEV2.sp.Vsym
#define IEVoffset2  IEV2.sp.Voffset
#define IEVseg2     IEV2._EP.Vseg
#define IEVpointer2 IEV2._EP.Vpointer
#define IEVlsym2    IEV2.lab.Vsym

    code() { next = NULL; Iflags = 0; Iop = 0; Iregs = 0; setCC(COND_AL); Iaddrmode = 0; IFL1 = 0; IEVsize_t1 = 0; }
    void print();               // pretty-printer

    void setRegN(unsigned reg) { Iregs = (Iregs & ~IREGRnmask) | (reg << IREGRnshift); }
    void setRegD(unsigned reg) { Iregs = (Iregs & ~IREGRdmask) | (reg << IREGRdshift); }
    void setRegS(unsigned reg) { Iregs = (Iregs & ~IREGRsmask) | (reg << IREGRsshift); }
    void setRegM(unsigned reg) { Iregs = (Iregs & ~IREGRmmask) | (reg << IREGRmshift); }

    void setCC(unsigned cc)    { Iregs = (Iregs & ~IREGCCmask) | (cc  << IREGCCshift); }

    void setAddrMode(unsigned mode)
    {
        if (mode == IAMMode_none)
            Iaddrmode = 0;
        else
            Iaddrmode = (Iaddrmode & ~IAMModeMask ) | (mode << IAMModeShift);
    }
    void setShiftType(unsigned type)  { Iaddrmode = (Iaddrmode & ~IAMSTypeMask) | (type << IAMSTypeShift); }
    void setShiftImm(unsigned type)   { Iaddrmode = (Iaddrmode & ~IAMSImmMask ) | (type << IAMSImmShift);  }
    void setShiftOffset(unsigned off) { Iaddrmode = (Iaddrmode & ~IAMSOffMask ) | (off  << IAMSOffShift);  }
    void setShiftReg(unsigned reg)    { Iaddrmode = (Iaddrmode & ~IAMSOffMask ) | (reg  << IAMSOffShift);  }
    void setShiftSubtract(bool a)     { Iaddrmode = (Iaddrmode & ~IAMSsubMask ) | (a    << IAMSsubShift);  }

    unsigned getRegN()   { return (Iregs & IREGRnmask) >> IREGRnshift; }
    unsigned getRegD()   { return (Iregs & IREGRdmask) >> IREGRdshift; }
    unsigned getRegS()   { return (Iregs & IREGRsmask) >> IREGRsshift; }
    unsigned getRegM()   { return (Iregs & IREGRmmask) >> IREGRmshift; }

    unsigned getCC()     { return (Iregs & IREGCCmask) >> IREGCCshift; }

    unsigned getAddrMode()      { return (Iaddrmode & IAMModeMask ) >> IAMModeShift;  }
    unsigned getShiftType()     { return (Iaddrmode & IAMSTypeMask) >> IAMSTypeShift; }
    unsigned getShiftImm()      { return (Iaddrmode & IAMSImmMask ) >> IAMSImmShift;  }
    unsigned getShiftOffset()   { return (Iaddrmode & IAMSOffMask ) >> IAMSOffShift;  }
    bool     getShiftSubtract() { return (Iaddrmode & IAMSsubMask ) >> IAMSsubShift;  }

    bool     isJumpOP() { return Iop == OP_B || OP_BL || OP_BLX || OP_BX; }
};

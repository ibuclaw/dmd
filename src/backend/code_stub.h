// opcodes
#define ASM 0
#define NOP 1
#define ESCAPE 2
    // 8 is to leave room for opcodes to be in the range 0 .. 255
    // probably better off moving them to the high byte rather than second byte
    #define ESCAPEmask   0xff
    #define ESClinnum   (0 << 8)
    #define ESCadjesp   (1 << 8)
    #define ESCadjfpu   (2 << 8)
    #define ESCdctor    (3 << 8)      // D object is constructed
    #define ESCddtor    (4 << 8)      // D object is destructed

// registers

// base pointer
#define BP    0
// stack pointer
#define SP    1
// status word
#define PSW   2
// stack
#define STACK 3

#define NUMGENREGS 2
#define NUMREGS 1

#define NOREG 3

// which register is used for PIC code?
#define PICREG BP

// masks
#define mBP    (1 << BP)
#define mPSW   (1 << PSW)
#define mSTACK (1 << STACK)

// used in several generic parts of the code still
#define mAX  0
#define mBX  0
#define mCX  0
#define mDX  0
#define mSI  0
#define mDI  0
#define mES  0
#define mR11 0
#define mSP  0
#define mXMM0 0

#define mLSW 0
#define mMSW 0

#define REX    0
#define REX_W  0
#define REX_R  0
#define REX_X  0
#define REX_B  0
#define VEX2_B1(ivex) 0
#define VEX3_B1(ivex) 0
#define VEX3_B2(ivex) 0

#define IDXREGS 0
#define XMMREGS 0

#define FLOATREGS_16 0
#define FLOATREGS2_16 0
#define DOUBLEREGS_16 0
#define BYTEREGS_INIT 0

#if TARGET_LINUX || TARGET_OSX || TARGET_FREEBSD || TARGET_OPENBSD || TARGET_SOLARIS
extern regm_t ALLREGS;
extern regm_t BYTEREGS;
#define ALLREGS_INIT            0
#define ALLREGS_INIT_PIC        0
#define BYTEREGS_INIT           0
#define BYTEREGS_INIT_PIC       0
#else
#define ALLREGS                 0
#define ALLREGS_INIT            ALLREGS
#undef BYTEREGS
#define BYTEREGS                0
#endif

struct code
{
    code* next;
    unsigned Iop;                           // opcode
    unsigned Iflags;
      // eliminate these?
      #define CFes        (1 << 0)    // generate an ES: segment override for this instr
      #define CFjmp16     (1 << 1)    // need 16 bit jump offset (long branch)
      #define CFseg       (1 << 2)    // get segment of immediate value
      #define CFoff       (1 << 3)    // get offset of immediate value
      #define CFss        (1 << 4)    // generate an SS: segment override (not with
      #define CFpsw       (1 << 5)    // we need the flags result after this instruction
      #define CFopsize    (1 << 6)    // prefix with operand size
      #define CFaddrsize  (1 << 7)    // prefix with address size
      #define CFds        (1 << 8)    // need DS override (not with es, ss, or cs )
      #define CFcs        (1 << 9)    // need CS override
      #define CFfs        (1 << 10)   // need FS override
      #define CFgs        (1 << 11)   // need GS override
      #define CFwait      (1 << 12)   // If I32 it indicates when to output a WAIT
      #define CFselfrel   (1 << 13)   // if self-relative
      #define CFoffset64  (1 << 14)   // offset is 64 bits
      #define CFpc32      (1 << 15)   // I64: PC relative 32 bit fixup
      #define CFvex       (1 << 16)   // vex prefix
      #define CFvex3      (1 << 17)   // 3 byte vex prefix
      #define CFswitch    (1 << 18)   // kludge for switch table fixups

    struct {
#pragma pack(1)
        unsigned char  op;
        unsigned short vvvv : 4;
        unsigned short    w : 1;
        unsigned short mmmm : 5;
        unsigned short    b : 1;
        unsigned short    x : 1;
        unsigned short    r : 1;
        unsigned char pfx;
#pragma pack()
    } Ivex;

    unsigned char Irm;                      // reg/mode
    unsigned char Isib;                     // SIB byte
    unsigned char Irex;                     // REX prefix

    unsigned char IFL1,IFL2;                // FLavors of 1st, 2nd operands
    union evc IEV1;                         // 1st operand, if any
      #define IEVpointer1 IEV1._EP.Vpointer
      #define IEVsym1     IEV1.sp.Vsym
      #define IEVdsym1    IEV1.dsp.Vsym
      #define IEVoffset1  IEV1.sp.Voffset
      #define IEVlsym1    IEV1.lab.Vsym
      #define IEVint1     IEV1.Vint
    union evc IEV2;                         // 2nd operand, if any
      #define IEVpointer2 IEV2._EP.Vpointer
      #define IEVsym2     IEV2.sp.Vsym
      #define IEVdsym2    IEV2.dsp.Vsym
      #define IEVoffset2  IEV2.sp.Voffset
      #define IEVlsym2    IEV2.lab.Vsym
      #define IEVint2     IEV2.Vint
      #define IEVllong2   IEV2.Vllong

    void setReg(unsigned) {}

    bool isJumpOP() { return false; }

    void print() {}
};


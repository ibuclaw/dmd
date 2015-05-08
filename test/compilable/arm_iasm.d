// PERMUTE_ARGS:

// Purely useful as a parse and emit test
version = DONE;
version = BASIC;
version = NEW;

version(ARM)
{

version(BASIC)
{

void test_cc()
{
    asm
    { // }{
        naked;

        MOVAL R0, R1; // aka mov r0, r1
        MOVCC R0, R1;
        MOVLO R0, R1; // aka movcc r0, r1
        MOVCS R0, R1;
        MOVHS R0, R1; // aka movcs r0, r1
        MOVEQ R0, R1;
        MOVGE R0, R1;
        MOVGT R0, R1;
        MOVHI R0, R1;
        MOVLE R0, R1;
        MOVLS R0, R1;
        MOVLT R0, R1;
        MOVMI R0, R1;
        MOVNE R0, R1;
        MOVPL R0, R1;
        MOVVC R0, R1;
        MOVVS R0, R1;
        //MOVNV R0, R1; // NV isn't actually a useable prefix. :)
    }
}

void test_reg()
{
    asm
    { // }{
        naked;

        MOV R0, R1;
        MOV R1, R2;
        MOV R2, R3;
        MOV R3, R4;
        MOV R4, R5;
        MOV R5, R6;
        MOV R6, R7;
        MOV R7, R8;
        MOV R8, R9;
        MOV R9, R10;
        MOV R10, R11;
        MOV R11, R12;
        MOV R13, R14;
        MOV R14, R15;
        MOV FP, IP;
        MOV IP, LR;
        MOV LR, PC;
        MOV PC, SP;
    }
}

void test_shift()
{
    asm
    { // }{
        naked;

        // every pair of lines results in the same instruction encoding
        // the second of which is the cannonical form

        MOV R0, R1, ASR #2;
        ASR R0, R1, #2;

        MOV R0, R1, LSL #2;
        LSL R0, R1, #2;

        MOV R0, R1, LSR #2;
        LSR R0, R1, #2;

        MOV R0, R1, ROR #2;
        ROR R0, R1, #2;

        MOV R0, R1, ASR R2;
        ASR R0, R1, R2;

        MOV R0, R1, LSL R2;
        LSL R0, R1, R2;

        MOV R0, R1, LSR R2;
        LSR R0, R1, R2;

        MOV R0, R1, ROR R2;
        ROR R0, R1, R2;

        MOV R0, R1, RRX;
        RRX R0, R1;
    }
}

void test_reglist()
{
    asm
    { // }{
        naked;

        STM R0, { R0 };
        STM R0, { R1 };
        STM R0, { R2 };
        STM R0, { R3 };
        STM R0, { R4 };
        STM R0, { R5 };
        STM R0, { R6 };
        STM R0, { R7 };
        STM R0, { R8 };
        STM R0, { R9 };
        STM R0, { R10 };
        STM R0, { R11 };
        STM R0, { R12 };
        STM R0, { R13 };
        STM R0, { R14 };
        STM R0, { R15 };

        STM R0, { R0-R1 };
        STM R0, { R1-R2 };
        STM R0, { R2-R3 };
        STM R0, { R1-R4 };
        STM R0, { R1-R5 };
        STM R0, { R2-R4 };
        STM R0, { R2-R9 };
        STM R0, { R0,R1-R4 };
        STM R0, { R1-R4,R5 };
        STM R0, { R1-R4,R6 };
        STM R0, { R1-R4,R6-R7 };
        STM R0, { R1-R4,R6-R7,R10 };
    }
}

void test_imm12()
{
    // need to write tests for imm12 expansions
}

} // BASIC

void foo()
{

version(DONE)
{
    asm
    {
        naked;

        // A8.6.1 ADC (immediate)
        // ADC{S}<c><q> {<Rd>,} <Rn>, #<const>

        ADC R0, #1;
        ADC R0, R1, #2;
        ADCS R0, #1;
        ADCS R0, R1, #2;

        // A8.6.2 ADC (register)
        // ADC{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        ADC R0, R1;
        ADC R0, R1, LSL #2;
        ADC R0, R1, R2;
        ADC R0, R1, R2, LSL #3;
        ADCS R0, R1;
        ADCS R0, R1, LSL #2;
        ADCS R0, R1, R2;
        ADCS R0, R1, R2, LSL #3;

        // A8.6.3 ADC (register-shifted register)
        // ADC{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        ADC R0, R1, LSL R2;
        ADC R0, R1, R2, LSL R3;
        ADCS R0, R1, LSL R2;
        ADCS R0, R1, R2, LSL R3;

        // A8.6.4 ADD (immediate, Thumb)

        // A8.6.5 ADD (immediate, ARM)
        // ADD{S}<c><q> {<Rd>,} <Rn>, #<const>

        ADD R0, #1;
        ADD R0, R1, #2;
        ADDS R0, #1;
        ADDS R0, R1, #2;

        // A8.6.6 ADD (register)
        // ADD{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        ADD R0, R1;
        ADD R0, R1, LSL #2;
        ADD R0, R1, R2;
        ADD R0, R1, R2, LSL #3;
        ADDS R0, R1;
        ADDS R0, R1, LSL #2;
        ADDS R0, R1, R2;
        ADDS R0, R1, R2, LSL #3;
       
        // A8.6.7 ADD (register-shifted register)
        // ADD{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        ADD R0, R1, LSL R2;
        ADD R0, R1, R2, LSL R3;
        ADDS R0, R1, LSL R2;
        ADDS R0, R1, R2, LSL R3;

        // A8.6.8 ADD (SP plus immediate)
        // ADD{S}<c><q> {<Rd>,} SP, #<const>

        ADD SP, #1;
        ADD R0, SP, #2;
        ADDS SP, #1;
        ADDS R0, SP, #2;

        // A8.6.9 ADD (SP plus register)
        // ADD{S}<c><q> {<Rd>,} SP, <Rm>{, <shift>}

        ADD SP, R1;
        ADD SP, R1, LSL #2;
        ADD R0, SP, R2;
        ADD R0, SP, R2, LSL #3;
        ADDS SP, R1;
        ADDS SP, R1, LSL #2;
        ADDS R0, SP, R2;
        ADDS R0, SP, R2, LSL #3;

        // A8.6.10 ADR
        // ADR<c><q> <Rd>, <label>
        // ADD<c><q> <Rd>, PC, #<const>
        // SUB<c><q> <Rd>, PC, #<const>
ADR_before:
        ADR R0, ADR_after;
        ADR R0, ADR_before;
        ADD R0, PC, #2;
        SUB R0, PC, #0; // noted as special
        SUB R0, PC, #2;
ADR_after:

        // A8.6.11 AND (immediate)
        // AND{S}<c><q> {<Rd>,} <Rn>, #<const>

        AND R0, #1;
        AND R0, R1, #2;
        ANDS R0, #1;
        ANDS R0, R1, #2;

        // A8.6.12 AND (register)
        // AND{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        AND R0, R1;
        AND R0, R1, LSL #2;
        AND R0, R1, R2;
        AND R0, R1, R2, LSL #3;
        ANDS R0, R1;
        ANDS R0, R1, LSL #2;
        ANDS R0, R1, R2;
        ANDS R0, R1, R2, LSL #3;

        // A8.6.13 AND (register-shifted register)
        // AND{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        AND R0, R1, LSL R2;
        AND R0, R1, R2, LSL R3;
        ANDS R0, R1, LSL R2;
        ANDS R0, R1, R2, LSL R3;

        // A8.6.14 ASR (immediate)
        // ASR{S}<c><q> {<Rd>,} <Rm>, #<imm>

        ASR R0, #1;
        ASR R0, R1, #2;
        ASRS R0, #1;
        ASRS R0, R1, #2;

        // A8.6.15 ASR (register)
        // ASR{S}<c><q> {<Rd>,} <Rn>, <Rm>

        ASR R0, R1;
        ASR R0, R1, R2;
        ASRS R0, R1;
        ASRS R0, R1, R2;

        // A8.6.16 B
        // B<c><q> <label>

B_before:
        B B_after;
        B B_before;
B_after:
        // A8.6.17 BFC
        // BFC<c><q> <Rd>, #<lsb>, #<width>

        BFC R0, #1, #2;

        // A8.6.18 BFI
        // BFI<c><q> <Rd>, <Rn>, #<lsb>, #<width>

        BFI R0, R1, #2, #3;

        // A8.6.19 BIC (immediate)
        // BIC{S}<c><q> {<Rd>,} <Rn>, #<const>

        BIC R0, #1;
        BIC R0, R1, #2;
        BICS R0, #1;
        BICS R0, R1, #2;

        // A8.6.20 BIC (register)
        // BIC{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        BIC R0, R1;
        BIC R0, R1, LSL #2;
        BIC R0, R1, R2;
        BIC R0, R1, R2, LSL #3;
        BICS R0, R1;
        BICS R0, R1, LSL #2;
        BICS R0, R1, R2;
        BICS R0, R1, R2, LSL #3;

        // A8.6.21 BIC (register-shifted register)
        // BIC{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        BIC R0, R1, LSL R2;
        BIC R0, R1, R2, LSL R3;
        BICS R0, R1, LSL R2;
        BICS R0, R1, R2, LSL R3;

        // A8.6.22 BKPT
        // BKPT<q> #<imm>

        BKPT #0;

        // A8.6.23 BL, BLX (immediate)
        // BL{X}<c><q> <label>
        // NOTE: X means state change between arm and thumb, but only supporting arm for now

BL_before:
        BL BL_after;
        BL BL_before;
        BLX BL_after;
        BLX BL_before;
BL_after:

        // A8.6.24 BLX (register)
        // BLX<c><q> <Rm>

        BLX R0;

        // A8.6.25 BX
        // BX<c><q> <Rm>

        BX R0;

        // A8.6.26 BXJ (Jazelle)
        // BXJ<c><q> <Rm>

        // A8.6.27 CBNZ, CBZ (Thumb)
        // CB{N}Z<q> <Rn>, <label>

        // A8.6.31 CLZ
        // CLZ<c><q> <Rd>, <Rm>

        CLZ R0, R1;

        // A8.6.32 CMN (immediate)
        // CMN<c><q> <Rn>, #<const>

        CMN R0, #1;

        // A8.6.33 CMN (register)
        // CMN<c><q> <Rn>, <Rm> {,<shift>}

        CMN R0, R1;
        CMN R0, R1, LSL #2;

        // A8.6.34 CMN (register-shifted register)
        // CMN<c><q> <Rn>, <Rm>, <type> <Rs>

        CMN R0, R1, LSL R2;

        // A8.6.35 CMP (immediate)
        // CMP<c><q> <Rn>, #<const>

        CMP R0, #1;

        // A8.6.36 CMP (register)
        // CMP<c><q> <Rn>, <Rm> {,<shift>}

        CMP R0, R1;
        CMP R0, R1, LSL #2;

        // A8.6.37 CMP (register-shifted register)
        // CMP<c><q> <Rn>, <Rm>, <type> <Rs>

        CMP R0, R1, LSL R2;

        // A8.6.40 DBG
        // DBG<c><q> #<option>

        DBG #1;

        // A8.6.44 EOR (immediate)
        // EOR{S}<c><q> {<Rd>,} <Rn>, #<const>

        EOR R0, #1;
        EOR R0, R1, #2;
        EORS R0, #1;
        EORS R0, R1, #2;

        // A8.6.45 EOR (register)
        // EOR{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        EOR R0, R1;
        EOR R0, R1, LSL #2;
        EOR R0, R1, R2;
        EOR R0, R1, R2, LSL #3;
        EORS R0, R1;
        EORS R0, R1, LSL #2;
        EORS R0, R1, R2;
        EORS R0, R1, R2, LSL #3;

        // A8.6.46 EOR (register-shifted register)
        // EOR{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        EOR R0, R1, LSL R2;
        EOR R0, R1, R2, LSL R3;
        EORS R0, R1, LSL R2;
        EORS R0, R1, R2, LSL R3;

        // ERET

        ERET;

        // HVC #<imm>

        HVC #1;

        // A8.6.53 LDM / LDMIA / LDMFD
        // LDM<c><q> <Rn>{!}, <registers>

        LDM R0, { R1 };
        LDM R0, { R1, R2 };
        LDM R0!, { R1 };
        LDM R0!, { R1, R2 };
        LDMIA R0, { R1 };
        LDMFD R0, { R1 };

        // A8.6.54 LDMDA / LDMFA
        // LDMDA<c><q> <Rn>{!}, <registers>

        LDMDA R0, { R1 };
        LDMDA R0!, { R1 };
        LDMFA R0, { R1 };

        // A8.6.55 LDMDB / LDMEA
        // LDMDB<c><q> <Rn>{!}, <registers>

        LDMDB R0, { R1 };
        LDMDB R0!, { R1 };
        LDMEA R0, { R1 };

        // A8.6.56 LDMIB / LDMED
        // LDMIB<c><q> <Rn>{!}, <registers>

        LDMIB R0, { R1 };
        LDMIB R0!, { R1 };
        LDMED R0, { R1 };

        // A8.6.57 LDR (immediate, Thumb)

        // A8.6.58 LDR (immediate, ARM)
        // LDR<c><q> <Rt>, [<Rn> {, #+/-<imm>}]              Offset: index==TRUE, wback==FALSE
        // LDR<c><q> <Rt>, [<Rn>, #+/-<imm>]!                Pre-indexed: index==TRUE, wback==TRUE
        // LDR<c><q> <Rt>, [<Rn>], #+/-<imm>                 Post-indexed: index==FALSE, wback==TRUE

        LDR R0, [R1];
        LDR R0, [R1, #2];
        LDR R0, [R1, #2]!;
        LDR R0, [R1], #2;

        // A8.6.59 LDR (literal)
        // LDR<c><q> <Rt>, <label>                           Normal form
        // LDR<c><q> <Rt>, [PC, #+/-<imm>]                   Alternative form

LDR_before:
        //LDR R0, LDR_after;
        //LDR R0, LDR_before;
        LDR R0, [PC, #2];
LDR_after:

        // A8.6.60 LDR (register)
        // LDR<c><q> <Rt>, [<Rn>, +/-<Rm>{, <shift>}]        Offset: index==TRUE, wback==FALSE
        // LDR<c><q> <Rt>, [<Rn>, +/-<Rm>{, <shift>}]!       Pre-indexed: index==TRUE, wback==TRUE
        // LDR<c><q> <Rt>, [<Rn>], +/-<Rm>{, <shift>}        Post-indexed: index==FALSE, wback==TRUE

        LDR R0, [R1, R2];
        LDR R0, [R1, R2, LSL #3];
        LDR R0, [R1, R2]!;
        LDR R0, [R1, R2, LSL #3]!;
        LDR R0, [R1], R2;
        LDR R0, [R1], R2, LSL #3;

        // A8.6.61 LDRB (immediate, Thumb)

        // A8.6.62 LDRB (immediate, ARM)
        // LDRB<c><q> <Rt>, [<Rn> {, #+/-<imm>}]             Offset: index==TRUE, wback==FALSE
        // LDRB<c><q> <Rt>, [<Rn>, #+/-<imm>]!               Pre-indexed: index==TRUE, wback==TRUE
        // LDRB<c><q> <Rt>, [<Rn>], #+/-<imm>                Post-indexed: index==FALSE, wback==TRUE

        LDRB R0, [R1];
        LDRB R0, [R1, #2];
        LDRB R0, [R1, #2]!;
        LDRB R0, [R1], #2;

        // A8.6.63 LDRB (literal)
        // LDRB<c><q> <Rt>, <label>                          Normal form
        // LDRB<c><q> <Rt>, [PC, #+/-<imm>]                  Alternative form

LDRB_before:
        //LDRB R0, LDRB_after;
        //LDRB R0, LDRB_before;
        LDRB R0, [PC, #2];
LDRB_after:

        // A8.6.64 LDRB (register)
        // LDRB<c><q> <Rt>, [<Rn>, +/-<Rm>{, <shift>}]       Offset: index==TRUE, wback==FALSE
        // LDRB<c><q> <Rt>, [<Rn>, +/-<Rm>{, <shift>}]!      Pre-indexed: index==TRUE, wback==TRUE
        // LDRB<c><q> <Rt>, [<Rn>], +/-<Rm>{, <shift>}       Post-indexed: index==FALSE, wback==TRUE

        LDRB R0, [R1, R2];
        LDRB R0, [R1, R2, LSL #3];
        LDRB R0, [R1, R2]!;
        LDRB R0, [R1, R2, LSL #3]!;
        LDRB R0, [R1], R2;
        LDRB R0, [R1], R2, LSL #3;

        // A8.6.65 LDRBT
        // LDRBT<c><q> <Rt>, [<Rn> {, #<imm>}]               Offset: Thumb only
        // LDRBT<c><q> <Rt>, [<Rn>] {, #+/-<imm>}            Post-indexed: ARM only
        // LDRBT<c><q> <Rt>, [<Rn>], +/-<Rm> {, <shift>}     Post-indexed: ARM only

        LDRBT R0, [R1];
        LDRBT R0, [R1], #-2;
        LDRBT R0, [R1], #2;
        LDRBT R0, [R1], R2;
        LDRBT R0, [R1], -R2, LSL #3;
        LDRBT R0, [R1], R2, LSL #3;

        // A8.6.66 LDRD (immediate)
        // LDRD<c><q> <Rt>, <Rt2>, [<Rn> {, #+/-<imm>}]      Offset: index==TRUE, wback==FALSE
        // LDRD<c><q> <Rt>, <Rt2>, [<Rn>, #+/-<imm>]!        Pre-indexed: index==TRUE, wback==TRUE
        // LDRD<c><q> <Rt>, <Rt2>, [<Rn>], #+/-<imm>         Post-indexed: index==FALSE, wback==TRUE

        LDRD R0, R1, [R2];
        LDRD R0, R1, [R2, #3];
        LDRD R0, R1, [R2, #3]!;
        LDRD R0, R1, [R2], #3;

        // A8.6.67 LDRD (literal)
        // LDRD<c><q> <Rt>, <Rt2>, <label>                   Normal form
        // LDRD<c><q> <Rt>, <Rt2>, [PC, #+/-<imm>]           Alternative form

LDRD_before:
        //LDRD R0, R1, LDRD_after;
        //LDRD R0, R1, LDRD_before;
        LDRD R0, R1, [PC, #3];
LDRD_after:

        // A8.6.68 LDRD (register)
        // LDRD<c><q> <Rt>, <Rt2>, [<Rn>, +/-<Rm>]           Offset: index==TRUE, wback==FALSE
        // LDRD<c><q> <Rt>, <Rt2>, [<Rn>, +/-<Rm>]!          Pre-indexed: index==TRUE, wback==TRUE
        // LDRD<c><q> <Rt>, <Rt2>, [<Rn>], +/-<Rm>           Post-indexed: index==FALSE, wback==TRUE

        LDRD R0, R1, [R2, R3];
        LDRD R0, R1, [R2, R3]!;
        LDRD R0, R1, [R2], R3;

        // A8.6.69 LDREX
        // LDREX<c><q> <Rt>, [<Rn> {,#<imm>}]

        LDREX R0, [R1];
        // LDREX R0, [R1, #2]; // thumb only

        // A8.6.70 LDREXB
        // LDREXB<c><q> <Rt>, [<Rn>]

        LDREXB R0, [R1];

        // A8.6.71 LDREXD
        // LDREXD<c><q> <Rt>, <Rt2>, [<Rn>]

        LDREXD R0, R1, [R2];

        // A8.6.72 LDREXH
        // LDREXH<c><q> <Rt>, [<Rn>]

        LDREXH R0, [R1];

        // A8.6.73 LDRH (immediate, Thumb)

        // A8.6.74 LDRH (immediate, ARM)
        // LDRH<c><q> <Rt>, [<Rn> {, #+/-<imm>}]             Offset: index==TRUE, wback==FALSE
        // LDRH<c><q> <Rt>, [<Rn>, #+/-<imm>]!               Pre-indexed: index==TRUE, wback==TRUE
        // LDRH<c><q> <Rt>, [<Rn>], #+/-<imm>                Post-indexed: index==FALSE, wback==TRUE

        LDRH R0, [R1];
        LDRH R0, [R1, #2];
        LDRH R0, [R1, #2]!;
        LDRH R0, [R1], #2;

        // A8.6.75 LDRH (literal)
        // LDRH<c><q> <Rt>, <label>                          Normal form
        // LDRH<c><q> <Rt>, [PC, #+/-<imm>]                  Alternative form

LDRH_before:
        // TODO: LDRH R0, LDRH_after;
        // TODO: LDRH R0, LDRH_before;
        LDRH R0, [PC, #2];
LDRH_after:

        // A8.6.76 LDRH (register)
        // LDRH<c><q> <Rt>, [<Rn>, <Rm>{, LSL #<imm>}]       Offset: index==TRUE, wback==FALSE (Thumb)
        // LDRH<c><q> <Rt>, [<Rn>, +/-<Rm>]                  Offset: index==TRUE, wback==FALSE
        // LDRH<c><q> <Rt>, [<Rn>, +/-<Rm>]!                 Pre-indexed: index==TRUE, wback==TRUE
        // LDRH<c><q> <Rt>, [<Rn>], +/-<Rm>                  Post-indexed: index==FALSE, wback==TRUE

        LDRH R0, [R1, R2];
        LDRH R0, [R1, R2]!;
        LDRH R0, [R1], R2;

        // A8.6.77 LDRHT
        // LDRHT<c><q> <Rt>, [<Rn> {, #<imm>}]               Offset: Thumb only
        // LDRHT<c><q> <Rt>, [<Rn>] {, #+/-<imm>}            Post-indexed: ARM only
        // LDRHT<c><q> <Rt>, [<Rn>], +/-<Rm>                 Post-indexed: ARM only

        LDRHT R0, [R1];
        LDRHT R0, [R1], #2;
        LDRHT R0, [R1], R2;

        // A8.6.78 LDRSB (immediate)
        // LDRSB<c><q> <Rt>, [<Rn> {, #+/-<imm>}]            Offset: index==TRUE, wback==FALSE
        // LDRSB<c><q> <Rt>, [<Rn>, #+/-<imm>]!              Pre-indexed: index==TRUE, wback==TRUE
        // LDRSB<c><q> <Rt>, [<Rn>], #+/-<imm>               Post-indexed: index==FALSE, wback==TRUE

        LDRSB R0, [R1];
        LDRSB R0, [R1, #2];
        LDRSB R0, [R1, #2]!;
        LDRSB R0, [R1], #2;

        // A8.6.79 LDRSB (literal)
        // LDRSB<c><q> <Rt>, <label>                         Normal form
        // LDRSB<c><q> <Rt>, [PC, #+/-<imm>]                 Alternative form

LDRSB_before:
        //LDRSB R0, LDRSB_after;
        //LDRSB R0, LDRSB_before;
        LDRSB R0, [PC, #2];
LDRSB_after:

        // A8.6.80 LDRSB (register)
        // LDRSB<c><q> <Rt>, [<Rn>, <Rm>{, LSL #<imm>}]      Offset: index==TRUE, wback==FALSE (Thumb)
        // LDRSB<c><q> <Rt>, [<Rn>, +/-<Rm>]                 Offset: index==TRUE, wback==FALSE
        // LDRSB<c><q> <Rt>, [<Rn>, +/-<Rm>]!                Pre-indexed: index==TRUE, wback==TRUE
        // LDRSB<c><q> <Rt>, [<Rn>], +/-<Rm>                 Post-indexed: index==FALSE, wback==TRUE

        LDRSB R0, [R1, R2];
        LDRSB R0, [R1, R2]!;
        LDRSB R0, [R1], R2;

        // A8.6.81 LDRSBT
        // LDRSBT<c><q> <Rt>, [<Rn> {, #<imm>}]              Offset: Thumb only
        // LDRSBT<c><q> <Rt>, [<Rn>] {, #+/-<imm>}           Post-indexed: ARM only
        // LDRSBT<c><q> <Rt>, [<Rn>], +/-<Rm>                Post-indexed: ARM only

        LDRSBT R0, [R1];
        LDRSBT R0, [R1], #2;
        LDRSBT R0, [R1], R2;

        // A8.6.82 LDRSH (immediate)
        // LDRSH<c><q> <Rt>, [<Rn> {, #+/-<imm>}]            Offset: index==TRUE, wback==FALSE
        // LDRSH<c><q> <Rt>, [<Rn>, #+/-<imm>]!              Pre-indexed: index==TRUE, wback==TRUE
        // LDRSH<c><q> <Rt>, [<Rn>], #+/-<imm>               Post-indexed: index==FALSE, wback==TRUE

        LDRSH R0, [R1];
        LDRSH R0, [R1, #2];
        LDRSH R0, [R1, #2]!;
        LDRSH R0, [R1], #2;

        // A8.6.83 LDRSH (literal)
        // LDRSH<c><q> <Rt>, <label>                         Normal form
        // LDRSH<c><q> <Rt>, [PC, #+/-<imm>]                 Alternative form

LDRSH_before:
        //LDRSH R0, LDRSH_after;
        //LDRSH R0, LDRSH_before;
        LDRSH R0, [PC, #2];
LDRSH_after:

        // A8.6.84 LDRSH (register)
        // LDRSH<c><q> <Rt>, [<Rn>, <Rm>{, LSL #<imm>}]      Offset: index==TRUE, wback==FALSE (Thumb)
        // LDRSH<c><q> <Rt>, [<Rn>, +/-<Rm>]                 Offset: index==TRUE, wback==FALSE
        // LDRSH<c><q> <Rt>, [<Rn>, +/-<Rm>]!                Pre-indexed: index==TRUE, wback==TRUE
        // LDRSH<c><q> <Rt>, [<Rn>], +/-<Rm>                 Post-indexed: index==FALSE, wback==TRUE

        LDRSH R0, [R1, R2];
        LDRSH R0, [R1, R2]!;
        LDRSH R0, [R1], R2;

        // A8.6.85 LDRSHT
        // LDRSHT<c><q> <Rt>, [<Rn> {, #<imm>}]              Offset: Thumb only
        // LDRSHT<c><q> <Rt>, [<Rn>] {, #+/-<imm>}           Post-indexed: ARM only
        // LDRSHT<c><q> <Rt>, [<Rn>], +/-<Rm>                Post-indexed: ARM only

        LDRSHT R0, [R1];
        // LDRSHT R0, [R1, #2]; // thumb only
        LDRSHT R0, [R1], #2;
        LDRSHT R0, [R1], R2;

        // A8.6.86 LDRT
        // LDRT<c><q> <Rt>, [<Rn> {, #<imm>}]                Offset: Thumb only
        // LDRT<c><q> <Rt>, [<Rn>] {, #+/-<imm>}             Post-indexed: ARM only
        // LDRT<c><q> <Rt>, [<Rn>], +/-<Rm> {, <shift>}      Post-indexed: ARM only

        LDRT R0, [R1];
        LDRT R0, [R1], #2;
        LDRT R0, [R1], R2;
        LDRT R0, [R1], R2, LSL #3;

        // A8.6.88 LSL (immediate)
        // LSL{S}<c><q> {<Rd>,} <Rm>, #<imm5>

        LSL R0, #1;
        LSL R0, R1, #2;
        LSLS R0, #1;
        LSLS R0, R1, #2;

        // A8.6.89 LSL (register)
        // LSL{S}<c><q> {<Rd>,} <Rn>, <Rm>

        LSL R0, R1;
        LSL R0, R1, R2;
        LSLS R0, R1;
        LSLS R0, R1, R2;

        // A8.6.90 LSR (immediate)
        // LSR{S}<c><q> {<Rd>,} <Rm>, #<imm>

        LSR R0, #1;
        LSR R0, R1, #2;
        LSRS R0, #1;
        LSRS R0, R1, #2;

        // A8.6.91 LSR (register)
        // LSR{S}<c><q> {<Rd>,} <Rn>, <Rm>

        LSR R0, R1;
        LSR R0, R1, R2;
        LSRS R0, R1;
        LSRS R0, R1, R2;

        // A8.6.94 MLA
        // MLA{S}<c><q> <Rd>, <Rn>, <Rm>, <Ra>

        MLA R0, R1, R2, R3;
        MLAS R0, R1, R2, R3;

        // A8.6.95 MLS
        // MLS<c><q> <Rd>, <Rn>, <Rm>, <Ra>

        MLS R0, R1, R2, R3;

        // A8.6.96 MOV (immediate)
        // MOV{S}<c><q> <Rd>, #<const>                       All encodings permitted

        MOV R0, #1;
        MOVS R0, #1;

        // A8.6.97 MOV (register)
        // MOV{S}<c><q> <Rd>, <Rm>

        MOV R0, R1;
        MOVS R0, R1;

        // A8.6.98 MOV (shifted register)
        // alternate form of ASR, LSL, LSR, ROR, RRX

        MOV R0, R1, ASR #2;
        MOV R0, R1, LSL #2;
        MOV R0, R1, LSR #2;
        MOV R0, R1, ROR #2;
        MOV R0, R1, ASR R2;
        MOV R0, R1, LSL R2;
        MOV R0, R1, LSR R2;
        MOV R0, R1, ROR R2;
        MOV R0, R1, RRX;

        // A8.6.102 MRS
        // MRS<c><q> <Rd>, <spec_reg>

        MRS R0, APSR;
        MRS R0, CPSR;
        MRS R0, SPSR;

        // MRS (banked register)

        MRS R1, R8_usr;
        MRS R1, R13_usr; // aka SP_usr
        MRS R1, R14_usr; // aka LR_usr

        MRS R1, R8_fiq;

        MRS R1, SP_irq;
        MRS R1, LR_irq;
        MRS R1, SP_svc;
        MRS R1, LR_svc;
        MRS R1, SP_abt;
        MRS R1, LR_abt;
        MRS R1, SP_und;
        MRS R1, LR_und;
        MRS R1, SP_mon;
        MRS R1, LR_mon;
        MRS R1, ELR_hyp;
        MRS R1, SP_hyp;

        MRS R1, SPSR_irq;
        MRS R1, SPSR_svc;
        MRS R1, SPSR_abt;
        MRS R1, SPSR_und;
        MRS R1, SPSR_fiq;
        MRS R1, SPSR_mon;
        MRS R1, SPSR_hyp;

        // A8.6.103 MSR (immediate)
        // MSR<c><q> <spec_reg>, #<imm>

        MSR APSR_g, #1;      // aka CPSR_s
        MSR APSR_nzcvq, #1;  // aka CPSR_f
        MSR APSR_nzcvqg, #1; // aka CPSR_fs
        MSR CPSR_c, #1;
        MSR CPSR_x, #1;
        MSR CPSR_s, #1;
        MSR CPSR_f, #1;
        MSR CPSR_fs, #1;
        MSR CPSR_fsxc, #1;
        MSR CPSR_cxsf, #1;
        MSR SPSR_c, #1;
        MSR SPSR_x, #1;
        MSR SPSR_s, #1;
        MSR SPSR_f, #1;

        // A8.6.104 MSR (register)
        // MSR<c><q> <spec_reg>, <Rn>

        MSR APSR_g, R1;      // aka CPSR_s
        MSR APSR_nzcvq, R1;  // aka CPSR_f
        MSR APSR_nzcvqg, R1; // aka CPSR_fs
        MSR CPSR_c, R1;
        MSR CPSR_x, R1;
        MSR CPSR_s, R1;
        MSR CPSR_f, R1;
        MSR CPSR_fs, R1;
        MSR CPSR_fsxc, R1;
        MSR SPSR_c, R1;
        MSR SPSR_x, R1;
        MSR SPSR_s, R1;
        MSR SPSR_f, R1;
        MSR SPSR_cxsf, R1;

        // MSR (banked register)

        MSR R8_usr, R1;
        MSR R13_usr, R1; // aka SP_usr
        MSR R14_usr, R1; // aka LR_usr

        MSR R8_fiq, R1;

        MSR SP_irq, R1;
        MSR LR_irq, R1;
        MSR SP_svc, R1;
        MSR LR_svc, R1;
        MSR SP_abt, R1;
        MSR LR_abt, R1;
        MSR SP_und, R1;
        MSR LR_und, R1;
        MSR SP_mon, R1;
        MSR LR_mon, R1;
        MSR ELR_hyp, R1;
        MSR SP_hyp, R1;

        MSR SPSR_irq, R1;
        MSR SPSR_svc, R1;
        MSR SPSR_abt, R1;
        MSR SPSR_und, R1;
        MSR SPSR_fiq, R1;
        MSR SPSR_mon, R1;
        MSR SPSR_hyp, R1;

        // A8.6.105 MUL
        // MUL{S}<c><q> {<Rd>,} <Rn>, <Rm>

        MUL R0, R1;
        MUL R0, R1, R2;
        MULS R0, R1;
        MULS R0, R1, R2;

        // A8.6.106 MVN (immediate)
        // MVN{S}<c><q> <Rd>, #<const>

        MVN R0, #1;
        MVNS R0, #1;

        // A8.6.107 MVN (register)
        // MVN{S}<c><q> <Rd>, <Rm> {, <shift>}

        MVN R0, R1;
        MVN R0, R1, LSL #2;
        MVNS R0, R1;
        MVNS R0, R1, LSL #2;

        // A8.6.108 MVN (register-shifted register)
        // MVN{S}<c><q> <Rd>, <Rm>, <type> <Rs>

        MVN R0, R1, LSL R2;
        MVNS R0, R1, LSL R2;

        // A8.6.109 NEG
        // NEG<c><q> <Rd>, <Rm> ==> RSBS<c><q> <Rd>, <Rm>, #0

        NEG R0, R1;

        // A8.6.110 NOP
        // NOP<c><q>

        NOP;

        // A8.6.111 ORN (immediate) (Thumb)
        // ORN{S}<c><q> {<Rd>,} <Rn>, #<const>

        // A8.6.112 ORN (register) (Thumb)
        // ORN{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        // A8.6.113 ORR (immediate)
        // ORR{S}<c><q> {<Rd>,} <Rn>, #<const>

        ORR R0, #1;
        ORR R0, R1, #2;
        ORRS R0, #1;
        ORRS R0, R1, #2;

        // A8.6.114 ORR (register)
        // ORR{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        ORR R0, R1;
        ORR R0, R1, LSL #2;
        ORR R0, R1, R2;
        ORR R0, R1, R2, LSL #3;
        ORRS R0, R1;
        ORRS R0, R1, LSL #2;
        ORRS R0, R1, R2;
        ORRS R0, R1, R2, LSL #3;

        // A8.6.115 ORR (register-shifted register)
        // ORR{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        // A8.6.116 PKH
        // PKHBT<c><q> {<Rd>,} <Rn>, <Rm> {, LSL #<imm>} tbform == FALSE
        // PKHTB<c><q> {<Rd>,} <Rn>, <Rm> {, ASR #<imm>} tbform == TRUE

        PKHBT R0, R1;
        PKHBT R0, R1, LSL #2;
        PKHBT R0, R1, R2;
        PKHBT R0, R1, R2, LSL #3;
        PKHTB R0, R1;              // aka pkhbt r0, r1, r0
        PKHTB R0, R1, ASR #2;
        PKHTB R0, R1, R2;          // aka pkhbt r0, r2, r1
        PKHTB R0, R1, R2, ASR #3;
        PKHTB R0, R1, R2, ASR #32;

        ORR R0, R1, LSL R2;
        ORR R0, R1, R2, LSL R3;
        ORRS R0, R1, LSL R2;
        ORRS R0, R1, R2, LSL R3;

        // A8.6.124 QADD
        // QADD<c><q> {<Rd>,} <Rm>, <Rn>

        QADD R0, R1;
        QADD R0, R1, R2;

        // A8.6.125 QADD16
        // QADD16<c><q> {<Rd>,} <Rn>, <Rm>

        QADD16 R0, R1;
        QADD16 R0, R1, R2;

        // A8.6.126 QADD8
        // QADD8<c><q> {<Rd>,} <Rn>, <Rm>

        QADD8 R0, R1;
        QADD8 R0, R1, R2;

        // A8.6.127 QASX
        // QASX<c><q> {<Rd>,} <Rn>, <Rm>

        QASX R0, R1;
        QASX R0, R1, R2;

        // A8.6.128 QDADD
        // QDADD<c><q> {<Rd>,} <Rm>, <Rn>
        QDADD R0, R1;
        QDADD R0, R1, R2;

        // A8.6.129 QDSUB
        // QDSUB<c><q> {<Rd>,} <Rm>, <Rn>

        QDSUB R0, R1;
        QDSUB R0, R1, R2;

        // A8.6.130 QSAX
        // QSAX<c><q> {<Rd>,} <Rn>, <Rm>

        QSAX R0, R1;
        QSAX R0, R1, R2;

        // A8.6.131 QSUB
        // QSUB<c><q> {<Rd>,} <Rm>, <Rn>

        QSUB R0, R1;
        QSUB R0, R1, R2;

        // A8.6.132 QSUB16
        // QSUB16<c><q> {<Rd>,} <Rn>, <Rm>

        QSUB16 R0, R1;
        QSUB16 R0, R1, R2;

        // A8.6.133 QSUB8
        // QSUB8<c><q> {<Rd>,} <Rn>, <Rm>

        QSUB8 R0, R1;
        QSUB8 R0, R1, R2;

        // A8.6.134 RBIT
        // RBIT<c><q> <Rd>, <Rm>

        RBIT R0, R1;

        // A8.6.135 REV
        // REV<c><q> <Rd>, <Rm>

        REV R0, R1;

        // A8.6.136 REV16
        // REV16<c><q> <Rd>, <Rm>

        REV16 R0, R1;

        // A8.6.137 REVSH
        // REVSH<c><q> <Rd>, <Rm>

        REVSH R0, R1;

        // A8.6.139 ROR (immediate)
        // ROR{S}<c><q> {<Rd>,} <Rm>, #<imm>

        ROR R0, #1;
        ROR R0, R1, #2;
        RORS R0, #1;
        RORS R0, R1, #2;

        // A8.6.140 ROR (register)
        // ROR{S}<c><q> {<Rd>,} <Rn>, <Rm>

        ROR R0, R1;
        ROR R0, R1, R2;
        RORS R0, R1;
        RORS R0, R1, R2;

        // A8.6.141 RRX
        // RRX{S}<c><q> {<Rd>,} <Rm>

        RRX R0;
        RRX R0, R1;
        RRXS R0;
        RRXS R0, R1;

        // A8.6.142 RSB (immediate)
        // RSB{S}<c><q> {<Rd>,} <Rn>, #<const>

        RSB R0, #1;
        RSB R0, R1, #2;
        RSBS R0, #1;
        RSBS R0, R1, #2;

        // A8.6.143 RSB (register)
        // RSB{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        RSB R0, R1;
        RSB R0, R1, LSL #2;
        RSB R0, R1, R2;
        RSB R0, R1, R2, LSL #3;
        RSBS R0, R1;
        RSBS R0, R1, LSL #2;
        RSBS R0, R1, R2;
        RSBS R0, R1, R2, LSL #3;

        // A8.6.144 RSB (register-shifted register)
        // RSB{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        RSB R0, R1, LSL R2;
        RSB R0, R1, R2, LSL R3;
        RSBS R0, R1, LSL R2;
        RSBS R0, R1, R2, LSL R3;

        // A8.6.145 RSC (immediate)
        // RSC{S}<c><q> {<Rd>,} <Rn>, #<const>

        RSC R0, #1;
        RSC R0, R1, #2;
        RSCS R0, #1;
        RSCS R0, R1, #2;

        // A8.6.146 RSC (register)
        // RSC{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        RSC R0, R1;
        RSC R0, R1, LSL #2;
        RSC R0, R1, R2;
        RSC R0, R1, R2, LSL #3;
        RSCS R0, R1;
        RSCS R0, R1, LSL #2;
        RSCS R0, R1, R2;
        RSCS R0, R1, R2, LSL #3;

        // A8.6.147 RSC (register-shifted register)
        // RSC{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        RSC R0, R1, LSL R2;
        RSC R0, R1, R2, LSL R3;
        RSCS R0, R1, LSL R2;
        RSCS R0, R1, R2, LSL R3;

        // A8.6.148 SADD16
        // SADD16<c><q> {<Rd>,} <Rn>, <Rm>

        SADD16 R0, R1;
        SADD16 R0, R1, R2;

        // A8.6.149 SADD8
        // SADD8<c><q> {<Rd>,} <Rn>, <Rm>

        SADD8 R0, R1;
        SADD8 R0, R1, R2;

        // A8.6.150 SASX
        // SASX<c><q> {<Rd>,} <Rn>, <Rm>

        SASX R0, R1;
        SASX R0, R1, R2;

        // A8.6.151 SBC (immediate)
        // SBC{S}<c><q> {<Rd>,} <Rn>, #<const>

        SBC R0, #1;
        SBC R0, R1, #2;
        SBCS R0, #1;
        SBCS R0, R1, #2;

        // A8.6.152 SBC (register)
        // SBC{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        SBC R0, R1;
        SBC R0, R1, LSL #2;
        SBC R0, R1, R2;
        SBC R0, R1, R2, LSL #3;
        SBCS R0, R1;
        SBCS R0, R1, LSL #2;
        SBCS R0, R1, R2;
        SBCS R0, R1, R2, LSL #3;

        // A8.6.153 SBC (register-shifted register)
        // SBC{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        SBC R0, R1, LSL R2;
        SBC R0, R1, R2, LSL R3;
        SBCS R0, R1, LSL R2;
        SBCS R0, R1, R2, LSL R3;

        // A8.6.154 SBFX
        // SBFX<c><q> <Rd>, <Rn>, #<lsb>, #<width>

        SBFX R0, R1, #2, #3;

        // A8.6.155 SDIV
        // SDIV<c><q> {<Rd>,} <Rn>, <Rm>

        SDIV R0, R1;
        SDIV R0, R1, R2;

        // A8.6.156 SEL
        // SEL<c><q> {<Rd>,} <Rn>, <Rm>

        SEL R0, R1;
        SEL R0, R1, R2;

        // A8.6.158 SEV
        // SEV<c><q>

        SEV;

        // A8.6.159 SHADD16
        // SHADD16<c><q> {<Rd>,} <Rn>, <Rm>

        SHADD16 R0, R1;
        SHADD16 R0, R1, R2;

        // A8.6.160 SHADD8
        // SHADD8<c><q> {<Rd>,} <Rn>, <Rm>

        SHADD8 R0, R1;
        SHADD8 R0, R1, R2;

        // A8.6.161 SHASX
        // SHASX<c><q> {<Rd>,} <Rn>, <Rm>

        SHASX R0, R1;
        SHASX R0, R1, R2;

        // A8.6.162 SHSAX
        // SHSAX<c><q> {<Rd>,} <Rn>, <Rm>

        SHSAX R0, R1;
        SHSAX R0, R1, R2;

        // A8.6.163 SHSUB16
        // SHSUB16<c><q> {<Rd>,} <Rn>, <Rm>

        SHSUB16 R0, R1;
        SHSUB16 R0, R1, R2;

        // A8.6.164 SHSUB8
        // SHSUB8<c><q> {<Rd>,} <Rn>, <Rm>

        SHSUB8 R0, R1;
        SHSUB8 R0, R1, R2;

        // A8.6.165 SMC (previously SMI)
        // see page B6-18
        // SMC<c><q> #<imm4>

        SMC #1;

        // A8.6.166 SMLABB, SMLABT, SMLATB, SMLATT
        // SMLA<x><y><c><q> <Rd>, <Rn>, <Rm>, <Ra>
        // x = B|T, y = B|T

        SMLABB R0, R1, R2, R3;
        SMLABT R0, R1, R2, R3;
        SMLATB R0, R1, R2, R3;
        SMLATT R0, R1, R2, R3;

        // A8.6.167 SMLAD
        // SMLAD{X}<c><q> <Rd>, <Rn>, <Rm>, <Ra>

        SMLAD R0, R1, R2, R3;
        SMLADX R0, R1, R2, R3;

        // A8.6.168 SMLAL
        // SMLAL{S}<c><q> <RdLo>, <RdHi>, <Rn>, <Rm>

        SMLAL R0, R1, R2, R3;
        SMLALS R0, R1, R2, R3;

        // A8.6.169 SMLALBB, SMLALBT, SMLALTB, SMLALTT
        // SMLAL<x><y><c><q> <RdLo>, <RdHi>, <Rn>, <Rm>
        // x = B|T, y = B|T

        SMLALBB R0, R1, R2, R3;
        SMLALBT R0, R1, R2, R3;
        SMLALTB R0, R1, R2, R3;
        SMLALTT R0, R1, R2, R3;

        // A8.6.170 SMLALD
        // SMLALD{X}<c><q> <RdLo>, <RdHi>, <Rn>, <Rm>

        SMLALD R0, R1, R2, R3;
        SMLALDX R0, R1, R2, R3;

        // A8.6.171 SMLAWB, SMLAWT
        // SMLAW<y><c><q> <Rd>, <Rn>, <Rm>, <Ra>
        // y = B|T

        SMLAWB R0, R1, R2, R3;
        SMLAWT R0, R1, R2, R3;

        // A8.6.172 SMLSD
        // SMLSD{X}<c><q> <Rd>, <Rn>, <Rm>, <Ra>

        SMLSD R0, R1, R2, R3;
        SMLSDX R0, R1, R2, R3;

        // A8.6.173 SMLSLD
        // SMLSLD{X}<c><q> <RdLo>, <RdHi>, <Rn>, <Rm>

        SMLSLD R0, R1, R2, R3;
        SMLSLDX R0, R1, R2, R3;

        // A8.6.174 SMMLA
        // SMMLA{R}<c><q> <Rd>, <Rn>, <Rm>, <Ra>

        SMMLA R0, R1, R2, R3;
        SMMLAR R0, R1, R2, R3;

        // A8.6.175 SMMLS
        // SMMLS{R}<c><q> <Rd>, <Rn>, <Rm>, <Ra>

        SMMLS R0, R1, R2, R3;
        SMMLSR R0, R1, R2, R3;

        // A8.6.176 SMMUL
        // SMMUL{R}<c><q> {<Rd>,} <Rn>, <Rm>

        SMMUL R0, R1;
        SMMUL R0, R1, R2;
        SMMULR R0, R1;
        SMMULR R0, R1, R2;

        // A8.6.177 SMUAD
        // SMUAD{X}<c><q> {<Rd>,} <Rn>, <Rm>

        SMUAD R0, R1;
        SMUAD R0, R1, R2;
        SMUADX R0, R1;
        SMUADX R0, R1, R2;

        // A8.6.178 SMULBB, SMULBT, SMULTB, SMULTT
        // SMUL<x><y><c><q> {<Rd>,} <Rn>, <Rm>
        // x = B|T, y = B|T

        SMULBB R0, R1;
        SMULBB R0, R1, R2;
        SMULBT R0, R1;
        SMULBT R0, R1, R2;
        SMULTB R0, R1;
        SMULTB R0, R1, R2;
        SMULTT R0, R1;
        SMULTT R0, R1, R2;

        // A8.6.179 SMULL
        // SMULL{S}<c><q> <RdLo>, <RdHi>, <Rn>, <Rm>

        SMULL R0, R1, R2, R3;
        SMULLS R0, R1, R2, R3;

        // A8.6.180 SMULWB, SMULWT
        // SMULW<y><c><q> {<Rd>,} <Rn>, <Rm>
        // y = B|T

        SMULWB R0, R1;
        SMULWB R0, R1, R2;
        SMULWT R0, R1;
        SMULWT R0, R1, R2;

        // A8.6.181 SMUSD
        // SMUSD{X}<c><q> {<Rd>,} <Rn>, <Rm>

        SMUSD R0, R1;
        SMUSD R0, R1, R2;
        SMUSDX R0, R1;
        SMUSDX R0, R1, R2;

        // A8.6.183 SSAT
        // SSAT<c><q> <Rd>, #<imm>, <Rn> {,<shift>}

        SSAT R0, #1, R2;
        SSAT R0, #1, R2, LSL #3;

        // A8.6.184 SSAT16
        // SSAT16<c><q> <Rd>, #<imm>, <Rn>

        SSAT16 R0, #1, R2;

        // A8.6.185 SSAX
        // SSAX<c><q> {<Rd>,} <Rn>, <Rm>

        SSAX R0, R1;
        SSAX R0, R1, R2;

        // A8.6.186 SSUB16
        // SSUB16<c><q> {<Rd>,} <Rn>, <Rm>

        SSUB16 R0, R1;
        SSUB16 R0, R1, R2;

        // A8.6.187 SSUB8
        // SSUB8<c><q> {<Rd>,} <Rn>, <Rm>

        SSUB8 R0, R1;
        SSUB8 R0, R1, R2;

        // A8.6.189 STM / STMIA / STMEA
        // STM<c><q> <Rn>{!}, <registers>

        STM R0, { R1 };
        STM R0!, { R1 };
        STMIA R0, { R1 };
        STMEA R0, { R1 };

        // A8.6.190 STMDA / STMED
        // STMDA<c><q> <Rn>{!}, <registers>

        STMDA R0, { R1 };
        STMDA R0!, { R1 };
        STMED R1, { R2 };

        // A8.6.191 STMDB / STMFD
        // STMDB<c><q> <Rn>{!}, <registers>

        STMDB R0, { R1 };
        STMDB R0!, { R1 };
        STMFD R0, { R1 };

        // A8.6.192 STMIB / STMFA
        // STMIB<c><q> <Rn>{!}, <registers>

        STMIB R0, { R1 };
        STMIB R0!, { R1 };
        STMFA R0, { R1 };

        // A8.6.193 STR (immediate, Thumb)

        // A8.6.194 STR (immediate, ARM)
        // STR<c><q> <Rt>, [<Rn> {, #+/-<imm>}]              Offset: index==TRUE, wback==FALSE
        // STR<c><q> <Rt>, [<Rn>, #+/-<imm>]!                Pre-indexed: index==TRUE, wback==TRUE
        // STR<c><q> <Rt>, [<Rn>], #+/-<imm>                 Post-indexed: index==FALSE, wback==TRUE

        STR R0, [R1];
        STR R0, [R1, #2];
        STR R0, [R1, #2]!;
        STR R0, [R1], #2;

        // A8.6.195 STR (register)
        // STR<c><q> <Rt>, [<Rn>, <Rm>{, <shift>}]           Offset: index==TRUE, wback==FALSE
        // STR<c><q> <Rt>, [<Rn>, <Rm>{, <shift>}]!          Pre-indexed: index==TRUE, wback==TRUE
        // STR<c><q> <Rt>, [<Rn>], <Rm>{, <shift>}           Post-indexed: index==FALSE, wback==TRUE

        STR R0, [R1, R2];
        STR R0, [R1, R2, LSL #3];
        STR R0, [R1, R2]!;
        STR R0, [R1, R2, LSL #3]!;
        STR R0, [R1], R2;
        STR R0, [R1], R2, LSL #3;

        // A8.6.196 STRB (immediate, Thumb)

        // A8.6.197 STRB (immediate, ARM)
        // STRB<c><q> <Rt>, [<Rn> {, #+/-<imm>}]             Offset: index==TRUE, wback==FALSE
        // STRB<c><q> <Rt>, [<Rn>, #+/-<imm>]!               Pre-indexed: index==TRUE, wback==TRUE
        // STRB<c><q> <Rt>, [<Rn>], #+/-<imm>                Post-indexed: index==FALSE, wback==TRUE

        STRB R0, [R1];
        STRB R0, [R1, #2];
        STRB R0, [R1, #2]!;
        STRB R0, [R1], #2;

        // A8.6.198 STRB (register)
        // STRB<c><q> <Rt>, [<Rn>, <Rm>{, <shift>}]          Offset: index==TRUE, wback==FALSE
        // STRB<c><q> <Rt>, [<Rn>, <Rm>{, <shift>}]!         Pre-indexed: index==TRUE, wback==TRUE
        // STRB<c><q> <Rt>, [<Rn>], <Rm>{, <shift>}          Post-indexed: index==FALSE, wback==TRUE

        STRB R0, [R1, R2];
        STRB R0, [R1, R2, LSL #3];
        STRB R0, [R1, R2]!;
        STRB R0, [R1, R2, LSL #3]!;
        STRB R0, [R1], R2;
        STRB R0, [R1], R2, LSL #3;

        // A8.6.199 STRBT
        // STRBT<c><q> <Rt>, [<Rn> {, #<imm>}]               Offset: Thumb only
        // STRBT<c><q> <Rt>, [<Rn>] {, #<imm>}               Post-indexed: ARM only
        // STRBT<c><q> <Rt>, [<Rn>], +/-<Rm> {, <shift>}     Post-indexed: ARM only

        STRBT R0, [R1];
        STRBT R0, [R1], #2;
        STRBT R0, [R1], R2;
        STRBT R0, [R1], R2, LSL #3;

        // A8.6.200 STRD (immediate)
        // STRD<c><q> <Rt>, <Rt2>, [<Rn> {, #+/-<imm>}]      Offset: index==TRUE, wback==FALSE
        // STRD<c><q> <Rt>, <Rt2>, [<Rn>, #+/-<imm>]!        Pre-indexed: index==TRUE, wback==TRUE
        // STRD<c><q> <Rt>, <Rt2>, [<Rn>], #+/-<imm>         Post-indexed: index==FALSE, wback==TRUE

        STRD R0, R1, [R2];
        STRD R0, R1, [R2, #2];
        STRD R0, R1, [R2, #2]!;
        STRD R0, R1, [R2], #2;

        // A8.6.201 STRD (register)
        // STRD<c><q> <Rt>, <Rt2>, [<Rn>, +/-<Rm>]           Offset: index==TRUE, wback==FALSE
        // STRD<c><q> <Rt>, <Rt2>, [<Rn>, +/-<Rm>]!          Pre-indexed: index==TRUE, wback==TRUE
        // STRD<c><q> <Rt>, <Rt2>, [<Rn>], +/-<Rm>           Post-indexed: index==FALSE, wback==TRUE

        STRD R0, R1, [R2, R3];
        STRD R0, R1, [R2, R3]!;
        STRD R0, R1, [R2], R3;

        // A8.6.202 STREX
        // STREX<c><q> <Rd>, <Rt>, [<Rn> {,#<imm>}]

        STREX R0, R1, [R2];
        // STREX R0, R1, [R2, #3]; // thumb only

        // A8.6.203 STREXB
        // STREXB<c><q> <Rd>, <Rt>, [<Rn>]

        STREXB R0, R1, [R2];

        // A8.6.204 STREXD
        // STREXD<c><q> <Rd>, <Rt>, <Rt2>, [<Rn>]

        STREXD R0, R1, R2, [R3];

        // A8.6.205 STREXH
        // STREXH<c><q> <Rd>, <Rt>, [<Rn>]

        STREXH R0, R1, [R2];

        // A8.6.206 STRH (immediate, Thumb)

        // A8.6.207 STRH (immediate, ARM)
        // STRH<c><q> <Rt>, [<Rn> {, #+/-<imm>}]             Offset: index==TRUE, wback==FALSE
        // STRH<c><q> <Rt>, [<Rn>, #+/-<imm>]!               Pre-indexed: index==TRUE, wback==TRUE
        // STRH<c><q> <Rt>, [<Rn>], #+/-<imm>                Post-indexed: index==FALSE, wback==TRUE

        STRH R0, [R1];
        STRH R0, [R1, #2];
        STRH R0, [R1, #2]!;
        STRH R0, [R1], R2;

        // A8.6.208 STRH (register)
        // STRH<c><q> <Rt>, [<Rn>, +/-<Rm>{, LSL #<imm>}]    Offset: index==TRUE, wback==FALSE
        // STRH<c><q> <Rt>, [<Rn>, +/-<Rm>]!                 Pre-indexed: index==TRUE, wback==TRUE
        // STRH<c><q> <Rt>, [<Rn>], +/-<Rm>                  Post-indexed: index==FALSE, wback==TRUE

        STRH R0, [R1, R2];
        // thumb only: STRH R0, [R1, R2, LSL #3];
        STRH R0, [R1, R2]!;
        STRH R0, [R1], R2;

        // A8.6.209 STRHT
        // STRHT<c><q> <Rt>, [<Rn> {, #<imm>}]               Offset: Thumb only
        // STRHT<c><q> <Rt>, [<Rn>] {, #+/-<imm>}            Post-indexed: ARM only
        // STRHT<c><q> <Rt>, [<Rn>], +/-<Rm>                 Post-indexed: ARM only

        STRHT R0, [R1];
        STRHT R0, [R1], #2;
        STRHT R0, [R1], R2;

        // A8.6.210 STRT
        // STRT<c><q> <Rt>, [<Rn> {, #<imm>}]                Offset: Thumb only
        // STRT<c><q> <Rt>, [<Rn>] {, #+/-<imm>}             Post-indexed: ARM only
        // STRT<c><q> <Rt>, [<Rn>], +/-<Rm> {, <shift>}      Post-indexed: ARM only

        STRT R0, [R1];
        STRT R0, [R1], #2;
        STRT R0, [R1], R2;
        STRT R0, [R1], R2, LSL #3;

        // A8.6.211 SUB (immediate, Thumb)

        // A8.6.212 SUB (immediate, ARM)
        // SUB{S}<c><q> {<Rd>,} <Rn>, #<const>

        SUB R0, #1;
        SUB R0, R1, #2;
        SUBS R0, #1;
        SUBS R0, R1, #2;

        // A8.6.213 SUB (register)
        // SUB{S}<c><q> {<Rd>,} <Rn>, <Rm> {,<shift>}

        SUB R0, R1;
        SUB R0, R1, LSL #2;
        SUB R0, R1, R2;
        SUB R0, R1, R2, LSL #3;
        SUBS R0, R1;
        SUBS R0, R1, LSL #2;
        SUBS R0, R1, R2;
        SUBS R0, R1, R2, LSL #3;

        // A8.6.214 SUB (register-shifted register)
        // SUB{S}<c><q> {<Rd>,} <Rn>, <Rm>, <type> <Rs>

        SUB R0, R1, LSL R2;
        SUB R0, R1, R2, LSL R3;
        SUBS R0, R1, LSL R2;
        SUBS R0, R1, R2, LSL R3;

        // A8.6.215 SUB (SP minus immediate)
        // SUB{S}<c><q> {<Rd>,} SP, #<const>                 All encodings permitted
        // SUBW<c><q> {<Rd>,} SP, #<const>                   Only encoding T3 permitted

        SUB SP, #1;
        SUB R0, SP, #2;
        SUBS SP, #1;
        SUBS R0, SP, #2;

        // A8.6.216 SUB (SP minus register)
        // SUB{S}<c><q> {<Rd>,} SP, <Rm> {,<shift>}

        SUB SP, R1;
        SUB SP, R1, LSL #2;
        SUB R0, SP, R2;
        SUB R0, SP, R2, LSL #3;
        SUBS SP, R1;
        SUBS SP, R1, LSL #2;
        SUBS R0, SP, R2;
        SUBS R0, SP, R2, LSL #3;

        // A8.6.219 SWP, SWPB
        // SWP{B}<c><q> <Rt>, <Rt2>, [<Rn>]

        SWP R0, R1, [R2];
        SWPB R0, R1, [R2];

        // A8.6.220 SXTAB
        // SXTAB<c><q> {<Rd>,} <Rn>, <Rm> {, <rotation>}

        SXTAB R0, R1;
        SXTAB R0, R1, ROR #8;
        SXTAB R0, R1, R2;
        SXTAB R0, R1, R2, ROR #8;

        // A8.6.221 SXTAB16
        // SXTAB16<c><q> {<Rd>,} <Rn>, <Rm> {, <rotation>}

        SXTAB16 R0, R1;
        SXTAB16 R0, R1, ROR #8;
        SXTAB16 R0, R1, R2;
        SXTAB16 R0, R1, R2, ROR #16;

        // A8.6.222 SXTAH
        // SXTAH<c><q> {<Rd>,} <Rn>, <Rm> {, <rotation>}

        SXTAH R0, R1;
        SXTAH R0, R1, ROR #8;
        SXTAH R0, R1, R2;
        SXTAH R0, R1, R2, ROR #8;

        // A8.6.223 SXTB
        // SXTB<c><q> {<Rd>,} <Rm> {, <rotation>}

        SXTB R0;
        SXTB R0, ROR #8;
        SXTB R0, R1;
        SXTB R0, R1, ROR #24;

        // A8.6.224 SXTB16
        // SXTB16<c><q> {<Rd>,} <Rm> {, <rotation>}

        SXTB16 R0;
        SXTB16 R0, ROR #8;
        SXTB16 R0, R1;
        SXTB16 R0, R1, ROR #16;

        // A8.6.225 SXTH
        // SXTH<c><q> {<Rd>,} <Rm> {, <rotation>}

        SXTH R0;
        SXTH R0, ROR #8;
        SXTH R0, R1;
        SXTH R0, R1, ROR #8;

        // A8.6.227 TEQ (immediate)
        // TEQ<c><q> <Rn>, #<const>

        TEQ R0, #1;

        // A8.6.228 TEQ (register)
        // TEQ<c><q> <Rn>, <Rm> {,<shift>}

        TEQ R0, R1;
        TEQ R0, R1, LSL #2;

        // A8.6.229 TEQ (register-shifted register)
        // TEQ<c><q> <Rn>, <Rm>, <type> <Rs>

        TEQ R0, R1, LSL R2;

        // A8.6.230 TST (immediate)
        // TST<c><q> <Rn>, #<const>

        TST R0, #1;

        // A8.6.231 TST (register)
        // TST<c><q> <Rn>, <Rm> {,<shift>}

        TST R0, R1;
        TST R0, R1, LSL #2;

        // A8.6.232 TST (register-shifted register)
        // TST<c><q> <Rn>, <Rm>, <type> <Rs>

        TST R0, R1, LSL R2;

        // A8.6.233 UADD16
        // UADD16<c><q> {<Rd>,} <Rn>, <Rm>

        UADD16 R0, R1;
        UADD16 R0, R1, R2;

        // A8.6.234 UADD8
        // UADD8<c><q> {<Rd>,} <Rn>, <Rm>

        UADD8 R0, R1;
        UADD8 R0, R1, R2;

        // A8.6.235 UASX
        // UASX<c><q> {<Rd>,} <Rn>, <Rm>

        UASX R0, R1;
        UASX R0, R1, R2;

        // A8.6.236 UBFX
        // UBFX<c><q> <Rd>, <Rn>, #<lsb>, #<width>

        UBFX R0, R1, #2, #3;

        // UDF #imm16

        UDF #0;

        // A8.6.237 UDIV
        // UDIV<c><q> {<Rd>,} <Rn>, <Rm>

        UDIV R0, R1;
        UDIV R0, R1, R2;

        // A8.6.238 UHADD16
        // UHADD16<c><q> {<Rd>,} <Rn>, <Rm>

        UHADD16 R0, R1;
        UHADD16 R0, R1, R2;

        // A8.6.239 UHADD8
        // UHADD8<c><q> {<Rd>,} <Rn>, <Rm>

        UHADD8 R0, R1;
        UHADD8 R0, R1, R2;

        // A8.6.240 UHASX
        // UHASX<c><q> {<Rd>,} <Rn>, <Rm>

        UHASX R0, R1;
        UHASX R0, R1, R2;

        // A8.6.241 UHSAX
        // UHSAX<c><q> {<Rd>,} <Rn>, <Rm>

        UHSAX R0, R1;
        UHSAX R0, R1, R2;

        // A8.6.242 UHSUB16
        // UHSUB16<c><q> {<Rd>,} <Rn>, <Rm>

        UHSUB16 R0, R1;
        UHSUB16 R0, R1, R2;

        // A8.6.243 UHSUB8
        // UHSUB8<c><q> {<Rd>,} <Rn>, <Rm>

        UHSUB8 R0, R1;
        UHSUB8 R0, R1, R2;

        // A8.6.244 UMAAL
        // UMAAL<c><q> <RdLo>, <RdHi>, <Rn>, <Rm>

        UMAAL R0, R1, R2, R3;

        // A8.6.245 UMLAL
        // UMLAL{S}<c><q> <RdLo>, <RdHi>, <Rn>, <Rm>

        UMLAL R0, R1, R2, R3;
        UMLALS R0, R1, R2, R3;

        // A8.6.246 UMULL
        // UMULL{S}<c><q> <RdLo>, <RdHi>, <Rn>, <Rm>

        UMULL R0, R1, R2, R3;
        UMULLS R0, R1, R2, R3;

        // A8.6.247 UQADD16
        // UQADD16<c><q> {<Rd>,} <Rn>, <Rm>

        UQADD16 R0, R1;
        UQADD16 R0, R1, R2;

        // A8.6.248 UQADD8
        // UQADD8<c><q> {<Rd>,} <Rn>, <Rm>

        UQADD8 R0, R1;
        UQADD8 R0, R1, R2;

        // A8.6.249 UQASX
        // UQASX<c><q> {<Rd>,} <Rn>, <Rm>

        UQASX R0, R1;
        UQASX R0, R1, R2;

        // A8.6.250 UQSAX
        // UQSAX<c><q> {<Rd>,} <Rn>, <Rm>

        UQSAX R0, R1;
        UQSAX R0, R1, R2;

        // A8.6.251 UQSUB16
        // UQSUB16<c><q> {<Rd>,} <Rn>, <Rm>

        UQSUB16 R0, R1;
        UQSUB16 R0, R1, R2;

        // A8.6.252 UQSUB8
        // UQSUB8<c><q> {<Rd>,} <Rn>, <Rm>

        UQSUB8 R0, R1;
        UQSUB8 R0, R1, R2;

        // A8.6.253 USAD8
        // USAD8<c><q> {<Rd>,} <Rn>, <Rm>

        USAD8 R0, R1;
        USAD8 R0, R1, R2;

        // A8.6.254 USADA8
        // USADA8<c><q> <Rd>, <Rn>, <Rm>, <Ra>

        USADA8 R0, R1, R2, R3;

        // A8.6.255 USAT
        // USAT<c><q> <Rd>, #<imm>, <Rn> {,<shift>}

        USAT R0, #1, R2;
        USAT R0, #1, R2, LSL #3;

        // A8.6.256 USAT16
        // USAT16<c><q> <Rd>, #<imm>, <Rn>

        USAT16 R0, #1, R2;

        // A8.6.257 USAX
        // USAX<c><q> {<Rd>,} <Rn>, <Rm>

        USAX R0, R1;
        USAX R0, R1, R2;

        // A8.6.258 USUB16
        // USUB16<c><q> {<Rd>,} <Rn>, <Rm>

        USUB16 R0, R1;
        USUB16 R0, R1, R2;

        // A8.6.259 USUB8
        // USUB8<c><q> {<Rd>,} <Rn>, <Rm>

        USUB8 R0, R1;
        USUB8 R0, R1, R2;

        // A8.6.260 UXTAB
        // UXTAB<c><q> {<Rd>,} <Rn>, <Rm> {, <rotation>}

        UXTAB R0, R1;
        UXTAB R0, R1, ROR #8;
        UXTAB R0, R1, R2;
        UXTAB R0, R1, R2, ROR #8;

        // A8.6.261 UXTAB16
        // UXTAB16<c><q> {<Rd>,} <Rn>, <Rm> {, <rotation>}

        UXTAB16 R0, R1;
        UXTAB16 R0, R1, ROR #8;
        UXTAB16 R0, R1, R2;
        UXTAB16 R0, R1, R2, ROR #8;

        // A8.6.262 UXTAH
        // UXTAH<c><q> {<Rd>,} <Rn>, <Rm> {, <rotation>}

        UXTAH R0, R1;
        UXTAH R0, R1, ROR #8;
        UXTAH R0, R1, R2;
        UXTAH R0, R1, R2, ROR #8;

        // A8.6.263 UXTB
        // UXTB<c><q> {<Rd>,} <Rm> {, <rotation>}

        UXTB R0;
        UXTB R0, ROR #8;
        UXTB R0, R1;
        UXTB R0, R1, ROR #8;

        // A8.6.264 UXTB16
        // UXTB16<c><q> {<Rd>,} <Rm> {, <rotation>}

        UXTB16 R0;
        UXTB16 R0, ROR #8;
        UXTB16 R0, R1;
        UXTB16 R0, R1, ROR #8;

        // A8.6.265 UXTH
        // UXTH<c><q> {<Rd>,} <Rm> {, <rotation>}

        UXTH R0;
        UXTH R0, ROR #8;
        UXTH R0, R1;
        UXTH R0, R1, ROR #8;

        // A8.6.411 WFE
        // WFE<c><q>

        WFE;

        // A8.6.412 WFI
        // WFI<c><q>

        WFI;

        // A8.6.413 YIELD
        // YIELD<c><q>

        YIELD;
    }
}

version(NEW)
{
    asm
    { // }{
        naked;

        // A8.6.217 SUBS PC, LR and related instructions
        // see B6-25, no new instructions

        // A8.6.218 SVC (previously SWI)
        // SVC<c><q> #<imm>

        SVC #0;

        // A8.6.226 TBB, TBH (Thumb)

    }
}
version(none)
{
    asm
    { // }{

    }
}

version(none)
{
    asm
    { // }{
        // A8.6.28 CDP, CDP2 
        // CDP{2}<c><q> <coproc>, #<opc1>, <CRd>, <CRn>, <CRm> {,#<opc2>}

        CDP p0, #1, CR2, CR3, CR4;
        CDP p0, #1, CR2, CR3, CR4, #5;
        CDP2 p0, #1, CR2, CR3, CR4;
        CDP2 p0, #1, CR2, CR3, CR4, #5;

        // A8.6.29 CHKA (ThumbEE)

        // A8.6.30 CLREX
        // CLREX<c><q>

        CLREX;

        // A8.6.38 CPS
        // CPS<effect><q> <iflags> {, #<mode>}
        // CPS<q> #<mode>
        // effect=ID, IE.  iflags=one or more of: a, i, f.

        CPS #0;
        CPSID a;
        CPSID i;
        CPSID f;
        CPSID aif;
        CPSID a, #1
        CPSID aif, #1
        CPSIE a;
        CPSIE i;
        CPSIE f;
        CPSIE aif;
        CPSIE a, #1
        CPSIE aif, #1

        // A8.6.39 CPY (synonym for MOV (register))
        // CPY <Rd>, <Rn>

        CPY R0, R1;

        // A8.6.41 DMB
        // DMB<c><q> {<option>}

        DMB;
        DMB SY;
        DMB ST;
        DMB ISH;
        DMB ISHST;
        DMB NSH;
        DMB NSHST;
        DMB OSH;
        DMB OSHST;
        DMB SH;    // an alias for ISH
        DMB SHST;  // an alias for ISHST
        DMB UN;    // an alias for NSH
        DMB UNST;  // an alias for NSHST

        // A8.6.42 DSB
        // DSB<c><q> {<opt>}

        DSB;
        DSB SY;
        DSB ST;
        DSB ISH;
        DSB ISHST;
        DSB NSH;
        DSB NSHST;
        DSB OSH;
        DSB OSHST;
        DSB SH;    // an alias for ISH
        DSB SHST;  // an alias for ISHST
        DSB UN;    // an alias for NSH
        DSB UNST;  // an alias for NSHST

        // A8.6.43 ENTERX (Thumb[EE])

        // A8.6.47 F* (former VFP instruction mnemonics)
        // TODO: leaving testing of the deprecated forms for another time

        // A8.6.48 HB, HBL, HBLP, HBP (ThumbEE)

        // A8.6.49 ISB
        // ISB<c><q> {<opt>}

        ISB;
        ISB SY;

        // A8.6.50 IT (Thumb)

        // A8.6.51 LDC, LDC2 (immediate)
        // LDC{2}{L}<c><q> <coproc>,<CRd>,[<Rn>{,#+/-<imm>}] Offset. P = 1, W = 0.
        // LDC{2}{L}<c><q> <coproc>,<CRd>,[<Rn>,#+/-<imm>]!  Pre-indexed. P = 1, W = 1.
        // LDC{2}{L}<c><q> <coproc>,<CRd>,[<Rn>],#+/-<imm>   Post-indexed. P = 0, W = 1.
        // LDC{2}{L}<c><q> <coproc>,<CRd>,[<Rn>],<option>    Unindexed. P = 0, W = 0, U = 1.
        // option={ 0..255 }

        LDC p0, CR1, [R2];
        LDC p0, CR1, [R2, #3];
        LDCL p0, CR1, [R2];
        LDCL p0, CR1, [R2, #3];
        LDC2 p0, CR1, [R2];
        LDC2 p0, CR1, [R2, #3];
        LDC2L p0, CR1, [R2];
        LDC2L p0, CR1, [R2, #3];
        LDC p0, CR1, [R2, #3]!;
        LDCL p0, CR1, [R2, #3]!;
        LDC2 p0, CR1, [R2, #3]!;
        LDC2L p0, CR1, [R2, #3]!;
        LDC p0, CR1, [R2], #3;
        LDCL p0, CR1, [R2], #3;
        LDC2 p0, CR1, [R2], #3;
        LDC2L p0, CR1, [R2], #3;
        LDC p0, CR1, [R2], { 3 };
        LDCL p0, CR1, [R2], { 3 };
        LDC2 p0, CR1, [R2], { 3 };
        LDC2L p0, CR1, [R2], { 3 };

        // A8.6.52 LDC, LDC2 (literal)
        // LDC{2}{L}<c><q> <coproc>, <CRd>, <label>          Normal form with P = 1, W = 0
        // LDC{2}{L}<c><q> <coproc>, <CRd>, [PC,#+/-<imm>]   Alternative form with P = 1, W = 0
        // LDC{2}{L}<c><q> <coproc>, <CRd>, [PC], <option>   Unindexed form with P = 0, U = 1, W = 0
        // option={ 0..255 }

LDC_before:
        LDC p0, CR1, LDC_after;
        LDCL p0, CR1, LDC_after;
        LDC2 p0, CR1, LDC_after;
        LDC2L p0, CR1, LDC_after;
        LDC p0, CR1, LDC_before;
        LDCL p0, CR1, LDC_before;
        LDC2 p0, CR1, LDC_before;
        LDC2L p0, CR1, LDC_before;
        LDC p0, CR1, [PC, #3];
        LDCL p0, CR1, [PC, #3];
        LDC2 p0, CR1, [PC, #3];
        LDC2L p0, CR1, [PC, #3];
        LDC p0, CR1, [PC], { 3 };
        LDCL p0, CR1, [PC], { 3 };
        LDC2 p0, CR1, [PC], { 3 };
        LDC2L p0, CR1, [PC], { 3 };
LDC_after:

        // A8.6.87 LEAVEX (Thumb[EE])

        // A8.6.92 MCR, MCR2
        // MCR{2}<c><q> <coproc>, #<opc1>, <Rt>, <CRn>, <CRm>{, #<opc2>}

        MCR p0, #1, R2, CR3, CR4;
        MCR p0, #1, R2, CR3, CR4, #5;
        MCR2 p0, #1, R2, CR3, CR4;
        MCR2 p0, #1, R2, CR3, CR4, #5;

        // A8.6.93 MCRR, MCRR2
        // MCRR{2}<c><q> <coproc>, #<opc1>, <Rt>, <Rt2>, <CRm>

        MCRR p0, #1, R2, R3, CR4;
        MCRR2 p0, #1, R2, R3, CR4;

        // A8.6.96 MOV (immediate)
        // MOVW<c><q> <Rd>, #<const>                         Only encoding T3 or A2 permitted
        MOVW R0, #1;

        // A8.6.99 MOVT
        // MOVT<c><q> <Rd>, #<imm16>

        MOVT R0, #1;

        // A8.6.100 MRC, MRC2
        // MRC{2}<c><q> <coproc>, #<opc1>, <Rt>, <CRn>, <CRm>{, #<opc2>}

        MRC p0, #1, R2, CR3, CR4;
        MRC p0, #1, R2, CR3, CR4, #5;
        MRC2 p0, #1, R2, CR3, CR4;
        MRC2 p0, #1, R2, CR3, CR4, #5;

        // A8.6.101 MRRC, MRRC2
        // MRRC{2}<c><q> <coproc>, #<opc1>, <Rt>, <Rt2>, <CRm>

        MRRC p0, #1, R2, R3, CR4;
        MRRC2 p0, #1, R2, R3, CR4;

        // A8.6.117 PLD, PLDW (immediate)
        // PLD{W}<c><q> [<Rn> {, #+/-<imm>}]

        PLD [R0];
        PLD [R0, #1];
        PLDW [R0];
        PLDW [R0, #1];

        // A8.6.118 PLD (literal)
        // PLD<c><q> <label>                                 Normal form
        // PLD<c><q> [PC, #+/-<imm>]                         Alternative form

PLD_before:
        PLD PLD_after;
        PLD BLD_before;
        PLD [PC, #1];
PLD_after:

        // A8.6.119 PLD, PLDW (register)
        // PLD{W}<c><q> [<Rn>, +/-<Rm> {, <shift>}]

        PLD [R0, R1];
        PLD [R0, R1, LSL #2];
        PLDW [R0, R1];
        PLDW [R0, R1, LSL #2];

        // A8.6.120 PLI (immediate, literal)
        // PLI<c><q> [<Rn> {, #+/-<imm>}]                    Immediate form
        // PLI<c><q> <label>                                 Normal literal form
        // PLI<c><q> [PC, #+/-<imm>]                         Alternative literal form

PLI_after:
        PLI [R0];
        PLI [R0, #1];
        PLI PLI_after;
        PLI PLI_before;
        PLI [PC, #1];
PLI_before:

        // A8.6.121 PLI (register)
        // PLI<c><q> [<Rn>, +/-<Rm> {, <shift>}]

        PLI [R0, R1];
        PLI [R0, R1, LSL #2];

        // A8.6.138 RFE, B6.1.8
        // RFE{<amode>}<c><q> <Rn>{!}
        // amode=DA, DB, IA, IB

        RFEDA R0;
        RFEDA R0!;
        RFEDB R0;
        RFEDB R0!;
        RFEIA R0;
        RFEIA R0!;
        RFEIB R0;
        RFEIB R0!;

        // A8.6.157 SETEND
        // SETEND<q> <endian_specifier>

        SETEND BE;
        SETEND LE;

        // A8.6.182 SRS
        // see page B6-20
        // SRS{<amode>}<c><q> SP{!}, #<mode>
        // amode=DA, DB, IA, IB

        SRSDA SP, #1;
        SRSDA SP!, #1;
        SRSDB SP, #1;
        SRSDB SP!, #1;
        SRSIA SP, #1;
        SRSIA SP!, #1;
        SRSIB SP, #1;
        SRSIB SP!, #1;

        // A8.6.188 STC, STC2
        // STC{2}{L}<c><q> <coproc>,<CRd>,[<Rn>{,#+/-<imm>}] Offset. P = 1, W = 0.
        // STC{2}{L}<c><q> <coproc>,<CRd>,[<Rn>,#+/-<imm>]!  Pre-indexed. P = 1, W = 1.
        // STC{2}{L}<c><q> <coproc>,<CRd>,[<Rn>],#+/-<imm>   Post-indexed. P = 0, W = 1.
        // STC{2}{L}<c><q> <coproc>,<CRd>,[<Rn>],<option>    Unindexed. P = 0, W = 0, U = 1

        STC p0, CR1, [R2];
        STC p0, CR1, [R2, #3];
        STC p0, CR1, [R2, #3]!;
        STC p0, CR1, [R2], #3;
        STC p0, CR1, [R2], { 3 };
        STCL p0, CR1, [R2];
        STCL p0, CR1, [R2, #3];
        STCL p0, CR1, [R2, #3]!;
        STCL p0, CR1, [R2], #3;
        STCL p0, CR1, [R2], { 3 };
        STC2 p0, CR1, [R2];
        STC2 p0, CR1, [R2, #3];
        STC2 p0, CR1, [R2, #3]!;
        STC2 p0, CR1, [R2], #3;
        STC2 p0, CR1, [R2], { 3 };
        STC2L p0, CR1, [R2];
        STC2L p0, CR1, [R2, #3];
        STC2L p0, CR1, [R2, #3]!;
        STC2L p0, CR1, [R2], #3;
        STC2L p0, CR1, [R2], { 3 };

        // A8.6.266 VABA, VABAL
        // VABA<c><q>.<dt> <Qd>, <Qn>, <Qm>                  Encoding T1 / A1, Q = 1
        // VABA<c><q>.<dt> <Dd>, <Dn>, <Dm>                  Encoding T1 / A1, Q = 0
        // VABAL<c><q>.<dt> <Qd>, <Dn>, <Dm>                 Encoding T2 / A2
        // dt = {S,U}{8,16,32}

        VABA.S8  Q0, Q1, Q2;
        VABA.S16 Q0, Q1, Q2;
        VABA.S32 Q0, Q1, Q2;
        VABA.U8  Q0, Q1, Q2;
        VABA.U16 Q0, Q1, Q2;
        VABA.U32 Q0, Q1, Q2;
        VABA.S8  D0, D1, D2;
        VABA.S16 D0, D1, D2;
        VABA.S32 D0, D1, D2;
        VABA.U8  D0, D1, D2;
        VABA.U16 D0, D1, D2;
        VABA.U32 D0, D1, D2;
        VABAL.S8  Q0, D1, D2;
        VABAL.S16 Q0, D1, D2;
        VABAL.S32 Q0, D1, D2;
        VABAL.U8  Q0, D1, D2;
        VABAL.U16 Q0, D1, D2;
        VABAL.U32 Q0, D1, D2;

        // A8.6.267 VABD, VABDL (integer)
        // VABD<c><q>.<dt> <Qd>, <Qn>, <Qm>                  Encoding T1 / A1, Q = 1
        // VABD<c><q>.<dt> <Dd>, <Dn>, <Dm>                  Encoding T1 / A1, Q = 0
        // VABDL<c><q>.<dt> <Qd>, <Dn>, <Dm>                 Encoding T2 / A2
        // dt = {S,U}{8,16,32}

        VABD.S8  Q0, Q1, Q2;
        VABD.S16 Q0, Q1, Q2;
        VABD.S32 Q0, Q1, Q2;
        VABD.U8  Q0, Q1, Q2;
        VABD.U16 Q0, Q1, Q2;
        VABD.U32 Q0, Q1, Q2;
        VABD.S8  D0, D1, D2;
        VABD.S16 D0, D1, D2;
        VABD.S32 D0, D1, D2;
        VABD.U8  D0, D1, D2;
        VABD.U16 D0, D1, D2;
        VABD.U32 D0, D1, D2;
        VABDL.S8  Q0, D1, D2;
        VABDL.S16 Q0, D1, D2;
        VABDL.S32 Q0, D1, D2;
        VABDL.U8  Q0, D1, D2;
        VABDL.U16 Q0, D1, D2;
        VABDL.U32 Q0, D1, D2;

        // A8.6.268 VABD (floating-point)
        // VABD<c><q>.F32 <Qd>, <Qn>, <Qm>                   Encoded as Q = 1, sz = 0
        // VABD<c><q>.F32 <Dd>, <Dn>, <Dm>                   Encoded as Q = 0, sz = 0

        VABD.F32 Q0, Q1, Q2;
        VABD.F32 D0, D1, D2;

        // A8.6.269 VABS
        // VABS<c><q>.<dt> <Qd>, <Qm>                        <dt> != F64
        // VABS<c><q>.<dt> <Dd>, <Dm>
        // VABS<c><q>.F32 <Sd>, <Sm>                         VFP only. Encoding T2/A2, sz = 0
        // dt={S8,S16,S32,F32,F64}

        VABS.S8  Q0, Q1;
        VABS.S16 Q0, Q1;
        VABS.S32 Q0, Q1;
        VABS.F32 Q0, Q1;
        VABS.S8  D0, D1;
        VABS.S16 D0, D1;
        VABS.S32 D0, D1;
        VABS.F32 D0, D1;
        VABS.F64 D0, D1;
        VABS.F32 S0, S1;

        // A8.6.270 VACGE, VACGT, VACLE,VACLT
        // V<op><c><q>.F32 {<Qd>,} <Qn>, <Qm>                Encoded as Q = 1
        // V<op><c><q>.F32 {<Dd>,} <Dn>, <Dm>                Encoded as Q = 0
        // op={ACGE,ACGT} // pseudo ACLE, ACGT

        VACGE.F32 Q0, Q1;
        VACGE.F32 Q0, Q1, Q2;
        VACGE.F32 D0, D1;
        VACGE.F32 D0, D1, D2;
        VACGT.F32 Q0, Q1;
        VACGT.F32 Q0, Q1, Q2;
        VACGT.F32 D0, D1;
        VACGT.F32 D0, D1, D2;
        VACLE.F32 Q0, Q1;
        VACLE.F32 Q0, Q1, Q2;
        VACLE.F32 D0, D1;
        VACLE.F32 D0, D1, D2;
        VACLT.F32 Q0, Q1;
        VACLT.F32 Q0, Q1, Q2;
        VACLT.F32 D0, D1;
        VACLT.F32 D0, D1, D2;

        // A8.6.271 VADD (integer)
        // VADD<c><q>.<dt> {<Qd>,} <Qn>, <Qm>
        // VADD<c><q>.<dt> {<Dd>,} <Dn>, <Dm>
        // dt=I{8,16,32,64}

        VADD.I8  Q0, Q1;
        VADD.I8  Q0, Q1, Q2;
        VADD.I16 Q0, Q1;
        VADD.I16 Q0, Q1, Q2;
        VADD.I32 Q0, Q1;
        VADD.I32 Q0, Q1, Q2;
        VADD.I64 Q0, Q1;
        VADD.I64 Q0, Q1, Q2;
        VADD.I8  D0, D1;
        VADD.I8  D0, D1, D2;
        VADD.I16 D0, D1;
        VADD.I16 D0, D1, D2;
        VADD.I32 D0, D1;
        VADD.I32 D0, D1, D2;
        VADD.I64 D0, D1;
        VADD.I64 D0, D1, D2;

        // A8.6.272 VADD (floating-point)
        // VADD<c><q>.F32 {<Qd>,} <Qn>, <Qm>                 Encoding T1 / A1, Q = 1, sz = 0
        // VADD<c><q>.F32 {<Dd>,} <Dn>, <Dm>                 Encoding T1 / A1, Q = 0, sz = 0
        // VADD<c><q>.F64 {<Dd>,} <Dn>, <Dm>                 Encoding T2 / A2, sz = 1
        // VADD<c><q>.F32 {<Sd>,} <Sn>, <Sm>                 Encoding T2 / A2, sz = 0

        VADD.F32 Q0, Q1;
        VADD.F32 Q0, Q1, Q2;
        VADD.F32 D0, D1;
        VADD.F32 D0, D1, D2;
        VADD.F64 D0, D1;
        VADD.F64 D0, D1, D2;
        VADD.F32 S0, S1;
        VADD.F32 S0, S1, S2;

        // A8.6.273 VADDHN
        // VADDHN<c><q>.<dt> <Dd>, <Qn>, <Qm>
        // dt=I{16,32,64}

        VADDHN.I16 D0, Q1, Q2;
        VADDHN.I32 D0, Q1, Q2;
        VADDHN.I64 D0, Q1, Q2;

        // A8.6.274 VADDL, VADDW
        // VADDL<c><q>.<dt> <Qd>, <Dn>, <Dm>                 Encoded as op = 0
        // VADDW<c><q>.<dt> {<Qd>,} <Qn>, <Dm>               Encoded as op = 1
        // dt={S,U}{8,16,32}

        VADDL.S8  Q0, D1, D2;
        VADDL.S16 Q0, D1, D2;
        VADDL.S32 Q0, D1, D2;
        VADDL.U8  Q0, D1, D2;
        VADDL.U16 Q0, D1, D2;
        VADDL.U32 Q0, D1, D2;
        VADDW.S8  Q0, Q1;
        VADDW.S8  Q0, Q1, Q2;
        VADDW.S16 Q0, Q1;
        VADDW.S16 Q0, Q1, Q2;
        VADDW.S32 Q0, Q1;
        VADDW.S32 Q0, Q1, Q2;
        VADDW.U8  Q0, Q1;
        VADDW.U8  Q0, Q1, Q2;
        VADDW.U16 Q0, Q1;
        VADDW.U16 Q0, Q1, Q2;
        VADDW.U32 Q0, Q1;
        VADDW.U32 Q0, Q1, Q2;

        // A8.6.275 VAND (immediate)
        // pseudo for VBIC with immediate valute bitwise inverted

        VAND.I16 Q0, #1;
        VAND.I16 Q0, Q1, #2;
        VAND.I32 Q0, #1;
        VAND.I32 Q0, Q1, #2;
        VAND.I16 D0, #1;
        VAND.I16 D0, D1, #2;
        VAND.I32 D0, #1;
        VAND.I32 D0, D1, #2;

        // A8.6.276 VAND (register)
        // VAND<c><q>{.<dt>} {<Qd>,} <Qn>, <Qm>              Encoded as Q = 1
        // VAND<c><q>{.<dt>} {<Dd>,} <Dn>, <Dm>              Encoded as Q = 0
        // dt=ignored
        // TODO: add variations with dt?

        VAND Q0, Q1;
        VAND Q0, Q1, Q2;
        VAND D0, D1;
        VAND D0, D1, D2;

        // A8.6.277 VBIC (immediate)
        // VBIC<c><q>.<dt> {<Qd>,} <Qd>, #<imm>              Encoded as Q = 1
        // VBIC<c><q>.<dt> {<Dd>,} <Dd>, #<imm>              Encoded as Q = 0
        // dt=I{16,32} // I8, I64, F32

        VBIC.I16 Q0, #1;
        VBIC.I16 Q0, Q1, #2;
        VBIC.I32 Q0, #1;
        VBIC.I32 Q0, Q1, #2;
        VBIC.I16 D0, #1;
        VBIC.I16 D0, D1, #2;
        VBIC.I32 D0, #1;
        VBIC.I32 D0, D1, #2;

        // A8.6.278 VBIC (register)
        // VBIC<c><q>{.<dt>} {<Qd>,} <Qn>, <Qm>              Encoded as Q = 1
        // VBIC<c><q>{.<dt>} {<Dd>,} <Dn>, <Dm>              Encoded as Q = 0
        // dt=ignored TODO: add variations with dt?

        VBIC Q0, Q1;
        VBIC Q0, Q1, Q2;
        VBIC D0, D1;
        VBIC D0, D1, D2;

        // A8.6.279 VBIF, VBIT, VBSL
        // V<op><c><q>{.<dt>} {<Qd>,} <Qn>, <Qm>             Encoded as Q = 1
        // V<op><c><q>{.<dt>} {<Dd>,} <Dn>, <Dm>             Encoded as Q = 0
        // op={BIF,BIT,BSL},  dt=ignored  TODO: add tests with dt??

        VBIF Q0, Q1;
        VBIF Q0, Q1, Q2;
        VBIF D0, D1;
        VBIF D0, D1, D2;
        VBIT Q0, Q1;
        VBIT Q0, Q1, Q2;
        VBIT D0, D1;
        VBIT D0, D1, D2;
        VBSL Q0, Q1;
        VBSL Q0, Q1, Q2;
        VBSL D0, D1;
        VBSL D0, D1, D2;

        // A8.6.280 VCEQ (register)
        // VCEQ<c><q>.<dt> {<Qd>,} <Qn>, <Qm>                Encoded as Q = 1
        // VCEQ<c><q>.<dt> {<Dd>,} <Dn>, <Dm>                Encoded as Q = 0
        // dt=I8, I16, I32, F32

        VCEQ.I8  Q0, Q1;
        VCEQ.I8  Q0, Q1, Q2;
        VCEQ.I8  D0, D1;
        VCEQ.I8  D0, D1, D2;
        VCEQ.I16 Q0, Q1;
        VCEQ.I16 Q0, Q1, Q2;
        VCEQ.I16 D0, D1;
        VCEQ.I16 D0, D1, D2;
        VCEQ.I32 Q0, Q1;
        VCEQ.I32 Q0, Q1, Q2;
        VCEQ.I32 D0, D1;
        VCEQ.I32 D0, D1, D2;
        VCEQ.F32 Q0, Q1;
        VCEQ.F32 Q0, Q1, Q2;
        VCEQ.F32 D0, D1;
        VCEQ.F32 D0, D1, D2;

        // A8.6.281 VCEQ (immediate #0)
        // VCEQ<c><q>.<dt> {<Qd>,} <Qm>, #0                  Encoded as Q = 1
        // VCEQ<c><q>.<dt> {<Dd>,} <Dm>, #0                  Encoded as Q = 0
        // dt=I8, I16, I32, F32

        VCEQ.I8  Q0, #0;
        VCEQ.I8  Q0, Q1, #0;
        VCEQ.I8  D0, #0;
        VCEQ.I8  D0, D1, #0;
        VCEQ.I16 Q0, #0;
        VCEQ.I16 Q0, Q1, #0;
        VCEQ.I16 D0, #0;
        VCEQ.I16 D0, D1, #0;
        VCEQ.I32 Q0, #0;
        VCEQ.I32 Q0, Q1, #0;
        VCEQ.I32 D0, #0;
        VCEQ.I32 D0, D1, #0;
        VCEQ.F32 Q0, #0;
        VCEQ.F32 Q0, Q1, #0;
        VCEQ.F32 D0, #0;
        VCEQ.F32 D0, D1, #0;

        // A8.6.282 VCGE (register)
        // VCGE<c><q>.<dt> {<Qd>,} <Qn>, <Qm>                Encoded as Q = 1
        // VCGE<c><q>.<dt> {<Dd>,} <Dn>, <Dm>                Encoded as Q = 0
        // dt=S8, S16, S32, U8, U16, U32, F32

        VCGE.S8  Q0, Q1;
        VCGE.S8  Q0, Q1, Q2;
        VCGE.S8  D0, D1;
        VCGE.S8  D0, D1, D2;
        VCGE.S16 Q0, Q1;
        VCGE.S16 Q0, Q1, Q2;
        VCGE.S16 D0, D1;
        VCGE.S16 D0, D1, D2;
        VCGE.S32 Q0, Q1;
        VCGE.S32 Q0, Q1, Q2;
        VCGE.S32 D0, D1;
        VCGE.S32 D0, D1, D2;
        VCGE.U8  Q0, Q1;
        VCGE.U8  Q0, Q1, Q2;
        VCGE.U8  D0, D1;
        VCGE.U8  D0, D1, D2;
        VCGE.U16 Q0, Q1;
        VCGE.U16 Q0, Q1, Q2;
        VCGE.U16 D0, D1;
        VCGE.U16 D0, D1, D2;
        VCGE.U32 Q0, Q1;
        VCGE.U32 Q0, Q1, Q2;
        VCGE.U32 D0, D1;
        VCGE.U32 D0, D1, D2;
        VCGE.F32 Q0, Q1;
        VCGE.F32 Q0, Q1, Q2;
        VCGE.F32 D0, D1;
        VCGE.F32 D0, D1, D2;

        // A8.6.283 VCGE (immediate #0)
        // VCGE<c><q>.<dt> {<Qd>,} <Qm>, #0                  Encoded as Q = 1
        // VCGE<c><q>.<dt> {<Dd>,} <Dm>, #0                  Encoded as Q = 0
        // dt=S8, S16, S32, F32

        VCGE.S8  Q0, #0;
        VCGE.S8  Q0, Q1, #0;
        VCGE.S8  D0, #0;
        VCGE.S8  D0, D1, #0;
        VCGE.S16 Q0, #0;
        VCGE.S16 Q0, Q1, #0;
        VCGE.S16 D0, #0;
        VCGE.S16 D0, D1, #0;
        VCGE.S32 Q0, #0;
        VCGE.S32 Q0, Q1, #0;
        VCGE.S32 D0, #0;
        VCGE.S32 D0, D1, #0;
        VCGE.F32 Q0, #0;
        VCGE.F32 Q0, Q1, #0;
        VCGE.F32 D0, #0;
        VCGE.F32 D0, D1, #0;

        // A8.6.284 VCGT (register)
        // VCGT<c><q>.<dt> {<Qd>,} <Qn>, <Qm>                Encoded as Q = 1
        // VCGT<c><q>.<dt> {<Dd>,} <Dn>, <Dm>                Encoded as Q = 0
        // dt=S8, S16, S32, U8, U16, U32, F32

        VCGT.S8  Q0, Q1;
        VCGT.S8  Q0, Q1, Q2;
        VCGT.S8  D0, D1;
        VCGT.S16 D0, D1, D2;
        VCGT.S16 Q0, Q1;
        VCGT.S16 Q0, Q1, Q2;
        VCGT.S16 D0, D1;
        VCGT.S32 D0, D1, D2;
        VCGT.S32 Q0, Q1;
        VCGT.S32 Q0, Q1, Q2;
        VCGT.S32 D0, D1;
        VCGT.U8  Q0, Q1;
        VCGT.U8  Q0, Q1, Q2;
        VCGT.U8  D0, D1;
        VCGT.U16 D0, D1, D2;
        VCGT.U16 Q0, Q1;
        VCGT.U16 Q0, Q1, Q2;
        VCGT.U16 D0, D1;
        VCGT.U32 D0, D1, D2;
        VCGT.U32 Q0, Q1;
        VCGT.U32 Q0, Q1, Q2;
        VCGT.U32 D0, D1;
        VCGT.F32 Q0, Q1;
        VCGT.F32 Q0, Q1, Q2;
        VCGT.F32 D0, D1;
        VCGT.F32 D0, D1, D2;

        // A8.6.285 VCGT (immediate #0)
        // VCGT<c><q>.<dt> {<Qd>,} <Qm>, #0                  Encoded as Q = 1
        // VCGT<c><q>.<dt> {<Dd>,} <Dm>, #0                  Encoded as Q = 0
        // dt=S8, S16, S32, F32

        VCGT.S8  Q0, #0;
        VCGT.S8  Q0, Q1, #0;
        VCGT.S8  D0, #0;
        VCGT.S8  D0, D1, #0;
        VCGT.S16 Q0, #0;
        VCGT.S16 Q0, Q1, #0;
        VCGT.S16 D0, #0;
        VCGT.S16 D0, D1, #0;
        VCGT.S32 Q0, #0;
        VCGT.S32 Q0, Q1, #0;
        VCGT.S32 D0, #0;
        VCGT.S32 D0, D1, #0;
        VCGT.F32 Q0, #0;
        VCGT.F32 Q0, Q1, #0;
        VCGT.F32 D0, #0;
        VCGT.F32 D0, D1, #0;

        // A8.6.286 VCLE (register)
        // pseudo instruction, VCGE with the operands reversed

        VCLE.S8  Q0, Q1;
        VCLE.S8  Q0, Q1, Q2;
        VCLE.S8  D0, D1;
        VCLE.S8  D0, D1, D2;
        VCLE.S16 Q0, Q1;
        VCLE.S16 Q0, Q1, Q2;
        VCLE.S16 D0, D1;
        VCLE.S16 D0, D1, D2;
        VCLE.S32 Q0, Q1;
        VCLE.S32 Q0, Q1, Q2;
        VCLE.S32 D0, D1;
        VCLE.S32 D0, D1, D2;
        VCLE.U8  Q0, Q1;
        VCLE.U8  Q0, Q1, Q2;
        VCLE.U8  D0, D1;
        VCLE.U8  D0, D1, D2;
        VCLE.U16 Q0, Q1;
        VCLE.U16 Q0, Q1, Q2;
        VCLE.U16 D0, D1;
        VCLE.U16 D0, D1, D2;
        VCLE.U32 Q0, Q1;
        VCLE.U32 Q0, Q1, Q2;
        VCLE.U32 D0, D1;
        VCLE.U32 D0, D1, D2;
        VCLE.F32 Q0, Q1;
        VCLE.F32 Q0, Q1, Q2;
        VCLE.F32 D0, D1;
        VCLE.F32 D0, D1, D2;

        // A8.6.287 VCLE (immediate #0)
        // VCLE<c><q>.<dt> {<Qd>,} <Qm>, #0                  Encoded as Q = 1
        // VCLE<c><q>.<dt> {<Dd>,} <Dm>, #0                  Encoded as Q = 0
        // dt=S8, S16, S32, F32

        VCLE.S8  Q0, #0;
        VCLE.S8  Q0, Q1, #0;
        VCLE.S8  D0, #0;
        VCLE.S8  D0, D1, #0;
        VCLE.S16 Q0, #0;
        VCLE.S16 Q0, Q1, #0;
        VCLE.S16 D0, #0;
        VCLE.S16 D0, D1, #0;
        VCLE.S32 Q0, #0;
        VCLE.S32 Q0, Q1, #0;
        VCLE.S32 D0, #0;
        VCLE.S32 D0, D1, #0;
        VCLE.F32 Q0, #0;
        VCLE.F32 Q0, Q1, #0;
        VCLE.F32 D0, #0;
        VCLE.F32 D0, D1, #0;

        // A8.6.288 VCLS
        // VCLS<c><q>.<dt> <Qd>, <Qm>                        Encoded as Q = 1
        // VCLS<c><q>.<dt> <Dd>, <Dm>                        Encoded as Q = 0
        // dt=S8, S16, S32

        VCLS.S8  Q0, Q1;
        VCLS.S8  D0, D1;
        VCLS.S16 Q0, Q1;
        VCLS.S16 D0, D1;
        VCLS.S32 Q0, Q1;
        VCLS.S32 D0, D1;

        // A8.6.289 VCLT (register)
        // pseudo instruction, VCGT with the operands reversed

        VCLT.S8  Q0, Q1;
        VCLT.S8  Q0, Q1, Q2;
        VCLT.S8  D0, D1;
        VCLT.S16 D0, D1, D2;
        VCLT.S16 Q0, Q1;
        VCLT.S16 Q0, Q1, Q2;
        VCLT.S16 D0, D1;
        VCLT.S32 D0, D1, D2;
        VCLT.S32 Q0, Q1;
        VCLT.S32 Q0, Q1, Q2;
        VCLT.S32 D0, D1;
        VCLT.U8  Q0, Q1;
        VCLT.U8  Q0, Q1, Q2;
        VCLT.U8  D0, D1;
        VCLT.U16 D0, D1, D2;
        VCLT.U16 Q0, Q1;
        VCLT.U16 Q0, Q1, Q2;
        VCLT.U16 D0, D1;
        VCLT.U32 D0, D1, D2;
        VCLT.U32 Q0, Q1;
        VCLT.U32 Q0, Q1, Q2;
        VCLT.U32 D0, D1;
        VCLT.F32 Q0, Q1;
        VCLT.F32 Q0, Q1, Q2;
        VCLT.F32 D0, D1;
        VCLT.F32 D0, D1, D2;

        // A8.6.290 VCLT (immediate #0)
        // VCLT<c><q>.<dt> {<Qd>,} <Qm>, #0                  Encoded as Q = 1
        // VCLT<c><q>.<dt> {<Dd>,} <Dm>, #0                  Encoded as Q = 0
        // dt=S8, S16, S32, F32

        VCLT.S8  Q0, #0;
        VCLT.S8  Q0, Q1, #0;
        VCLT.S8  D0, #0;
        VCLT.S8  D0, D1, #0;
        VCLT.S16 Q0, #0;
        VCLT.S16 Q0, Q1, #0;
        VCLT.S16 D0, #0;
        VCLT.S16 D0, D1, #0;
        VCLT.S32 Q0, #0;
        VCLT.S32 Q0, Q1, #0;
        VCLT.S32 D0, #0;
        VCLT.S32 D0, D1, #0;
        VCLT.F32 Q0, #0;
        VCLT.F32 Q0, Q1, #0;
        VCLT.F32 D0, #0;
        VCLT.F32 D0, D1, #0;

        // A8.6.291 VCLZ
        // VCLZ<c><q>.<dt> <Qd>, <Qm>                        Encoded as Q = 1
        // VCLZ<c><q>.<dt> <Dd>, <Dm>                        Encoded as Q = 0
        // dt=I8, I16, I32

        VCLZ.I8  Q0, Q1;
        VCLZ.I8  D0, D1;
        VCLZ.I16 Q0, Q1;
        VCLZ.I16 D0, D1;
        VCLZ.I32 Q0, Q1;
        VCLZ.I32 D0, D1;

        // A8.6.292 VCMP, VCMPE
        // VCMP{E}<c><q>.F64 <Dd>, <Dm>                      Encoding T1 / A1, sz = 1
        // VCMP{E}<c><q>.F32 <Sd>, <Sm>                      Encoding T1 / A1, sz = 0
        // VCMP{E}<c><q>.F64 <Dd>, #0.0                      Encoding T2 / A2, sz = 1
        // VCMP{E}<c><q>.F32 <Sd>, #0.0                      Encoding T2 / A2, sz = 0

        VCMP.F64 D0, D1;
        VCMPE.F64 D0, D1;
        VCMP.F32 S0, S1;
        VCMPE.F32 S0, S1;
        VCMP.F64 D0, #0;   // supposed to be 0.0
        VCMPE.F64 D0, #0;  // supposed to be 0.0
        VCMP.F32 S0, #0;   // supposed to be 0.0
        VCMPE.F32 S0, #0;  // supposed to be 0.0

        // A8.6.293 VCNT
        // VCNT<c><q>.8 <Qd>, <Qm>                           Encoded as Q = 1
        // VCNT<c><q>.8 <Dd>, <Dm>                           Encoded as Q = 0

        VCNT.8 Q0, Q1;
        VCNT.8 D0, D1;

        // A8.6.294 VCVT (between floating-point and integer, Advanced SIMD)
        // VCVT<c><q>.<Td>.<Tm> <Qd>, <Qm>                   Encoded as Q = 1
        // VCVT<c><q>.<Td>.<Tm> <Dd>, <Dm>                   Encoded as Q = 0
        // .Td.Tm = .S32.F32 .U32.F32 .F32.S32 .F32.U32

        VCVT.S32.F32 Q0, Q1;
        VCVT.S32.F32 D0, D1;
        VCVT.U32.F32 Q0, Q1;
        VCVT.U32.F32 D0, D1;
        VCVT.F32.S32 Q0, Q1;
        VCVT.F32.S32 D0, D1;
        VCVT.F32.U32 Q0, Q1;
        VCVT.F32.U32 D0, D1;

        // A8.6.295 VCVT, VCVTR (between floating-point and integer, VFP)
        // VCVT{R}<c><q>.S32.F64 <Sd>, <Dm>                  opc2 = ’101’, sz = 1
        // VCVT{R}<c><q>.S32.F32 <Sd>, <Sm>                  opc2 = ’101’, sz = 0
        // VCVT{R}<c><q>.U32.F64 <Sd>, <Dm>                  opc2 = ’100’, sz = 1
        // VCVT{R}<c><q>.U32.F32 <Sd>, <Sm>                  opc2 = ’100’, sz = 0
        // VCVT<c><q>.F64.<Tm> <Dd>, <Sm>                    opc2 = ’000’, sz = 1
        // VCVT<c><q>.F32.<Tm> <Sd>, <Sm>                    opc2 = ’000’, sz = 0
        // Tm=S32, U32

        VCVT.S32.F64 S0, D1;
        VCVTR.S32.F64 S0, D1;
        VCVT.S32.F32 S0, S1;
        VCVTR.S32.F32 S0, S1;
        VCVT.U32.F64 S0, D1;
        VCVTR.U32.F64 S0, D1;
        VCVT.U32.F32 S0, S1;
        VCVTR.U32.F32 S0, S1;
        VCVT.F64.S32 D0, S1;
        VCVT.F64.U32 D0, S1;
        VCVT.F32.S32 S0, S1;
        VCVT.F32.U32 S0, S1;

        // A8.6.296 VCVT (between floating-point and fixed-point, Advanced SIMD)
        // VCVT<c><q>.<Td>.<Tm> <Qd>, <Qm>, #<fbits>         Encoded as Q = 1
        // VCVT<c><q>.<Td>.<Tm> <Dd>, <Dm>, #<fbits>         Encoded as Q = 0
        // .Td.Tm = .S32.F32 .U32.F32 .F32.S32 .F32.U32

        VCVT.S32.F32 Q0, Q1, #2;
        VCVT.S32.F32 D0, D1, #2;
        VCVT.U32.F32 Q0, Q1, #2;
        VCVT.U32.F32 D0, D1, #2;
        VCVT.F32.S32 Q0, Q1, #2;
        VCVT.F32.S32 D0, D1, #2;
        VCVT.F32.U32 Q0, Q1, #2;
        VCVT.F32.U32 D0, D1, #2;

        // A8.6.297 VCVT (between floating-point and fixed-point, VFP)
        // VCVT<c><q>.<Td>.F64 <Dd>, <Dd>, #<fbits>          op = 1, sf = 1
        // VCVT<c><q>.<Td>.F32 <Sd>, <Sd>, #<fbits>          op = 1, sf = 0
        // VCVT<c><q>.F64.<Td> <Dd>, <Dd>, #<fbits>          op = 0, sf = 1
        // VCVT<c><q>.F32.<Td> <Sd>, <Sd>, #<fbits>          op = 0, sf = 0
        // Td = S16, U16, S32, U32

        VCVT.S16.F64 D0, D1, #2;
        VCVT.S16.F32 S0, S1, #2;
        VCVT.S32.F64 D0, D1, #2;
        VCVT.S32.F32 S0, S1, #2;
        VCVT.U16.F64 D0, D1, #2;
        VCVT.U16.F32 S0, S1, #2;
        VCVT.U32.F64 D0, D1, #2;
        VCVT.U32.F32 S0, S1, #2;
        VCVT.F64.S16 D0, D1, #2;
        VCVT.F32.S16 S0, S1, #2;
        VCVT.F64.S32 D0, D1, #2;
        VCVT.F32.S32 S0, S1, #2;
        VCVT.F64.U16 D0, D1, #2;
        VCVT.F32.U16 S0, S1, #2;
        VCVT.F64.U32 D0, D1, #2;
        VCVT.F32.U32 S0, S1, #2;

        // A8.6.298 VCVT (between double-precision and single-precision)
        // VCVT<c><q>.F64.F32 <Dd>, <Sm>                     Encoded as sz = 0
        // VCVT<c><q>.F32.F64 <Sd>, <Dm>                     Encoded as sz = 1

        VCVT.F64.F32 D0, D1;
        VCVT.F32.F64 S0, D1;

        // A8.6.299 VCVT (between half-precision and single-precision, Advanced SIMD)
        // VCVT<c><q>.F32.F16 <Qd>, <Dm>                     Encoded as op = 1
        // VCVT<c><q>.F16.F32 <Dd>, <Qm>                     Encoded as op = 0

        VCVT.F32.F16 Q0, D1;
        VCVT.F16.F32 D0, Q1;

        // A8.6.300 VCVTB, VCVTT (between half-precision and single-precision, VFP)
        // VCVT<y><c><q>.F32.F16 <Sd>, <Sm>                  Encoded as op = 0
        // VCVT<y><c><q>.F16.F32 <Sd>, <Sm>                  Encoded as op = 1
        // y=B, T

        VCVTB.F32.F16 S0, S1;
        VCVTT.F32.F16 S0, S1;
        VCVTB.F16.F32 S0, S1;
        VCVTT.F16.F32 S0, S1;

        // A8.6.301 VDIV
        // VDIV<c><q>.F64 {<Dd>,} <Dn>, <Dm>                 Encoded as sz = 1
        // VDIV<c><q>.F32 {<Sd>,} <Sn>, <Sm>                 Encoded as sz = 0

        VDIV.F64 D0, D1;
        VDIV.F64 D0, D1, D2;
        VDIV.F32 S0, S1;
        VDIV.F32 S0, S1, S2;

        // A8.6.302 VDUP (scalar)
        // VDUP<c><q>.<size> <Qd>, <Dm[x]>                   Encoded as Q = 1
        // VDUP<c><q>.<size> <Dd>, <Dm[x]>                   Encoded as Q = 0
        // size=8, 16, 32

        VDUP.8  Q0, D1[2];
        VDUP.8  D0, D1[2];
        VDUP.16 Q0, D1[2];
        VDUP.16 D0, D1[2];
        VDUP.32 Q0, D1[2]; // illegal, valid range is 0..1
        VDUP.32 D0, D1[2]; // illegal, valid range is 0..1

        // A8.6.303 VDUP (ARM core register)
        // VDUP<c><q>.<size> <Qd>, <Rt>                      Encoded as Q = 1
        // VDUP<c><q>.<size> <Dd>, <Rt>                      Encoded as Q = 0
        // size=8, 16, 32

        VDUP.8  Q0, R1;
        VDUP.8  D0, R1;
        VDUP.16 Q0, R1;
        VDUP.16 D0, R1;
        VDUP.32 Q0, R1;
        VDUP.32 D0, R1;

        // A8.6.304 VEOR
        // VEOR<c><q>{.<dt>} {<Qd>,} <Qn>, <Qm>              Encoded as Q = 1
        // VEOR<c><q>{.<dt>} {<Dd>,} <Dn>, <Dm>              Encoded as Q = 0
        // dt=optional  TODO: need variations with dt?

        VEOR Q0, Q1;
        VEOR Q0, Q1, Q2;
        VEOR D0, D1;
        VEOR D0, D1, D2;

        // A8.6.305 VEXT
        // VEXT<c><q>.<size> {<Qd>,} <Qn>, <Qm>, #<imm>      Encoded as Q = 1
        // VEXT<c><q>.<size> {<Dd>,} <Dn>, <Dm>, #<imm>      Encoded as Q = 0
        // size=8, 16, 32. 64 if quad word operation

        VEXT.8  Q0, Q1, #2;
        VEXT.8  Q0, Q1, Q2, #3;
        VEXT.8  D0, D1, #2;
        VEXT.8  D0, D1, D2, #3;
        VEXT.16 Q0, Q1, #2;
        VEXT.16 Q0, Q1, Q2, #3;
        VEXT.16 D0, D1, #2;
        VEXT.16 D0, D1, D2, #3;
        VEXT.32 Q0, Q1, #2;
        VEXT.32 Q0, Q1, Q2, #3;
        VEXT.32 D0, D1, #2;
        VEXT.32 D0, D1, D2, #3;
        VEXT.64 Q0, Q1, #2;
        VEXT.64 Q0, Q1, Q2, #3;

        // A8.6.306 VHADD, VHSUB
        // VH<op><c><q>.<dt> {<Qd>,} <Qn>, <Qm>              Encoded as Q = 1
        // VH<op><c><q>.<dt> {<Dd>,} <Dn>, <Dm>              Encoded as Q = 0
        // op=ADD, SUB.  dt=S8, S16, S32, U8, U16, U32

        VHADD.S8  Q0, Q1;
        VHADD.S8  Q0, Q1, Q2;
        VHADD.S8  D0, D1;
        VHADD.S8  D0, D1, D2;
        VHADD.S16 Q0, Q1;
        VHADD.S16 Q0, Q1, Q2;
        VHADD.S16 D0, D1;
        VHADD.S16 D0, D1, D2;
        VHADD.S32 Q0, Q1;
        VHADD.S32 Q0, Q1, Q2;
        VHADD.S32 D0, D1;
        VHADD.S32 D0, D1, D2;
        VHADD.U8  Q0, Q1;
        VHADD.U8  Q0, Q1, Q2;
        VHADD.U8  D0, D1;
        VHADD.U8  D0, D1, D2;
        VHADD.U16 Q0, Q1;
        VHADD.U16 Q0, Q1, Q2;
        VHADD.U16 D0, D1;
        VHADD.U16 D0, D1, D2;
        VHADD.U32 Q0, Q1;
        VHADD.U32 Q0, Q1, Q2;
        VHADD.U32 D0, D1;
        VHADD.U32 D0, D1, D2;
        VHSUB.S8  Q0, Q1;
        VHSUB.S8  Q0, Q1, Q2;
        VHSUB.S8  D0, D1;
        VHSUB.S8  D0, D1, D2;
        VHSUB.S16 Q0, Q1;
        VHSUB.S16 Q0, Q1, Q2;
        VHSUB.S16 D0, D1;
        VHSUB.S16 D0, D1, D2;
        VHSUB.S32 Q0, Q1;
        VHSUB.S32 Q0, Q1, Q2;
        VHSUB.S32 D0, D1;
        VHSUB.S32 D0, D1, D2;
        VHSUB.U8  Q0, Q1;
        VHSUB.U8  Q0, Q1, Q2;
        VHSUB.U8  D0, D1;
        VHSUB.U8  D0, D1, D2;
        VHSUB.U16 Q0, Q1;
        VHSUB.U16 Q0, Q1, Q2;
        VHSUB.U16 D0, D1;
        VHSUB.U16 D0, D1, D2;
        VHSUB.U32 Q0, Q1;
        VHSUB.U32 Q0, Q1, Q2;
        VHSUB.U32 D0, D1;
        VHSUB.U32 D0, D1, D2;

        // A8.6.307 VLD1 (multiple single elements)
        // VLD1<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VLD1<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VLD1<c><q>.<size> <list>, [<Rn>{@<align>}],       <Rm> Rm = other values
        // size=8, 16, 32, 64.  list={ Dd .. Dd+4 } (1..4 registers).  align=64, 128, 256, omitted
        // TODO: needs @align

        VLD1.I8  { D0 }, [R1];

        // A8.6.308 VLD1 (single element to one lane)
        // VLD1<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VLD1<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VLD1<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd[x] }.  align=16, 32, omitted
        // TODO: needs @align

        // A8.6.309 VLD1 (single element to all lanes)
        // VLD1<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VLD1<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VLD1<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd[] }, { Dd[], Dd+1[] }.  align=16, 32, omitted
        // TODO: needs @align

        // A8.6.310 VLD2 (multiple 2-element structures)
        // VLD2<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VLD2<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VLD2<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd, Dd+1 }, { Dd, Dd+2 }, or {+0, +1, +2, +3}.  align=64, 128, 256, omitted
        // TODO: needs @align

        VLD2.I8  { D0 }, [R1];

        // A8.6.311 VLD2 (single 2-element structure to one lane)
        // VLD2<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VLD2<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VLD2<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd[x], Dd+1[x] }, { Dd[x], Dd+2[x] }.  align=omitted, 16, 32, 64
        // TODO: needs @align

        // A8.6.312 VLD2 (single 2-element structure to all lanes)
        // VLD2<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VLD2<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VLD2<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd[], Dd+1[] }, { Dd[], Dd+2[] }.  align=omitted, 16, 32, 64
        // TODO: needs @align

        // A8.6.313 VLD3 (multiple 3-element structures)
        // VLD3<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VLD3<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VLD3<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd, Dd+1, Dd+2 }, { Dd, Dd+2, Dd+4 }.  align=omitted, 64
        // TODO: needs @align

        VLD3.I8  { D0 }, [R1];

        // A8.6.314 VLD3 (single 3-element structure to one lane)
        // VLD3<c><q>.<size> <list>, [<Rn>]                  Rm = ’1111’
        // VLD3<c><q>.<size> <list>, [<Rn>]!                 Rm = ’1101’
        // VLD3<c><q>.<size> <list>, [<Rn>], <Rm>            Rm = other values
        // size=8, 16, 32.  list={ Dd[x], Dd+1[x], Dd+2[x] }, { Dd[x], Dd+2[x], Dd+4[x] }.
        // TODO: needs @align

        // A8.6.315 VLD3 (single 3-element structure to all lanes)
        // VLD3<c><q>.<size> <list>, [<Rn>]                  Rm = ’1111’
        // VLD3<c><q>.<size> <list>, [<Rn>]!                 Rm = ’1101’
        // VLD3<c><q>.<size> <list>, [<Rn>], <Rm>            Rm = other values
        // size=8, 16, 32.  list={ Dd[], Dd+1[], Dd+2[] }, { Dd[], Dd+2[], Dd+4[] }.
        // TODO: needs @align

        // A8.6.316 VLD4 (multiple 4-element structures)
        // VLD4<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VLD4<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VLD4<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd, Dd+1, Dd+2, Dd+3 }, { Dd, Dd+2, Dd+4, Dd+6 }.  alignment=omitted, 64, 128, 256
        // TODO: needs @align

        VLD4.I8  { D0 }, [R1];

        // A8.6.317 VLD4 (single 4-element structure to one lane)
        // VLD4<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VLD4<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VLD4<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd[x], Dd+1[x], Dd+2[x], Dd+3[x] }, { Dd[x], Dd+2[x], Dd+4[x], Dd+6 }[x].  alignment=omitted, 64, 128, 256
        // TODO: needs @align

        // A8.6.318 VLD4 (single 4-element structure to all lanes)
        // VLD4<c><q>.<size> <list>, [<Rn>{ @<align>}]       Rm = ’1111’
        // VLD4<c><q>.<size> <list>, [<Rn>{ @<align>}]!      Rm = ’1101’
        // VLD4<c><q>.<size> <list>, [<Rn>{ @<align>}], <Rm> Rm = other values
        // size=8, 16, 32.  list={ Dd[], Dd+1[], Dd+2[], Dd+3[] }, { Dd[], Dd+2[], Dd+4[], Dd+6 }[].  alignment=omitted, 64, 128, 256
        // TODO: needs @align

        // A8.6.319 VLDM
        // VLDM{<mode>}<c><q>{.<size>} <Rn>{!}, <list>
        // mode=IA, DB.  list=list of D or Q registers, min 1

        VLDMIA R0, { Q0 };
        VLDMIA R0!, { Q0 };
        VLDMDB R0, { Q0 };
        VLDMDB R0!, { Q0 };

        // A8.6.320 VLDR
        // VLDR<c><q>{.64} <Dd>, [<Rn> {, #+/-<imm>}]        Encoding T1 / A1, immediate form
        // VLDR<c><q>{.64} <Dd>, <label>                     Encoding T1 / A1, normal literal form
        // VLDR<c><q>{.64} <Dd>, [PC, #+/-<imm>]             Encoding T1 / A1, alternative literal form
        // VLDR<c><q>{.32} <Sd>, [<Rn> {, #+/-<imm>}]        Encoding T2 / A2, immediate form
        // VLDR<c><q>{.32} <Sd>, <label>                     Encoding T2 / A2, normal literal form
        // VLDR<c><q>{.32} <Sd>, [PC, #+/-<imm>]             Encoding T2 / A2, alternative literal form

VLDR_before:
        VLDR    D0, [R1];
        VLDR    D0, [R1, #2];
        VLDR.64 D0, [R1];
        VLDR.64 D0, [R1, #2];

        VLDR    D0, VLDR_after;
        VLDR    D0, VLDR_before;
        VLDR.64 D0, VLDR_after;
        VLDR.64 D0, VLDR_before;

        VLDR    D0, [PC, #2];
        VLDR.64 D0, [PC, #2];

        VLDR    S0, [R1];
        VLDR    S0, [R1, #2];
        VLDR.32 S0, [R1];
        VLDR.32 S0, [R1, #2];

        VLDR    S0, VLDR_after;
        VLDR    S0, VLDR_before;
        VLDR.32 S0, VLDR_after;
        VLDR.32 S0, VLDR_before;

        VLDR    S0, [PC, #2];
        VLDR.32 S0, [PC, #2];
VLDR_after:

        // A8.6.321 VMAX, VMIN (integer)
        // V<op><c><q>.<dt> {<Qd>,} <Qn>, <Qm>               Encoded as Q = 1
        // V<op><c><q>.<dt> {<Dd>,} <Dn>, <Dm>               Encoded as Q = 0
        // op=MIN, MAX.  dt=S8, S16, S32, U8, U16, U32.

        VMIN.S8  Q0, Q1;
        VMIN.S8  Q0, Q1, Q2;
        VMIN.S16 Q0, Q1;
        VMIN.S16 Q0, Q1, Q2;
        VMIN.S32 Q0, Q1;
        VMIN.S32 Q0, Q1, Q2;
        VMIN.U8  Q0, Q1;
        VMIN.U8  Q0, Q1, Q2;
        VMIN.U16 Q0, Q1;
        VMIN.U16 Q0, Q1, Q2;
        VMIN.U32 Q0, Q1;
        VMIN.U32 Q0, Q1, Q2;
        VMIN.S8  D0, D1;
        VMIN.S8  D0, D1, D2;
        VMIN.S16 D0, D1;
        VMIN.S16 D0, D1, D2;
        VMIN.S32 D0, D1;
        VMIN.S32 D0, D1, D2;
        VMIN.U8  D0, D1;
        VMIN.U8  D0, D1, D2;
        VMIN.U16 D0, D1;
        VMIN.U16 D0, D1, D2;
        VMIN.U32 D0, D1;
        VMIN.U32 D0, D1, D2;
        VMAX.S8  Q0, Q1;
        VMAX.S8  Q0, Q1, Q2;
        VMAX.S16 Q0, Q1;
        VMAX.S16 Q0, Q1, Q2;
        VMAX.S32 Q0, Q1;
        VMAX.S32 Q0, Q1, Q2;
        VMAX.U8  Q0, Q1;
        VMAX.U8  Q0, Q1, Q2;
        VMAX.U16 Q0, Q1;
        VMAX.U16 Q0, Q1, Q2;
        VMAX.U32 Q0, Q1;
        VMAX.U32 Q0, Q1, Q2;
        VMAX.S8  D0, D1;
        VMAX.S8  D0, D1, D2;
        VMAX.S16 D0, D1;
        VMAX.S16 D0, D1, D2;
        VMAX.S32 D0, D1;
        VMAX.S32 D0, D1, D2;
        VMAX.U8  D0, D1;
        VMAX.U8  D0, D1, D2;
        VMAX.U16 D0, D1;
        VMAX.U16 D0, D1, D2;
        VMAX.U32 D0, D1;
        VMAX.U32 D0, D1, D2;

        // A8.6.322 VMAX, VMIN (floating-point)
        // V<op><c><q>.F32 {<Qd>,} <Qn>, <Qm>                Encoded as Q = 1
        // V<op><c><q>.F32 {<Dd>,} <Dn>, <Dm>                Encoded as Q = 0
        // OP=MIN, MAX

        VMIN.F32 Q0, Q1;
        VMIN.F32 Q0, Q1, Q2;
        VMIN.F32 D0, D1;
        VMIN.F32 D0, D1, D2;
        VMAX.F32 Q0, Q1;
        VMAX.F32 Q0, Q1, Q2;
        VMAX.F32 D0, D1;
        VMAX.F32 D0, D1, D2;

        // A8.6.323 VMLA, VMLAL, VMLS, VMLSL (integer)
        // V<op><c><q>.<type><size> <Qd>, <Qn>, <Qm>         Encoding T1 / A1, Q = 1
        // V<op><c><q>.<type><size> <Dd>, <Dn>, <Dm>         Encoding T1 / A1, Q = 0
        // V<op>L<c><q>.<type><size> <Qd>, <Dn>, <Dm>        Encoding T2 / A2
        // op=MLA, MLS.  type=S, U, I.  size=8, 16, 32.

        VMLA.S8  Q0, Q1, Q2;
        VMLA.S16 Q0, Q1, Q2;
        VMLA.S32 Q0, Q1, Q2;
        VMLA.U8  Q0, Q1, Q2;
        VMLA.U16 Q0, Q1, Q2;
        VMLA.U32 Q0, Q1, Q2;
        VMLA.S8  D0, D1, D2;
        VMLA.S16 D0, D1, D2;
        VMLA.S32 D0, D1, D2;
        VMLA.U8  D0, D1, D2;
        VMLA.U16 D0, D1, D2;
        VMLA.U32 D0, D1, D2;
        VMLAL.S8  Q0, D1, D2;
        VMLAL.S16 Q0, D1, D2;
        VMLAL.S32 Q0, D1, D2;
        VMLAL.U8  Q0, D1, D2;
        VMLAL.U16 Q0, D1, D2;
        VMLAL.U32 Q0, D1, D2;
        VMLS.S8  Q0, Q1, Q2;
        VMLS.S16 Q0, Q1, Q2;
        VMLS.S32 Q0, Q1, Q2;
        VMLS.U8  Q0, Q1, Q2;
        VMLS.U16 Q0, Q1, Q2;
        VMLS.U32 Q0, Q1, Q2;
        VMLS.S8  D0, D1, D2;
        VMLS.S16 D0, D1, D2;
        VMLS.S32 D0, D1, D2;
        VMLS.U8  D0, D1, D2;
        VMLS.U16 D0, D1, D2;
        VMLS.U32 D0, D1, D2;
        VMLSL.S8  Q0, D1, D2;
        VMLSL.S16 Q0, D1, D2;
        VMLSL.S32 Q0, D1, D2;
        VMLSL.U8  Q0, D1, D2;
        VMLSL.U16 Q0, D1, D2;
        VMLSL.U32 Q0, D1, D2;

        // A8.6.324 VMLA, VMLS (floating-point)
        // V<op><c><q>.F32 <Qd>, <Qn>, <Qm>                  Encoding T1 / A1, Q = 1, sz = 0
        // V<op><c><q>.F32 <Dd>, <Dn>, <Dm>                  Encoding T1 / A1, Q = 0, sz = 0
        // V<op><c><q>.F64 <Dd>, <Dn>, <Dm>                  Encoding T2 / A2, sz = 1
        // V<op><c><q>.F32 <Sd>, <Sn>, <Sm>                  Encoding T2 / A2, sz = 0
        // op=MLA, MLS

        VMLA.F32 Q0, Q1, Q2;
        VMLA.F32 D0, D1, D2;
        VMLA.F64 D0, D1, D2;
        VMLA.F32 S0, S1, S2;
        VMLS.F32 Q0, Q1, Q2;
        VMLS.F32 D0, D1, D2;
        VMLS.F64 D0, D1, D2;
        VMLS.F32 S0, S1, S2;

        // A8.6.325 VMLA, VMLAL, VMLS, VMLSL (by scalar)
        // V<op><c><q>.<type><size> <Qd>, <Qn>, <Dm[x]>      Encoding T1 / A1, Q = 1
        // V<op><c><q>.<type><size> <Dd>, <Dn>, <Dm[x]>      Encoding T1 / A1, Q = 0
        // V<op>L<c><q>.<type><size> <Qd>, <Dn>, <Dm[x]>     Encoding T2 / A2
        // op=MLA, MLS.  type=S, U, I, F.  size=16, 32

        VMLA.S16 Q0, Q1, D2[3];
        VMLA.S32 Q0, Q1, D2[3];
        VMLA.U16 Q0, Q1, D2[3];
        VMLA.U32 Q0, Q1, D2[3];
        VMLA.I16 Q0, Q1, D2[3];
        VMLA.I32 Q0, Q1, D2[3];
        VMLA.F16 Q0, Q1, D2[3];
        VMLA.F32 Q0, Q1, D2[3];
        VMLA.S16 D0, D1, D2[3];
        VMLA.S32 D0, D1, D2[3];
        VMLA.U16 D0, D1, D2[3];
        VMLA.U32 D0, D1, D2[3];
        VMLA.I16 D0, D1, D2[3];
        VMLA.I32 D0, D1, D2[3];
        VMLA.F16 D0, D1, D2[3];
        VMLA.F32 D0, D1, D2[3];
        VMLAL.S16 Q0, D1, D2[3];
        VMLAL.S32 Q0, D1, D2[3];
        VMLAL.U16 Q0, D1, D2[3];
        VMLAL.U32 Q0, D1, D2[3];
        VMLAL.I16 Q0, D1, D2[3];
        VMLAL.I32 Q0, D1, D2[3];
        VMLAL.F16 Q0, D1, D2[3];
        VMLAL.F32 Q0, D1, D2[3];
        VMLS.S16 Q0, Q1, D2[3];
        VMLS.S32 Q0, Q1, D2[3];
        VMLS.U16 Q0, Q1, D2[3];
        VMLS.U32 Q0, Q1, D2[3];
        VMLS.I16 Q0, Q1, D2[3];
        VMLS.I32 Q0, Q1, D2[3];
        VMLS.F16 Q0, Q1, D2[3];
        VMLS.F32 Q0, Q1, D2[3];
        VMLS.S16 D0, D1, D2[3];
        VMLS.S32 D0, D1, D2[3];
        VMLS.U16 D0, D1, D2[3];
        VMLS.U32 D0, D1, D2[3];
        VMLS.I16 D0, D1, D2[3];
        VMLS.I32 D0, D1, D2[3];
        VMLS.F16 D0, D1, D2[3];
        VMLS.F32 D0, D1, D2[3];
        VMLSL.S16 Q0, D1, D2[3];
        VMLSL.S32 Q0, D1, D2[3];
        VMLSL.U16 Q0, D1, D2[3];
        VMLSL.U32 Q0, D1, D2[3];
        VMLSL.I16 Q0, D1, D2[3];
        VMLSL.I32 Q0, D1, D2[3];
        VMLSL.F16 Q0, D1, D2[3];
        VMLSL.F32 Q0, D1, D2[3];

        // A8.6.326 VMOV (immediate)
        // VMOV<c><q>.<dt> <Qd>, #<imm>                      Encoding T1 / A1, Q = 1
        // VMOV<c><q>.<dt> <Dd>, #<imm>                      Encoding T1 / A1, Q = 0
        // VMOV<c>.F64 <Dd>, #<imm>                          Encoding T2 / A2, sz = 1
        // VMOV<c>.F32 <Sd>, #<imm>                          Encoding T2 / A2, sz = 0
        // dt=I8, I16, I32, I64, F32

        VMOV.I8  Q0, #1;
        VMOV.I16 Q0, #1;
        VMOV.I32 Q0, #1;
        VMOV.I64 Q0, #1;
        VMOV.F32 Q0, #1;
        VMOV.I8  D0, #1;
        VMOV.I16 D0, #1;
        VMOV.I32 D0, #1;
        VMOV.I64 D0, #1;
        VMOV.F32 D0, #1;
        VMOV.F64 D0, #1;
        VMOV.F32 S0, #1;

        // A8.6.327 VMOV (register)
        // VMOV<c><q>{.<dt>} <Qd>, <Qm>                      Encoding T1 / A1, Q = 1
        // VMOV<c><q>{.<dt>} <Dd>, <Dm>                      Encoding T1 / A1, Q = 0
        // VMOV<c><q>.F64 <Dd>, <Dm>                         Encoding T2 / A2, sz = 1
        // VMOV<c><q>.F32 <Sd>, <Sm>                         Encoding T2 / A2, sz = 0
        // dt=ignored, but must not be F64.  TODO: add tests with dt

        VMOV Q0, Q1;
        VMOV D0, D1;
        VMOV.F64 D0, D1;
        VMOV.F32 S0, S1;

        // A8.6.328 VMOV (ARM core register to scalar)
        // VMOV<c>{.<size>} <Dd[x]>, <Rt>
        // size=omitted, 8, 16, 32

        VMOV    D0[1], R2;
        VMOV.8  D0[1], R2;
        VMOV.16 D0[1], R2;
        VMOV.32 D0[1], R2;

        // A8.6.329 VMOV (scalar to ARM core register)
        // VMOV<c>{.<dt>} <Rt>, <Dn[x]>
        // dt=S8, S16, U8, U16, 32

        VMOV     R0, D1[0];
        VMOV.S8  R0, D1[0];
        VMOV.S16 R0, D1[0];
        VMOV.U8  R0, D1[0];
        VMOV.U16 R0, D1[0];
        VMOV.32  R0, D1[0];

        // A8.6.330 VMOV (between ARM core register and single-precision register)
        // VMOV<c><q> <Sn>, <Rt>                             Encoded as op = 0
        // VMOV<c><q> <Rt>, <Sn>                             Encoded as op = 1

        VMOV S0, R1;
        VMOV R0, S1;

        // A8.6.331 VMOV (between two ARM core registers and two single-precision registers)
        // VMOV<c><q> <Sm>, <Sm1>, <Rt>, <Rt2>               Encoded as op = 0
        // VMOV<c><q> <Rt>, <Rt2>, <Sm>, <Sm1>               Encoded as op = 1

        VMOV S0, S1, R2, R3;
        VMOV R0, R1, S2, S3;

        // A8.6.332 VMOV (between two ARM core registers and a doubleword extension register)
        // VMOV<c><q> <Dm>, <Rt>, <Rt2>                      Encoded as op = 0
        // VMOV<c><q> <Rt>, <Rt2>, <Dm>                      Encoded as op = 1

        VMOV D0, R1, R2;
        VMOV R0, R1, D2;

        // A8.6.333 VMOVL
        // VMOVL<c><q>.<dt> <Qd>, <Dm>
        // dt=S8, S16, S32, U8, U16, U32

        VMOVL.S8  Q0, D1;
        VMOVL.S16 Q0, D1;
        VMOVL.S32 Q0, D1;
        VMOVL.U8  Q0, D1;
        VMOVL.U16 Q0, D1;
        VMOVL.U32 Q0, D1;

        // A8.6.334 VMOVN
        // VMOVN<c><q>.<dt> <Dd>, <Qm>
        // dt=I16, I32, I64

        VMOVN.I16 D0, Q1;
        VMOVN.I32 D0, Q1;
        VMOVN.I64 D0, Q1;

        // A8.6.335 VMRS
        // VMRS<c><q> <Rt>, FPSCR

        VMRS R0, FPSCR;
        VMRS APSR_nzcv, FPSCR;

        // A8.6.336 VMSR
        // VMSR<c><q> FPSCR, <Rt>

        VMSR FPSCR, R0;

        // A8.6.337 VMUL, VMULL (integer and polynomial)
        // VMUL<c><q>.<type><size> {<Qd>,} <Qn>, <Qm>        Encoding T1 / A1. Q = 1
        // VMUL<c><q>.<type><size> {<Dd>,} <Dn>, <Dm>        Encoding T1 / A1. Q = 0
        // VMULL<c><q>.<type><size> <Qd>, <Dn>, <Dm>         Encoding T2 / A2
        // type=S, U, I, P.  size=8, 16, 32

        VMUL.S8  Q0, Q1;
        VMUL.S8  Q0, Q1, Q2;
        VMUL.S16 Q0, Q1;
        VMUL.S16 Q0, Q1, Q2;
        VMUL.S32 Q0, Q1;
        VMUL.S32 Q0, Q1, Q2;
        VMUL.U8  Q0, Q1;
        VMUL.U8  Q0, Q1, Q2;
        VMUL.U16 Q0, Q1;
        VMUL.U16 Q0, Q1, Q2;
        VMUL.U32 Q0, Q1;
        VMUL.U32 Q0, Q1, Q2;
        VMUL.I8  Q0, Q1;
        VMUL.I8  Q0, Q1, Q2;
        VMUL.I16 Q0, Q1;
        VMUL.I16 Q0, Q1, Q2;
        VMUL.I32 Q0, Q1;
        VMUL.I32 Q0, Q1, Q2;
        VMUL.P8  Q0, Q1;
        VMUL.P8  Q0, Q1, Q2;

        VMUL.S8  D0, D1;
        VMUL.S8  D0, D1, D2;
        VMUL.S16 D0, D1;
        VMUL.S16 D0, D1, D2;
        VMUL.S32 D0, D1;
        VMUL.S32 D0, D1, D2;
        VMUL.U8  D0, D1;
        VMUL.U8  D0, D1, D2;
        VMUL.U16 D0, D1;
        VMUL.U16 D0, D1, D2;
        VMUL.U32 D0, D1;
        VMUL.U32 D0, D1, D2;
        VMUL.I8  D0, D1;
        VMUL.I8  D0, D1, D2;
        VMUL.I16 D0, D1;
        VMUL.I16 D0, D1, D2;
        VMUL.I32 D0, D1;
        VMUL.I32 D0, D1, D2;
        VMUL.P8  D0, D1;
        VMUL.P8  D0, D1, D2;

        VMULL.S8  Q0, D1, D2;
        VMULL.S16 Q0, D1, D2;
        VMULL.S32 Q0, D1, D2;
        VMULL.U8  Q0, D1, D2;
        VMULL.U16 Q0, D1, D2;
        VMULL.U32 Q0, D1, D2;
        VMULL.I8  Q0, D1, D2;
        VMULL.I16 Q0, D1, D2;
        VMULL.I32 Q0, D1, D2;
        VMULL.P8  Q0, D1, D2;

        // A8.6.338 VMUL (floating-point)
        // VMUL<c><q>.F32 {<Qd>,} <Qn>, <Qm>                 Encoding T1 / A1, Q = 1, sz = 0
        // VMUL<c><q>.F32 {<Dd>,} <Dn>, <Dm>                 Encoding T1 / A1, Q = 0, sz = 0
        // VMUL<c><q>.F64 {<Dd>,} <Dn>, <Dm>                 Encoding T2 / A2, sz = 1
        // VMUL<c><q>.F32 {<Sd>,} <Sn>, <Sm>                 Encoding T2 / A2, sz = 0

        VMUL.F32 Q0, Q1;
        VMUL.F32 Q0, Q1, Q2;
        VMUL.F32 D0, D1;
        VMUL.F32 D0, D1, D2;
        VMUL.F64 D0, D1;
        VMUL.F64 D0, D1, D2;
        VMUL.F32 S0, S1;
        VMUL.F32 S0, S1, S2;

        // A8.6.339 VMUL, VMULL (by scalar)
        // VMUL<c><q>.<dt> {<Qd>,} <Qn>, <Dm[x]>             Encoding T1 / A1, Q = 1
        // VMUL<c><q>.<dt> {<Dd>,} <Dn>, <Dm[x]>             Encoding T1 / A1, Q = 0
        // VMULL<c><q>.<dt> <Qd>, <Dn>, <Dm[x]>              Encoding T2 / A2
        // dt=I16, I32, F32, S16, S32, U16, U32

        VMUL.I16 Q0, D1[0];
        VMUL.I16 Q0, Q1, D2[0];
        VMUL.I32 Q0, D1[0];
        VMUL.I32 Q0, Q1, D2[0];
        VMUL.F32 Q0, D1[0];
        VMUL.F32 Q0, Q1, D2[0];
        VMUL.S16 Q0, D1[0];
        VMUL.S16 Q0, Q1, D2[0];
        VMUL.S32 Q0, D1[0];
        VMUL.S32 Q0, Q1, D2[0];
        VMUL.U16 Q0, D1[0];
        VMUL.U16 Q0, Q1, D2[0];
        VMUL.U32 Q0, D1[0];
        VMUL.U32 Q0, Q1, D2[0];

        VMUL.I16 D0, D1[0];
        VMUL.I16 D0, D1, D2[0];
        VMUL.I32 D0, D1[0];
        VMUL.I32 D0, D1, D2[0];
        VMUL.F32 D0, D1[0];
        VMUL.F32 D0, D1, D2[0];
        VMUL.S16 D0, D1[0];
        VMUL.S16 D0, D1, D2[0];
        VMUL.S32 D0, D1[0];
        VMUL.S32 D0, D1, D2[0];
        VMUL.U16 D0, D1[0];
        VMUL.U16 D0, D1, D2[0];
        VMUL.U32 D0, D1[0];
        VMUL.U32 D0, D1, D2[0];

        VMULL.I16 Q0, D1, D2[0];
        VMULL.I32 Q0, D1, D2[0];
        VMULL.F32 Q0, D1, D2[0];
        VMULL.S16 Q0, D1, D2[0];
        VMULL.S32 Q0, D1, D2[0];
        VMULL.U16 Q0, D1, D2[0];
        VMULL.U32 Q0, D1, D2[0];

        // A8.6.340 VMVN (immediate)
        // VMVN<c><q>.<dt> <Qd>, #<imm>                      Encoding T1 / A1, Q = 1
        // VMVN<c><q>.<dt> <Dd>, #<imm>                      Encoding T1 / A1, Q = 0
        // dt=I16, I32

        VMVN.I16 Q0, #1;
        VMVN.I32 Q0, #1;
        VMVN.I16 D0, #1;
        VMVN.I32 D0, #1;

        // A8.6.341 VMVN (register)
        // VMVN<c><q>{.<dt>} <Qd>, <Qm>
        // VMVN<c><q>{.<dt>} <Dd>, <Dm>
        // dt=ignored, TODO: add tests with dt

        VMVN Q0, Q1;
        VMVN D0, D1;

        // A8.6.342 VNEG
        // VNEG<c><q>.<dt> <Qd>, <Qm>                        <dt> != F64
        // VNEG<c><q>.<dt> <Dd>, <Dm>
        // VNEG<c><q>.F32 <Sd>, <Sm>                         VFP only, encoding T2/A2, sz = 0
        // dt=S8, S16, S32, F32, F64

        VNEG.S8  Q0, Q1;
        VNEG.S16 Q0, Q1;
        VNEG.S32 Q0, Q1;
        VNEG.F32 Q0, Q1;
        VNEG.S8  D0, D1;
        VNEG.S16 D0, D1;
        VNEG.S32 D0, D1;
        VNEG.F32 D0, D1;
        VNEG.F64 D0, D1;
        VNEG.F32 S0, S1;

        // A8.6.343 VNMLA, VNMLS, VNMUL
        // VN<op><c><q>.F64 <Dd>, <Dn>, <Dm>                 Encoding T1 / A1 with sz = 1
        // VN<op><c><q>.F32 <Sd>, <Sn>, <Sm>                 Encoding T1 / A1 with sz = 0
        // VNMUL<c><q>.F64 {<Dd>,} <Dn>, <Dm>                Encoding T2 / A2 with sz = 1
        // VNMUL<c><q>.F32 {<Sd>,} <Sn>, <Sm>                Encoding T2 / A2 with sz = 0
        // op=MLA, MLS

        VNMLA.F64 D0, D1, D2;
        VNMLS.F64 D0, D1, D2;
        VNMLA.F32 S0, S1, S2;
        VNMLS.F32 S0, S1, S2;
        VNMUL.F64 D0, D1;
        VNMUL.F64 D0, D1, D2;
        VNMUL.F32 S0, S1;
        VNMUL.F32 S0, S1, S2;

        // A8.6.344 VORN (immediate)
        // pseudo for VORR with immediate value bitwise inverted

        VORN.I16 Q0, #1;
        VORN.I16 Q0, Q1, #2;
        VORN.I32 Q0, #1;
        VORN.I32 Q0, Q1, #2;
        VORN.I16 D0, #1;
        VORN.I16 D0, D1, #2;
        VORN.I32 D0, #1;
        VORN.I32 D0, D1, #2;

        // A8.6.345 VORN (register)
        // VORN<c><q>{.<dt>} {<Qd>,} <Qn>, <Qm>              Encoded as Q = 1
        // VORN<c><q>{.<dt>} {<Dd>,} <Dn>, <Dm>              Encoded as Q = 0
        // dt=ignored  TODO: add dt tests

        VORN Q0, Q1;
        VORN Q0, Q1, Q2;
        VORN D0, D1;
        VORN D0, D1, D2;

        // A8.6.346 VORR (immediate)
        // VORR<c><q>.<dt> {<Qd>,} <Qd>, #<imm>              Encoded as Q = 1
        // VORR<c><q>.<dt> {<Dd>,} <Dd>, #<imm>>             Encoded as Q = 0
        // dt=I16, I32

        VORR.I16 Q0, #1;
        VORR.I16 Q0, Q1, #2;
        VORR.I32 Q0, #1;
        VORR.I32 Q0, Q1, #2;
        VORR.I16 D0, #1;
        VORR.I16 D0, D1, #2;
        VORR.I32 D0, #1;
        VORR.I32 D0, D1, #2;

        // A8.6.347 VORR (register)
        // VORR<c><q>{.<dt>} {<Qd>,} <Qn>, <Qm>              Encoded as Q = 1
        // VORR<c><q>{.<dt>} {<Dd>,} <Dn>, <Dm>              Encoded as Q = 0
        // dt=ignored TODO: add tests with dt

        VORR Q0, Q1;
        VORR Q0, Q1, Q2;
        VORR D0, D1;
        VORR D0, D1, D2;

        // A8.6.348 VPADAL
        // VPADAL<c><q>.<dt> <Qd>, <Qm>                      Encoded as Q = 1
        // VPADAL<c><q>.<dt> <Dd>, <Dm>                      Encoded as Q = 0
        // dt=S8, S16, S32, U8, U16, U32

        VPADAL.S8  Q0, Q1;
        VPADAL.S16 Q0, Q1;
        VPADAL.S32 Q0, Q1;
        VPADAL.U8  Q0, Q1;
        VPADAL.U16 Q0, Q1;
        VPADAL.U32 Q0, Q1;
        VPADAL.S8  D0, D1;
        VPADAL.S16 D0, D1;
        VPADAL.S32 D0, D1;
        VPADAL.U8  D0, D1;
        VPADAL.U16 D0, D1;
        VPADAL.U32 D0, D1;

        // A8.6.349 VPADD (integer)
        // VPADD<c><q>.<dt> {<Dd>,} <Dn>, <Dm>               Encoded as Q = 0
        // dt=I8, I16, I32

        VPADD.I8  D0, D1;
        VPADD.I8  D0, D1, D2;
        VPADD.I16 D0, D1;
        VPADD.I16 D0, D1, D2;
        VPADD.I32 D0, D1;
        VPADD.I32 D0, D1, D2;

        // A8.6.350 VPADD (floating-point)
        // VPADD<c><q>.F32 {<Dd>,} <Dn>, <Dm>                Encoded as Q = 0, sz = 0

        VPADD.F32 D0, D1;
        VPADD.F32 D0, D1, D2;

        // A8.6.351 VPADDL
        // VPADDL<c><q>.<dt> <Qd>, <Qm>                      Encoded as Q = 1
        // VPADDL<c><q>.<dt> <Dd>, <Dm>                      Encoded as Q = 0
        // dt=S8, S16, S32, U8, U16, U32

        VPADDL.S8  Q0, Q1;
        VPADDL.S16 Q0, Q1;
        VPADDL.S32 Q0, Q1;
        VPADDL.U8  Q0, Q1;
        VPADDL.U16 Q0, Q1;
        VPADDL.U32 Q0, Q1;
        VPADDL.S8  D0, D1;
        VPADDL.S16 D0, D1;
        VPADDL.S32 D0, D1;
        VPADDL.U8  D0, D1;
        VPADDL.U16 D0, D1;
        VPADDL.U32 D0, D1;

        // A8.6.352 VPMAX, VPMIN (integer)
        // VP<op><c><q>.<dt> {<Dd>,} <Dn>, <Dm>              Encoded as Q = 0
        // op=MIN, MAX.  dt=S8, S16, S32, U8, U16, U32

        VPMIN.S8  D0, D1;
        VPMIN.S8  D0, D1, D2;
        VPMIN.S16 D0, D1;
        VPMIN.S16 D0, D1, D2;
        VPMIN.S32 D0, D1;
        VPMIN.S32 D0, D1, D2;
        VPMIN.U8  D0, D1;
        VPMIN.U8  D0, D1, D2;
        VPMIN.U16 D0, D1;
        VPMIN.U16 D0, D1, D2;
        VPMIN.U32 D0, D1;
        VPMIN.U32 D0, D1, D2;
        VPMAX.S8  D0, D1;
        VPMAX.S8  D0, D1, D2;
        VPMAX.S16 D0, D1;
        VPMAX.S16 D0, D1, D2;
        VPMAX.S32 D0, D1;
        VPMAX.S32 D0, D1, D2;
        VPMAX.U8  D0, D1;
        VPMAX.U8  D0, D1, D2;
        VPMAX.U16 D0, D1;
        VPMAX.U16 D0, D1, D2;
        VPMAX.U32 D0, D1;
        VPMAX.U32 D0, D1, D2;

        // A8.6.353 VPMAX, VPMIN (floating-point)
        // VP<op><c><q>.F32 {<Dd>,} <Dn>, <Dm>               Encoded as Q = 0, sz = 0
        // op=MIN, MAX

        VPMIN.F32 D0, D1;
        VPMIN.F32 D0, D1, D2;
        VPMAX.F32 D0, D1;
        VPMAX.F32 D0, D1, D2;

        // A8.6.354 VPOP
        // VPOP<c><q>{.<size>} <list>
        // size=omitted, 32, 64

        VPOP { D0 };
        VPOP { D0, D1 };
        VPOP { S0 };
        VPOP { S0, S1 };

        // A8.6.355 VPUSH
        // VPUSH<c><q>{.<size>} <list>
        // size=omitted, 32, 64

        VPUSH { D0 };
        VPUSH { D0, D1 };
        VPUSH { S0 };
        VPUSH { S0, S1 };

        // A8.6.356 VQABS
        // VQABS<c><q>.<dt> <Qd>, <Qm>                       Encoded as Q = 1
        // VQABS<c><q>.<dt> <Dd>, <Dm>                       Encoded as Q = 0
        // dt=S8, S16, S32

        VQABS.S8  Q0, Q1;
        VQABS.S16 Q0, Q1;
        VQABS.S32 Q0, Q1;
        VQABS.S8  D0, D1;
        VQABS.S16 D0, D1;
        VQABS.S32 D0, D1;

        // A8.6.357 VQADD
        // VQADD<c><q>.<type><size> {<Qd>,} <Qn>, <Qm>       Encoded as Q = 1
        // VQADD<c><q>.<type><size> {<Dd>,} <Dn>, <Dm>       Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64

        VQADD.S8  Q0, Q1;
        VQADD.S8  Q0, Q1, Q2;
        VQADD.S16 Q0, Q1;
        VQADD.S16 Q0, Q1, Q2;
        VQADD.S32 Q0, Q1;
        VQADD.S32 Q0, Q1, Q2;
        VQADD.S64 Q0, Q1;
        VQADD.S64 Q0, Q1, Q2;
        VQADD.U8  Q0, Q1;
        VQADD.U8  Q0, Q1, Q2;
        VQADD.U16 Q0, Q1;
        VQADD.U16 Q0, Q1, Q2;
        VQADD.U32 Q0, Q1;
        VQADD.U32 Q0, Q1, Q2;
        VQADD.U64 Q0, Q1;
        VQADD.U64 Q0, Q1, Q2;
        VQADD.S8  D0, D1;
        VQADD.S8  D0, D1, D2;
        VQADD.S16 D0, D1;
        VQADD.S16 D0, D1, D2;
        VQADD.S32 D0, D1;
        VQADD.S32 D0, D1, D2;
        VQADD.S64 D0, D1;
        VQADD.S64 D0, D1, D2;
        VQADD.U8  D0, D1;
        VQADD.U8  D0, D1, D2;
        VQADD.U16 D0, D1;
        VQADD.U16 D0, D1, D2;
        VQADD.U32 D0, D1;
        VQADD.U32 D0, D1, D2;
        VQADD.U64 D0, D1;
        VQADD.U64 D0, D1, D2;

        // A8.6.358 VQDMLAL, VQDMLSL
        // VQD<op><c><q>.<dt> <Qd>, <Dn>, <Dm>
        // VQD<op><c><q>.<dt> <Qd>, <Dn>, <Dm[x]>
        // op=MLAL, MLSL.  dt=S16, S32

        VQDMLAL.S16 Q0, D1, D2;
        VQDMLAL.S32 Q0, D1, D2;
        VQDMLAL.S16 Q0, D1, D2[0];
        VQDMLAL.S32 Q0, D1, D2[0];

        // A8.6.359 VQDMULH
        // VQDMULH<c><q>.<dt> {<Qd>,} <Qn>, <Qm>             Encoding T1 / A1, Q = 1
        // VQDMULH<c><q>.<dt> {<Dd>,} <Dn>, <Dm>             Encoding T1 / A1, Q = 0
        // VQDMULH<c><q>.<dt> {<Qd>,} <Qn>, <Dm[x]>          Encoding T2 / A2, U = 1
        // VQDMULH<c><q>.<dt> {<Dd>,} <Dn>, <Dm[x]>          Encoding T2 / A2, U = 0
        // dt=S16, S32

        VQDMULH.S16 Q0, Q1;
        VQDMULH.S16 Q0, Q1, Q2;
        VQDMULH.S32 Q0, Q1;
        VQDMULH.S32 Q0, Q1, Q2;
        VQDMULH.S16 D0, D1;
        VQDMULH.S16 D0, D1, D2;
        VQDMULH.S32 D0, D1;
        VQDMULH.S32 D0, D1, D2;
        VQDMULH.S16 Q0, D1[0];
        VQDMULH.S16 Q0, Q1, Q2[0];
        VQDMULH.S32 Q0, D1[0];
        VQDMULH.S32 Q0, Q1, D2[0];
        VQDMULH.S16 D0, D1[0];
        VQDMULH.S16 D0, D1, D2[0];
        VQDMULH.S32 D0, D1[0];
        VQDMULH.S32 D0, D1, D2[0];

        // A8.6.360 VQDMULL
        // VQDMULL<c><q>.<dt> <Qd>, <Dn>, <Dm>
        // VQDMULL<c><q>.<dt> <Qd>, <Dn>, <Dm[x]>
        // dt=S16, S32

        VQDMULL.S16 Q0, D1, D2;
        VQDMULL.S32 Q0, D1, D2;
        VQDMULL.S16 Q0, D1, D2[0];
        VQDMULL.S32 Q0, D1, D2[0];

        // A8.6.361 VQMOVN, VQMOVUN
        // VQMOV{U}N<c><q>.<type><size> <Dd>, <Qm>
        // type=S, U.  size=16, 32, 64

        VQMOVN.S16 D0, Q1;
        VQMOVN.S32 D0, Q1;
        VQMOVN.S64 D0, Q1;
        VQMOVN.U16 D0, Q1;
        VQMOVN.U32 D0, Q1;
        VQMOVN.U64 D0, Q1;
        VQMOVUN.S16 D0, Q1;
        VQMOVUN.S32 D0, Q1;
        VQMOVUN.S64 D0, Q1;
        VQMOVUN.U16 D0, Q1; // should be illegal
        VQMOVUN.U32 D0, Q1; // should be illegal
        VQMOVUN.U64 D0, Q1; // should be illegal

        // A8.6.362 VQNEG
        // VQNEG<c><q>.<dt> <Qd>, <Qm>                       Encoded as Q = 1
        // VQNEG<c><q>.<dt> <Dd>, <Dm>                       Encoded as Q = 0
        // dt=S8, S16, S32

        VQNEG.S8  Q0, Q1;
        VQNEG.S16 Q0, Q1;
        VQNEG.S32 Q0, Q1;
        VQNEG.S8  D0, D1;
        VQNEG.S16 D0, D1;
        VQNEG.S32 D0, D1;

        // A8.6.363 VQRDMULH
        // VQRDMULH<c><q>.<dt> {<Qd>,} <Qn>, <Qm>            Encoding T1 / A1, Q = 1
        // VQRDMULH<c><q>.<dt> {<Dd>,} <Dn>, <Dm>            Encoding T1 / A1, Q = 0
        // VQRDMULH<c><q>.<dt> {<Qd>,} <Qn>, <Dm[x]>         Encoding T2 / A2, Q = 1
        // VQRDMULH<c><q>.<dt> {<Dd>,} <Dn>, <Dm[x]>         Encoding T2 / A2, Q = 0
        // dt=S16, S32

        VQRDMULH.S16 Q0, Q1;
        VQRDMULH.S16 Q0, Q1, Q2;
        VQRDMULH.S32 Q0, Q1;
        VQRDMULH.S32 Q0, Q1, Q2;
        VQRDMULH.S16 D0, D1;
        VQRDMULH.S16 D0, D1, D2;
        VQRDMULH.S32 D0, D1;
        VQRDMULH.S32 D0, D1, D2;
        VQRDMULH.S16 Q0, D1[0];
        VQRDMULH.S16 Q0, Q1, D2[0];
        VQRDMULH.S32 Q0, D1[0];
        VQRDMULH.S32 Q0, Q1, D2[0];
        VQRDMULH.S16 D0, D1[0];
        VQRDMULH.S16 D0, D1, D2[0];
        VQRDMULH.S32 D0, D1[0];
        VQRDMULH.S32 D0, D1, D2[0];

        // A8.6.364 VQRSHL
        // VQRSHL<c><q>.<type><size> {<Qd>,} <Qm>, <Qn> Encoded as Q = 1
        // VQRSHL<c><q>.<type><size> {<Dd>,} <Dm>, <Dn> Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64.

        VQRSHL.S8  Q0, Q1;
        VQRSHL.S8  Q0, Q1, Q2;
        VQRSHL.S16 Q0, Q1;
        VQRSHL.S16 Q0, Q1, Q2;
        VQRSHL.S32 Q0, Q1;
        VQRSHL.S32 Q0, Q1, Q2;
        VQRSHL.S64 Q0, Q1;
        VQRSHL.S64 Q0, Q1, Q2;
        VQRSHL.U8  Q0, Q1;
        VQRSHL.U8  Q0, Q1, Q2;
        VQRSHL.U16 Q0, Q1;
        VQRSHL.U16 Q0, Q1, Q2;
        VQRSHL.U32 Q0, Q1;
        VQRSHL.U32 Q0, Q1, Q2;
        VQRSHL.U64 Q0, Q1;
        VQRSHL.U64 Q0, Q1, Q2;
        VQRSHL.S8  D0, D1;
        VQRSHL.S8  D0, D1, D2;
        VQRSHL.S16 D0, D1;
        VQRSHL.S16 D0, D1, D2;
        VQRSHL.S32 D0, D1;
        VQRSHL.S32 D0, D1, D2;
        VQRSHL.S64 D0, D1;
        VQRSHL.S64 D0, D1, D2;
        VQRSHL.U8  D0, D1;
        VQRSHL.U8  D0, D1, D2;
        VQRSHL.U16 D0, D1;
        VQRSHL.U16 D0, D1, D2;
        VQRSHL.U32 D0, D1;
        VQRSHL.U32 D0, D1, D2;
        VQRSHL.U64 D0, D1;
        VQRSHL.U64 D0, D1, D2;

        // A8.6.365 VQRSHRN, VQRSHRUN
        // VQRSHR{U}N<c><q>.<type><size> <Dd>, <Qm>, #<imm>
        // type=S, U.  size=16, 32, 64

        VQRSHRN.S16 D0, Q1, #2;
        VQRSHRN.S32 D0, Q1, #2;
        VQRSHRN.S64 D0, Q1, #2;
        VQRSHRN.U16 D0, Q1, #2;
        VQRSHRN.U32 D0, Q1, #2;
        VQRSHRN.U64 D0, Q1, #2;
        VQRSHRUN.S16 D0, Q1, #2;
        VQRSHRUN.S32 D0, Q1, #2;
        VQRSHRUN.S64 D0, Q1, #2;
        VQRSHRUN.U16 D0, Q1, #2; // should be illegal
        VQRSHRUN.U32 D0, Q1, #2; // should be illegal
        VQRSHRUN.U64 D0, Q1, #2; // should be illegal

        // A8.6.366 VQSHL (register)
        // VQSHL<c><q>.<type><size> {<Qd>,} <Qm>, <Qn>       Encoded as Q = 1
        // VQSHL<c><q>.<type><size> {<Dd>,} <Dm>, <Dn>       Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64

        VQSHL.S8  Q0, Q1;
        VQSHL.S8  Q0, Q1, Q2;
        VQSHL.S16 Q0, Q1;
        VQSHL.S16 Q0, Q1, Q2;
        VQSHL.S32 Q0, Q1;
        VQSHL.S32 Q0, Q1, Q2;
        VQSHL.S64 Q0, Q1;
        VQSHL.S64 Q0, Q1, Q2;
        VQSHL.U8  Q0, Q1;
        VQSHL.U8  Q0, Q1, Q2;
        VQSHL.U16 Q0, Q1;
        VQSHL.U16 Q0, Q1, Q2;
        VQSHL.U32 Q0, Q1;
        VQSHL.U32 Q0, Q1, Q2;
        VQSHL.U64 Q0, Q1;
        VQSHL.U64 Q0, Q1, Q2;
        VQSHL.S8  D0, D1;
        VQSHL.S8  D0, D1, D2;
        VQSHL.S16 D0, D1;
        VQSHL.S16 D0, D1, D2;
        VQSHL.S32 D0, D1;
        VQSHL.S32 D0, D1, D2;
        VQSHL.S64 D0, D1;
        VQSHL.S64 D0, D1, D2;
        VQSHL.U8  D0, D1;
        VQSHL.U8  D0, D1, D2;
        VQSHL.U16 D0, D1;
        VQSHL.U16 D0, D1, D2;
        VQSHL.U32 D0, D1;
        VQSHL.U32 D0, D1, D2;
        VQSHL.U64 D0, D1;
        VQSHL.U64 D0, D1, D2;

        // A8.6.367 VQSHL, VQSHLU (immediate)
        // VQSHL{U}<c><q>.<type><size> {<Qd>,} <Qm>, #<imm>  Encoded as Q = 1
        // VQSHL{U}<c><q>.<type><size> {<Dd>,} <Dm>, #<imm>  Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64

        VQSHL.S8  Q0, #1;
        VQSHL.S8  Q0, Q1, #2;
        VQSHL.S16 Q0, #1;
        VQSHL.S16 Q0, Q1, #2;
        VQSHL.S32 Q0, #1;
        VQSHL.S32 Q0, Q1, #2;
        VQSHL.S64 Q0, #1;
        VQSHL.S64 Q0, Q1, #2;
        VQSHL.U8  Q0, #1;
        VQSHL.U8  Q0, Q1, #2;
        VQSHL.U16 Q0, #1;
        VQSHL.U16 Q0, Q1, #2;
        VQSHL.U32 Q0, #1;
        VQSHL.U32 Q0, Q1, #2;
        VQSHL.U64 Q0, #1;
        VQSHL.U64 Q0, Q1, #2;
        VQSHL.S8  D0, #1;
        VQSHL.S8  D0, D1, #2;
        VQSHL.S16 D0, #1;
        VQSHL.S16 D0, D1, #2;
        VQSHL.S32 D0, #1;
        VQSHL.S32 D0, D1, #2;
        VQSHL.S64 D0, #1;
        VQSHL.S64 D0, D1, #2;
        VQSHL.U8  D0, #1;
        VQSHL.U8  D0, D1, #2;
        VQSHL.U16 D0, #1;
        VQSHL.U16 D0, D1, #2;
        VQSHL.U32 D0, #1;
        VQSHL.U32 D0, D1, #2;
        VQSHL.U64 D0, #1;
        VQSHL.U64 D0, D1, #2;
        VQSHLU.S8  Q0, #1;
        VQSHLU.S8  Q0, Q1, #2;
        VQSHLU.S16 Q0, #1;
        VQSHLU.S16 Q0, Q1, #2;
        VQSHLU.S32 Q0, #1;
        VQSHLU.S32 Q0, Q1, #2;
        VQSHLU.S64 Q0, #1;
        VQSHLU.S64 Q0, Q1, #2;
        VQSHLU.U8  Q0, #1;     // should be illegal
        VQSHLU.U8  Q0, Q1, #2; // should be illegal
        VQSHLU.U16 Q0, #1;     // should be illegal
        VQSHLU.U16 Q0, Q1, #2; // should be illegal
        VQSHLU.U32 Q0, #1;     // should be illegal
        VQSHLU.U32 Q0, Q1, #2; // should be illegal
        VQSHLU.U64 Q0, #1;     // should be illegal
        VQSHLU.U64 Q0, Q1, #2; // should be illegal
        VQSHLU.S8  D0, #1;
        VQSHLU.S8  D0, D1, #2;
        VQSHLU.S16 D0, #1;
        VQSHLU.S16 D0, D1, #2;
        VQSHLU.S32 D0, #1;
        VQSHLU.S32 D0, D1, #2;
        VQSHLU.S64 D0, #1;
        VQSHLU.S64 D0, D1, #2;
        VQSHLU.U8  D0, #1;     // should be illegal
        VQSHLU.U8  D0, D1, #2; // should be illegal
        VQSHLU.U16 D0, #1;     // should be illegal
        VQSHLU.U16 D0, D1, #2; // should be illegal
        VQSHLU.U32 D0, #1;     // should be illegal
        VQSHLU.U32 D0, D1, #2; // should be illegal
        VQSHLU.U64 D0, #1;     // should be illegal
        VQSHLU.U64 D0, D1, #2; // should be illegal

        // A8.6.368 VQSHRN, VQSHRUN
        // VQSHR{U}N<c><q>.<type><size> <Dd>, <Qm>, #<imm>
        // type=S, U.  size=16, 32, 64

        VQSHRN.S16 D0, Q1, #3;
        VQSHRN.S32 D0, Q1, #3;
        VQSHRN.S64 D0, Q1, #3;
        VQSHRN.U16 D0, Q1, #3;
        VQSHRN.U32 D0, Q1, #3;
        VQSHRN.U64 D0, Q1, #3;
        VQSHRUN.S16 D0, Q1, #3;
        VQSHRUN.S32 D0, Q1, #3;
        VQSHRUN.S64 D0, Q1, #3;
        VQSHRUN.U16 D0, Q1, #3; // should be illegal
        VQSHRUN.U32 D0, Q1, #3; // should be illegal
        VQSHRUN.U64 D0, Q1, #3; // should be illegal

        // A8.6.369 VQSUB
        // VQSUB<c><q>.<type><size> {<Qd>,} <Qn>, <Qm>       Encoded as Q = 1
        // VQSUB<c><q>.<type><size> {<Dd>,} <Dn>, <Dm>       Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64.

        VQSUB.S8  Q0, Q1;
        VQSUB.S8  Q0, Q1, Q2;
        VQSUB.S16 Q0, Q1;
        VQSUB.S16 Q0, Q1, Q2;
        VQSUB.S32 Q0, Q1;
        VQSUB.S32 Q0, Q1, Q2;
        VQSUB.S64 Q0, Q1;
        VQSUB.S64 Q0, Q1, Q2;
        VQSUB.U8  Q0, Q1;
        VQSUB.U8  Q0, Q1, Q2;
        VQSUB.U16 Q0, Q1;
        VQSUB.U16 Q0, Q1, Q2;
        VQSUB.U32 Q0, Q1;
        VQSUB.U32 Q0, Q1, Q2;
        VQSUB.U64 Q0, Q1;
        VQSUB.U64 Q0, Q1, Q2;
        VQSUB.S8  D0, D1;
        VQSUB.S8  D0, D1, D2;
        VQSUB.S16 D0, D1;
        VQSUB.S16 D0, D1, D2;
        VQSUB.S32 D0, D1;
        VQSUB.S32 D0, D1, D2;
        VQSUB.S64 D0, D1;
        VQSUB.S64 D0, D1, D2;
        VQSUB.U8  D0, D1;
        VQSUB.U8  D0, D1, D2;
        VQSUB.U16 D0, D1;
        VQSUB.U16 D0, D1, D2;
        VQSUB.U32 D0, D1;
        VQSUB.U32 D0, D1, D2;
        VQSUB.U64 D0, D1;
        VQSUB.U64 D0, D1, D2;

        // A8.6.370 VRADDHN
        // VRADDHN<c><q>.<dt> <Dd>, <Qn>, <Qm>
        // dt=I16, I32, I64

        VRADDHN.I16 D0, Q1, Q2;
        VRADDHN.I32 D0, Q1, Q2;
        VRADDHN.I64 D0, Q1, Q2;

        // A8.6.371 VRECPE
        // VRECPE<c><q>.<dt> <Qd>, <Qm>                      Encoded as Q = 1
        // VRECPE<c><q>.<dt> <Dd>, <Dm>                      Encoded as Q = 0
        // dt=U32, F32

        VRECRE.U32 Q0, Q1;
        VRECPE.F32 Q0, Q1;
        VRECRE.U32 D0, D1;
        VRECPE.F32 D0, D1;

        // A8.6.372 VRECPS
        // VRECPS<c><q>.F32 {<Qd>,} <Qn>, <Qm>               Encoded as Q = 1
        // VRECPS<c><q>.F32 {<Dd>,} <Dn>, <Dm>               Encoded as Q = 0

        VRECPS.F32 Q0, Q1;
        VRECPS.F32 Q0, Q1, Q2;
        VRECPS.F32 D0, D1;
        VRECPS.F32 D0, D1, D2;

        // A8.6.373 VREV16, VREV32, VREV64
        // VREV<n><c><q>.<size> <Qd>, <Qm>                   Encoded as Q = 1
        // VREV<n><c><q>.<size> <Dd>, <Dm>                   Encoded as Q = 0
        // n=16, 32, 64.  size=8, 16, 32

        VREV16.8  Q0, Q1;
        VREV16.16 Q0, Q1; // should be illegal
        VREV16.32 Q0, Q1; // should be illegal
        VREV32.8  Q0, Q1;
        VREV32.16 Q0, Q1;
        VREV32.32 Q0, Q1; // should be illegal
        VREV64.8  Q0, Q1;
        VREV64.16 Q0, Q1;
        VREV64.32 Q0, Q1;
        VREV16.8  D0, D1;
        VREV16.16 D0, D1; // should be illegal
        VREV16.32 D0, D1; // should be illegal
        VREV32.8  D0, D1;
        VREV32.16 D0, D1;
        VREV32.32 D0, D1; // should be illegal
        VREV64.8  D0, D1;
        VREV64.16 D0, D1;
        VREV64.32 D0, D1;

        // A8.6.374 VRHADD
        // VRHADD<c><q>.<dt> {<Qd>,} <Qn>, <Qm>              Encoded as Q = 1
        // VRHADD<c><q>.<dt> {<Dd>,} <Dn>, <Dm>              Encoded as Q = 0
        // dt=S8, S16, S32, U8, U16, U32

        VRHADD.S8  Q0, Q1;
        VRHADD.S8  Q0, Q1, Q2;
        VRHADD.S16 Q0, Q1;
        VRHADD.S16 Q0, Q1, Q2;
        VRHADD.S32 Q0, Q1;
        VRHADD.S32 Q0, Q1, Q2;
        VRHADD.U8  Q0, Q1;
        VRHADD.U8  Q0, Q1, Q2;
        VRHADD.U16 Q0, Q1;
        VRHADD.U16 Q0, Q1, Q2;
        VRHADD.U32 Q0, Q1;
        VRHADD.U32 Q0, Q1, Q2;
        VRHADD.S8  D0, D1;
        VRHADD.S8  D0, D1, D2;
        VRHADD.S16 D0, D1;
        VRHADD.S16 D0, D1, D2;
        VRHADD.S32 D0, D1;
        VRHADD.S32 D0, D1, D2;
        VRHADD.U8  D0, D1;
        VRHADD.U8  D0, D1, D2;
        VRHADD.U16 D0, D1;
        VRHADD.U16 D0, D1, D2;
        VRHADD.U32 D0, D1;
        VRHADD.U32 D0, D1, D2;

        // A8.6.375 VRSHL
        // VRSHL<c><q>.<type><size> {<Qd>,} <Qm>, <Qn>       Encoded as Q = 1
        // VRSHL<c><q>.<type><size> {<Dd>,} <Dm>, <Dn>       Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64.

        VRSHL.S8  Q0, Q1;
        VRSHL.S8  Q0, Q1, Q2;
        VRSHL.S16 Q0, Q1;
        VRSHL.S16 Q0, Q1, Q2;
        VRSHL.S32 Q0, Q1;
        VRSHL.S32 Q0, Q1, Q2;
        VRSHL.S64 Q0, Q1;
        VRSHL.S64 Q0, Q1, Q2;
        VRSHL.U8  Q0, Q1;
        VRSHL.U8  Q0, Q1, Q2;
        VRSHL.U16 Q0, Q1;
        VRSHL.U16 Q0, Q1, Q2;
        VRSHL.U32 Q0, Q1;
        VRSHL.U32 Q0, Q1, Q2;
        VRSHL.U64 Q0, Q1;
        VRSHL.U64 Q0, Q1, Q2;
        VRSHL.S8  D0, D1;
        VRSHL.S8  D0, D1, D2;
        VRSHL.S16 D0, D1;
        VRSHL.S16 D0, D1, D2;
        VRSHL.S32 D0, D1;
        VRSHL.S32 D0, D1, D2;
        VRSHL.S64 D0, D1;
        VRSHL.S64 D0, D1, D2;
        VRSHL.U8  D0, D1;
        VRSHL.U8  D0, D1, D2;
        VRSHL.U16 D0, D1;
        VRSHL.U16 D0, D1, D2;
        VRSHL.U32 D0, D1;
        VRSHL.U32 D0, D1, D2;
        VRSHL.U64 D0, D1;
        VRSHL.U64 D0, D1, D2;

        // A8.6.376 VRSHR
        // VRSHR<c><q>.<type><size> {<Qd>,} <Qm>, #<imm>     Encoded as Q = 1
        // VRSHR<c><q>.<type><size> {<Dd>,} <Dm>, #<imm>     Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64.

        VRSHR.S8  Q0, #1;
        VRSHR.S8  Q0, Q1, #2;
        VRSHR.S16 Q0, #1;
        VRSHR.S16 Q0, Q1, #2;
        VRSHR.S32 Q0, #1;
        VRSHR.S32 Q0, Q1, #2;
        VRSHR.S64 Q0, #1;
        VRSHR.S64 Q0, Q1, #2;
        VRSHR.U8  Q0, #1;
        VRSHR.U8  Q0, Q1, #2;
        VRSHR.U16 Q0, #1;
        VRSHR.U16 Q0, Q1, #2;
        VRSHR.U32 Q0, #1;
        VRSHR.U32 Q0, Q1, #2;
        VRSHR.U64 Q0, #1;
        VRSHR.U64 Q0, Q1, #2;
        VRSHR.S8  D0, #1;
        VRSHR.S8  D0, D1, #2;
        VRSHR.S16 D0, #1;
        VRSHR.S16 D0, D1, #2;
        VRSHR.S32 D0, #1;
        VRSHR.S32 D0, D1, #2;
        VRSHR.S64 D0, #1;
        VRSHR.S64 D0, D1, #2;
        VRSHR.U8  D0, #1;
        VRSHR.U8  D0, D1, #2;
        VRSHR.U16 D0, #1;
        VRSHR.U16 D0, D1, #2;
        VRSHR.U32 D0, #1;
        VRSHR.U32 D0, D1, #2;
        VRSHR.U64 D0, #1;
        VRSHR.U64 D0, D1, #2;

        // A8.6.377 VRSHRN
        // VRSHRN<c><q>.I<size> <Dd>, <Qm>, #<imm>
        // size=16, 32, 64

        VRSHRN.I16 D0, Q1, #2;
        VRSHRN.I32 D0, Q1, #2;
        VRSHRN.I64 D0, Q1, #2;

        // A8.6.378 VRSQRTE
        // VRSQRTE<c><q>.<dt> <Qd>, <Qm>                     Encoded as Q = 1
        // VRSQRTE<c><q>.<dt> <Dd>, <Dm>                     Encoded as Q = 0
        // dt=U32, F32

        VRSQRTE.U32 Q0, Q1;
        VRSQRTE.F32 Q0, Q1;
        VRSQRTE.U32 D0, D1;
        VRSQRTE.F32 D0, D1;

        // A8.6.379 VRSQRTS
        // VRSQRTS<c><q>.F32 {<Qd>,} <Qn>, <Qm>              Encoded as Q = 1, sz = 0
        // VRSQRTS<c><q>.F32 {<Dd>,} <Dn>, <Dm>              Encoded as Q = 0, sz = 0

        VRSQRTS.F32 Q0, Q1;
        VRSQRTS.F32 Q0, Q1, Q2;
        VRSQRTS.F32 D0, D1;
        VRSQRTS.F32 D0, D1, D2;

        // A8.6.380 VRSRA
        // VRSRA<c><q>.<type><size> {<Qd>,} <Qm>, #<imm> Encoded as Q = 1
        // VRSRA<c><q>.<type><size> {<Dd>,} <Dm>, #<imm> Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64.

        VRSRA.S8  Q0, #1;
        VRSRA.S8  Q0, Q1, #1;
        VRSRA.S16 Q0, #1;
        VRSRA.S16 Q0, Q1, #1;
        VRSRA.S32 Q0, #1;
        VRSRA.S32 Q0, Q1, #1;
        VRSRA.S64 Q0, #1;
        VRSRA.S64 Q0, Q1, #1;
        VRSRA.U8  Q0, #1;
        VRSRA.U8  Q0, Q1, #1;
        VRSRA.U16 Q0, #1;
        VRSRA.U16 Q0, Q1, #1;
        VRSRA.U32 Q0, #1;
        VRSRA.U32 Q0, Q1, #1;
        VRSRA.U64 Q0, #1;
        VRSRA.U64 Q0, Q1, #1;
        VRSRA.S8  D0, #1;
        VRSRA.S8  D0, D1, #1;
        VRSRA.S16 D0, #1;
        VRSRA.S16 D0, D1, #1;
        VRSRA.S32 D0, #1;
        VRSRA.S32 D0, D1, #1;
        VRSRA.S64 D0, #1;
        VRSRA.S64 D0, D1, #1;
        VRSRA.U8  D0, #1;
        VRSRA.U8  D0, D1, #1;
        VRSRA.U16 D0, #1;
        VRSRA.U16 D0, D1, #1;
        VRSRA.U32 D0, #1;
        VRSRA.U32 D0, D1, #1;
        VRSRA.U64 D0, #1;
        VRSRA.U64 D0, D1, #1;

        // A8.6.381 VRSUBHN
        // VRSUBHN<c><q>.<dt> <Dd>, <Qn>, <Qm>
        // dt=I16, I32, I64

        VRSUBHN.I16 D0, Q1, Q2;
        VRSUBHN.I32 D0, Q1, Q2;
        VRSUBHN.I64 D0, Q1, Q2;

        // A8.6.382 VSHL (immediate)
        // VSHL<c><q>.I<size> {<Qd>,} <Qm>, #<imm>           Encoded as Q = 1
        // VSHL<c><q>.I<size> {<Dd>,} <Dm>, #<imm>           Encoded as Q = 0
        // size=8, 16, 32, 64

        VSHL.I8  Q0, #1;
        VSHL.I8  Q0, Q1, #2;
        VSHL.I16 Q0, #1;
        VSHL.I16 Q0, Q1, #2;
        VSHL.I32 Q0, #1;
        VSHL.I32 Q0, Q1, #2;
        VSHL.I64 Q0, #1;
        VSHL.I64 Q0, Q1, #2;
        VSHL.I8  D0, #1;
        VSHL.I8  D0, D1, #2;
        VSHL.I16 D0, #1;
        VSHL.I16 D0, D1, #2;
        VSHL.I32 D0, #1;
        VSHL.I32 D0, D1, #2;
        VSHL.I64 D0, #1;
        VSHL.I64 D0, D1, #2;

        // A8.6.383 VSHL (register)
        // VSHL<c><q>.<type><size> {<Qd>,} <Qm>, <Qn>        Encoded as Q = 1
        // VSHL<c><q>.<type><size> {<Dd>,} <Dm>, <Dn>        Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64.

        VSHL.S8  Q0, Q1;
        VSHL.S8  Q0, Q1, Q2;
        VSHL.S16 Q0, Q1;
        VSHL.S16 Q0, Q1, Q2;
        VSHL.S32 Q0, Q1;
        VSHL.S32 Q0, Q1, Q2;
        VSHL.S64 Q0, Q1;
        VSHL.S64 Q0, Q1, Q2;
        VSHL.U8  Q0, Q1;
        VSHL.U8  Q0, Q1, Q2;
        VSHL.U16 Q0, Q1;
        VSHL.U16 Q0, Q1, Q2;
        VSHL.U32 Q0, Q1;
        VSHL.U32 Q0, Q1, Q2;
        VSHL.U64 Q0, Q1;
        VSHL.U64 Q0, Q1, Q2;
        VSHL.S8  D0, D1;
        VSHL.S8  D0, D1, D2;
        VSHL.S16 D0, D1;
        VSHL.S16 D0, D1, D2;
        VSHL.S32 D0, D1;
        VSHL.S32 D0, D1, D2;
        VSHL.S64 D0, D1;
        VSHL.S64 D0, D1, D2;
        VSHL.U8  D0, D1;
        VSHL.U8  D0, D1, D2;
        VSHL.U16 D0, D1;
        VSHL.U16 D0, D1, D2;
        VSHL.U32 D0, D1;
        VSHL.U32 D0, D1, D2;
        VSHL.U64 D0, D1;
        VSHL.U64 D0, D1, D2;

        // A8.6.384 VSHLL
        // VSHLL<c><q>.<type><size> <Qd>, <Dm>, #<imm>
        // type=S, U, I.  size=8, 16, 32.

        VSHLL.S8  R0, D1, #2;
        VSHLL.S16 R0, D1, #2;
        VSHLL.S32 R0, D1, #2;
        VSHLL.U8  R0, D1, #2;
        VSHLL.U16 R0, D1, #2;
        VSHLL.U32 R0, D1, #2;
        VSHLL.I8  R0, D1, #2;
        VSHLL.I16 R0, D1, #2;
        VSHLL.I32 R0, D1, #2;

        // A8.6.385 VSHR
        // VSHR<c><q>.<type><size> {<Qd>,} <Qm>, #<imm>      Encoded as Q = 1
        // VSHR<c><q>.<type><size> {<Dd>,} <Dm>, #<imm>      Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64.A

        VSHR.S8  Q0, #1;
        VSHR.S8  Q0, Q1, #2;
        VSHR.S16 Q0, #1;
        VSHR.S16 Q0, Q1, #2;
        VSHR.S32 Q0, #1;
        VSHR.S32 Q0, Q1, #2;
        VSHR.S64 Q0, #1;
        VSHR.S64 Q0, Q1, #2;
        VSHR.U8  Q0, #1;
        VSHR.U8  Q0, Q1, #2;
        VSHR.U16 Q0, #1;
        VSHR.U16 Q0, Q1, #2;
        VSHR.U32 Q0, #1;
        VSHR.U32 Q0, Q1, #2;
        VSHR.U64 Q0, #1;
        VSHR.U64 Q0, Q1, #2;
        VSHR.S8  D0, #1;
        VSHR.S8  D0, D1, #2;
        VSHR.S16 D0, #1;
        VSHR.S16 D0, D1, #2;
        VSHR.S32 D0, #1;
        VSHR.S32 D0, D1, #2;
        VSHR.S64 D0, #1;
        VSHR.S64 D0, D1, #2;
        VSHR.U8  D0, #1;
        VSHR.U8  D0, D1, #2;
        VSHR.U16 D0, #1;
        VSHR.U16 D0, D1, #2;
        VSHR.U32 D0, #1;
        VSHR.U32 D0, D1, #2;
        VSHR.U64 D0, #1;
        VSHR.U64 D0, D1, #2;

        // A8.6.386 VSHRN
        // VSHRN<c><q>.I<size> <Dd>, <Qm>, #<imm>
        // size=16, 32, 64

        VSHRN.I16 D0, Q1, #2;
        VSHRN.I32 D0, Q1, #2;
        VSHRN.I64 D0, Q1, #2;

        // A8.6.387 VSLI
        // VSLI<c><q>.<size> {<Qd>,} <Qm>, #<imm>            Encoded as Q = 1
        // VSLI<c><q>.<size> {<Dd>,} <Dm>, #<imm>            Encoded as Q = 0
        // size=8, 16, 32, 64

        VSHI.8  Q0, #1;
        VSHI.8  Q0, Q1, #2;
        VSHI.16 Q0, #1;
        VSHI.16 Q0, Q1, #2;
        VSHI.32 Q0, #1;
        VSHI.32 Q0, Q1, #2;
        VSHI.64 Q0, #1;
        VSHI.64 Q0, Q1, #2;
        VSHI.8  D0, #1;
        VSHI.8  D0, D1, #2;
        VSHI.16 D0, #1;
        VSHI.16 D0, D1, #2;
        VSHI.32 D0, #1;
        VSHI.32 D0, D1, #2;
        VSHI.64 D0, #1;
        VSHI.64 D0, D1, #2;

        // A8.6.388 VSQRT
        // VSQRT<c><q>.F64 <Dd>, <Dm>                        Encoded as sz = 1
        // VSQRT<c><q>.F32 <Sd>, <Sm>                        Encoded as sz = 0

        VSQRT.F64 D0, D1;
        VSQRT.F32 S0, S1;

        // A8.6.389 VSRA
        // VSRA<c><q>.<type><size> {<Qd>,} <Qm>, #<imm>      Encoded as Q = 1
        // VSRA<c><q>.<type><size> {<Dd>,} <Dm>, #<imm>      Encoded as Q = 0
        // type=S, U.  size=8, 16, 32, 64

        VSRA.S8  Q0, #1;
        VSRA.S8  Q0, Q1, #2;
        VSRA.S16 Q0, #1;
        VSRA.S16 Q0, Q1, #2;
        VSRA.S32 Q0, #1;
        VSRA.S32 Q0, Q1, #2;
        VSRA.S64 Q0, #1;
        VSRA.S64 Q0, Q1, #2;
        VSRA.U8  Q0, #1;
        VSRA.U8  Q0, Q1, #2;
        VSRA.U16 Q0, #1;
        VSRA.U16 Q0, Q1, #2;
        VSRA.U32 Q0, #1;
        VSRA.U32 Q0, Q1, #2;
        VSRA.U64 Q0, #1;
        VSRA.U64 Q0, Q1, #2;
        VSRA.S8  D0, #1;
        VSRA.S8  D0, D1, #2;
        VSRA.S16 D0, #1;
        VSRA.S16 D0, D1, #2;
        VSRA.S32 D0, #1;
        VSRA.S32 D0, D1, #2;
        VSRA.S64 D0, #1;
        VSRA.S64 D0, D1, #2;
        VSRA.U8  D0, #1;
        VSRA.U8  D0, D1, #2;
        VSRA.U16 D0, #1;
        VSRA.U16 D0, D1, #2;
        VSRA.U32 D0, #1;
        VSRA.U32 D0, D1, #2;
        VSRA.U64 D0, #1;
        VSRA.U64 D0, D1, #2;

        // A8.6.390 VSRI
        // VSRI<c><q>.<size> {<Qd>,} <Qm>, #<imm>            Encoded as Q = 1
        // VSRI<c><q>.<size> {<Dd>,} <Dm>, #<imm>            Encoded as Q = 0
        // size=8, 16, 32, 64.

        VSRI.8  Q0, #1;
        VSRI.8  Q0, Q1, #2;
        VSRI.16 Q0, #1;
        VSRI.16 Q0, Q1, #2;
        VSRI.32 Q0, #1;
        VSRI.32 Q0, Q1, #2;
        VSRI.64 Q0, #1;
        VSRI.64 Q0, Q1, #2;
        VSRI.8  D0, #1;
        VSRI.8  D0, D1, #2;
        VSRI.16 D0, #1;
        VSRI.16 D0, D1, #2;
        VSRI.32 D0, #1;
        VSRI.32 D0, D1, #2;
        VSRI.64 D0, #1;
        VSRI.64 D0, D1, #2;

        // A8.6.391 VST1 (multiple single elements)
        // VST1<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VST1<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VST1<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32, 64.  list={ Dd+0..3 } 1 to 4 elements.  align=omitted, 64, 128, 256
        // TODO: needs @align

        // stress the list parser:
        VST1.8  { D0 }, [R1];
        VST1.8  { D0-D3 }, [R4];
        VST1.8  { D0-D1, D2 }, [R4];
        VST1.8  { D0-D1, D2-D3 }, [R4];

        // now the normal tests
        VST1.8  { D0 }, [R1];
        VST1.8  { D0, D1 }, [R2];
        VST1.8  { D0, D1, D2 }, [R3];
        VST1.8  { D0, D1, D2, D3 }, [R4];
        VST1.16 { D0 }, [R1];
        VST1.16 { D0, D1 }, [R2];
        VST1.16 { D0, D1, D2 }, [R3];
        VST1.16 { D0, D1, D2, D3 }, [R4];
        VST1.32 { D0 }, [R1];
        VST1.32 { D0, D1 }, [R2];
        VST1.32 { D0, D1, D2 }, [R3];
        VST1.32 { D0, D1, D2, D3 }, [R4];
        VST1.64 { D0 }, [R1];
        VST1.64 { D0, D1 }, [R2];
        VST1.64 { D0, D1, D2 }, [R3];
        VST1.64 { D0, D1, D2, D3 }, [R4];
        VST1.8  { D0 }, [R1]!;
        VST1.8  { D0, D1 }, [R2]!;
        VST1.8  { D0, D1, D2 }, [R3]!;
        VST1.8  { D0, D1, D2, D3 }, [R4]!;
        VST1.16 { D0 }, [R1]!;
        VST1.16 { D0, D1 }, [R2]!;
        VST1.16 { D0, D1, D2 }, [R3]!;
        VST1.16 { D0, D1, D2, D3 }, [R4]!;
        VST1.32 { D0 }, [R1]!;
        VST1.32 { D0, D1 }, [R2]!;
        VST1.32 { D0, D1, D2 }, [R3]!;
        VST1.32 { D0, D1, D2, D3 }, [R4]!;
        VST1.64 { D0 }, [R1]!;
        VST1.64 { D0, D1 }, [R2]!;
        VST1.64 { D0, D1, D2 }, [R3]!;
        VST1.64 { D0, D1, D2, D3 }, [R4]!;
        VST1.8  { D0 }, [R1], R2;
        VST1.8  { D0, D1 }, [R2], R3;
        VST1.8  { D0, D1, D2 }, [R3], R4;
        VST1.8  { D0, D1, D2, D3 }, [R4], R5;
        VST1.16 { D0 }, [R1], R2;
        VST1.16 { D0, D1 }, [R2], R3;
        VST1.16 { D0, D1, D2 }, [R3], R4;
        VST1.16 { D0, D1, D2, D3 }, [R4], R5;
        VST1.32 { D0 }, [R1], R2;
        VST1.32 { D0, D1 }, [R2], R3;
        VST1.32 { D0, D1, D2 }, [R3], R4;
        VST1.32 { D0, D1, D2, D3 }, [R4], R5;
        VST1.64 { D0 }, [R1], R2;
        VST1.64 { D0, D1 }, [R2], R3;
        VST1.64 { D0, D1, D2 }, [R3], R4;
        VST1.64 { D0, D1, D2, D3 }, [R4], R5;

        // A8.6.392 VST1 (single element from one lane)
        // VST1<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VST1<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VST1<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd[x] }.  align=omitted, 16, 32.
        // TODO: needs @align

        VST1.8  { D0[1] }, [R1];
        VST1.8  { D0[1], D1[1] }, [R2];
        VST1.8  { D0[1], D1[1], D2[1] }, [R3];
        VST1.8  { D0[1], D1[1], D2[1], D3[1] }, [R4];
        VST1.16 { D0[1] }, [R1];
        VST1.16 { D0[1], D1[1] }, [R2];
        VST1.16 { D0[1], D1[1], D2[1] }, [R3];
        VST1.16 { D0[1], D1[1], D2[1], D3[1] }, [R4];
        VST1.32 { D0[1] }, [R1];
        VST1.32 { D0[1], D1[1] }, [R2];
        VST1.32 { D0[1], D1[1], D2[1] }, [R3];
        VST1.32 { D0[1], D1[1], D2[1], D3[1] }, [R4];
        VST1.8  { D0[1] }, [R1]!;
        VST1.8  { D0[1], D1[1] }, [R2]!;
        VST1.8  { D0[1], D1[1], D2[1] }, [R3]!;
        VST1.8  { D0[1], D1[1], D2[1], D3[1] }, [R4]!;
        VST1.16 { D0[1] }, [R1]!;
        VST1.16 { D0[1], D1[1] }, [R2]!;
        VST1.16 { D0[1], D1[1], D2[1] }, [R3]!;
        VST1.16 { D0[1], D1[1], D2[1], D3[1] }, [R4]!;
        VST1.32 { D0[1] }, [R1]!;
        VST1.32 { D0[1], D1[1] }, [R2]!;
        VST1.32 { D0[1], D1[1], D2[1] }, [R3]!;
        VST1.32 { D0[1], D1[1], D2[1], D3[1] }, [R4]!;
        VST1.8  { D0[1] }, [R1], R2;
        VST1.8  { D0[1], D1[1] }, [R2], R3;
        VST1.8  { D0[1], D1[1], D2[1] }, [R3], R4;
        VST1.8  { D0[1], D1[1], D2[1], D3[1] }, [R4], R5;
        VST1.16 { D0[1] }, [R1], R2;
        VST1.16 { D0[1], D1[1] }, [R2], R3;
        VST1.16 { D0[1], D1[1], D2[1] }, [R3], R4;
        VST1.16 { D0[1], D1[1], D2[1], D3[1] }, [R4], R5;
        VST1.32 { D0[1] }, [R1], R2;
        VST1.32 { D0[1], D1[1] }, [R2], R3;
        VST1.32 { D0[1], D1[1], D2[1] }, [R3], R4;
        VST1.32 { D0[1], D1[1], D2[1], D3[1] }, [R4], R5;

        // A8.6.393 VST2 (multiple 2-element structures)
        // VST2<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VST2<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VST2<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd, Dd+1 }, { Dd, Dd+2 }, { Dd, Dd+1, Dd+2, Dd+3 }.  align=omitted, 64, 128, 256
        // TODO: needs @align

        VST2.8  { D0, D1 }, [R2];
        VST2.8  { D0, D2 }, [R3];
        VST2.8  { D0, D1, D2, D3 }, [R4];
        VST2.16 { D0, D1 }, [R2];
        VST2.16 { D0, D2 }, [R3];
        VST2.16 { D0, D1, D2, D3 }, [R4];
        VST2.32 { D0, D1 }, [R2];
        VST2.32 { D0, D2 }, [R3];
        VST2.32 { D0, D1, D2, D3 }, [R4];
        VST2.8  { D0, D1 }, [R2]!;
        VST2.8  { D0, D2 }, [R3]!;
        VST2.8  { D0, D1, D2, D3 }, [R4]!;
        VST2.16 { D0, D1 }, [R2]!;
        VST2.16 { D0, D2 }, [R3]!;
        VST2.16 { D0, D1, D2, D3 }, [R4]!;
        VST2.32 { D0, D1 }, [R2]!;
        VST2.32 { D0, D2 }, [R3]!;
        VST2.32 { D0, D1, D2, D3 }, [R4]!;
        VST2.8  { D0, D1 }, [R2], R3;
        VST2.8  { D0, D2 }, [R3], R4;
        VST2.8  { D0, D1, D2, D3 }, [R4], R5;
        VST2.16 { D0, D1 }, [R2], R3;
        VST2.16 { D0, D2 }, [R3], R4;
        VST2.16 { D0, D1, D2, D3 }, [R4], R5;
        VST2.32 { D0, D1 }, [R2], R3;
        VST2.32 { D0, D2 }, [R3], R4;
        VST2.32 { D0, D1, D2, D3 }, [R4], R5;

        // A8.6.394 VST2 (single 2-element structure from one lane)
        // VST2<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VST2<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VST2<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd[x], Dd+1[x] }, { Dd[x], Dd+2[x] }.  align=omitted, 64, 128, 256.
        // TODO: needs @align

        VST2.8  { D0[1], D1[1] }, [R2];
        VST2.8  { D0[1], D2[1] }, [R3];
        VST2.16 { D0[1], D1[1] }, [R2];
        VST2.16 { D0[1], D2[1] }, [R3];
        VST2.32 { D0[1], D1[1] }, [R2];
        VST2.32 { D0[1], D2[1] }, [R3];
        VST2.8  { D0[1], D1[1] }, [R2]!;
        VST2.8  { D0[1], D2[1] }, [R3]!;
        VST2.16 { D0[1], D1[1] }, [R2]!;
        VST2.16 { D0[1], D2[1] }, [R3]!;
        VST2.32 { D0[1], D1[1] }, [R2]!;
        VST2.32 { D0[1], D2[1] }, [R3]!;
        VST2.8  { D0[1], D1[1] }, [R2], R3;
        VST2.8  { D0[1], D2[1] }, [R3], R4;
        VST2.16 { D0[1], D1[1] }, [R2], R3;
        VST2.16 { D0[1], D2[1] }, [R3], R4;
        VST2.32 { D0[1], D1[1] }, [R2], R3;
        VST2.32 { D0[1], D2[1] }, [R3], R4;

        // A8.6.395 VST3 (multiple 3-element structures)
        // VST3<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VST3<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VST3<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd, Dd+1, Dd+2 }, { Dd, Dd+2, Dd+4 }.  align=omitted, 64
        // TODO: needs @align

        VST3.8  { D0, D1, D2 }, [R2];
        VST3.8  { D0, D2, D4 }, [R3];
        VST3.16 { D0, D1, D2 }, [R2];
        VST3.16 { D0, D2, D4 }, [R3];
        VST3.32 { D0, D1, D2 }, [R2];
        VST3.32 { D0, D2, D4 }, [R3];
        VST3.8  { D0, D1, D2 }, [R2]!;
        VST3.8  { D0, D2, D4 }, [R3]!;
        VST3.16 { D0, D1, D2 }, [R2]!;
        VST3.16 { D0, D2, D4 }, [R3]!;
        VST3.32 { D0, D1, D2 }, [R2]!;
        VST3.32 { D0, D2, D4 }, [R3]!;
        VST3.8  { D0, D1, D2 }, [R2], R3;
        VST3.8  { D0, D2, D4 }, [R3], R4;
        VST3.16 { D0, D1, D2 }, [R2], R3;
        VST3.16 { D0, D2, D4 }, [R3], R4;
        VST3.32 { D0, D1, D2 }, [R2], R3;
        VST3.32 { D0, D2, D4 }, [R3], R4;

        // A8.6.396 VST3 (single 3-element structure from one lane)
        // VST3<c><q>.<size> <list>, [<Rn>]                  Rm = ’1111’
        // VST3<c><q>.<size> <list>, [<Rn>]!                 Rm = ’1101’
        // VST3<c><q>.<size> <list>, [<Rn>], <Rm>            Rm = other values
        // size=8, 16, 32.  list={ Dd[x], Dd+1[x], Dd+2[x] }, { Dd[x], Dd+2[x], Dd+4[x] }.
        // TODO: needs @align

        VST3.8  { D0[1], D1[1], D2[1] }, [R2];
        VST3.8  { D0[1], D2[1], D4[1] }, [R3];
        VST3.16 { D0[1], D1[1], D2[1] }, [R2];
        VST3.16 { D0[1], D2[1], D4[1] }, [R3];
        VST3.32 { D0[1], D1[1], D2[1] }, [R2];
        VST3.32 { D0[1], D2[1], D4[1] }, [R3];
        VST3.8  { D0[1], D1[1], D2[1] }, [R2]!;
        VST3.8  { D0[1], D2[1], D4[1] }, [R3]!;
        VST3.16 { D0[1], D1[1], D2[1] }, [R2]!;
        VST3.16 { D0[1], D2[1], D4[1] }, [R3]!;
        VST3.32 { D0[1], D1[1], D2[1] }, [R2]!;
        VST3.32 { D0[1], D2[1], D4[1] }, [R3]!;
        VST3.8  { D0[1], D1[1], D2[1] }, [R2], R3;
        VST3.8  { D0[1], D2[1], D4[1] }, [R3], R4;
        VST3.16 { D0[1], D1[1], D2[1] }, [R2], R3;
        VST3.16 { D0[1], D2[1], D4[1] }, [R3], R4;
        VST3.32 { D0[1], D1[1], D2[1] }, [R2], R3;
        VST3.32 { D0[1], D2[1], D4[1] }, [R3], R4;

        // A8.6.397 VST4 (multiple 4-element structures)
        // VST4<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VST4<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VST4<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd, Dd+1, Dd+2, Dd+3 }, { Dd, Dd+2, Dd+4, Dd+6 }.  align=omitted, 64, 128, 256.
        // TODO: needs @align

        VST4.8  { D0, D1, D2, D3 }, [R2];
        VST4.8  { D0, D2, D4, D6 }, [R3];
        VST4.16 { D0, D1, D2, D3 }, [R2];
        VST4.16 { D0, D2, D4, D6 }, [R3];
        VST4.32 { D0, D1, D2, D3 }, [R2];
        VST4.32 { D0, D2, D4, D6 }, [R3];
        VST4.8  { D0, D1, D2, D3 }, [R2]!;
        VST4.8  { D0, D2, D4, D6 }, [R3]!;
        VST4.16 { D0, D1, D2, D3 }, [R2]!;
        VST4.16 { D0, D2, D4, D6 }, [R3]!;
        VST4.32 { D0, D1, D2, D3 }, [R2]!;
        VST4.32 { D0, D2, D4, D6 }, [R3]!;
        VST4.8  { D0, D1, D2, D3 }, [R2], R3;
        VST4.8  { D0, D2, D4, D6 }, [R3], R4;
        VST4.16 { D0, D1, D2, D3 }, [R2], R3;
        VST4.16 { D0, D2, D4, D6 }, [R3], R4;
        VST4.32 { D0, D1, D2, D3 }, [R2], R3;
        VST4.32 { D0, D2, D4, D6 }, [R3], R4;

        // A8.6.398 VST4 (single 4-element structure from one lane)
        // VST4<c><q>.<size> <list>, [<Rn>{@<align>}]        Rm = ’1111’
        // VST4<c><q>.<size> <list>, [<Rn>{@<align>}]!       Rm = ’1101’
        // VST4<c><q>.<size> <list>, [<Rn>{@<align>}], <Rm>  Rm = other values
        // size=8, 16, 32.  list={ Dd[x], Dd+1[x], Dd+2[x], Dd+3[x] }, { Dd[x], Dd+2[x], Dd+4[x], Dd+6[x] }.  align=omitted, 64, 128, 256.
        // TODO: needs @align

        VST4.8  { D0[1], D1[1], D2[1], D3[1] }, [R2];
        VST4.8  { D0[1], D2[1], D4[1], D6[1] }, [R3];
        VST4.16 { D0[1], D1[1], D2[1], D3[1] }, [R2];
        VST4.16 { D0[1], D2[1], D4[1], D6[1] }, [R3];
        VST4.32 { D0[1], D1[1], D2[1], D3[1] }, [R2];
        VST4.32 { D0[1], D2[1], D4[1], D6[1] }, [R3];
        VST4.8  { D0[1], D1[1], D2[1], D3[1] }, [R2]!;
        VST4.8  { D0[1], D2[1], D4[1], D6[1] }, [R3]!;
        VST4.16 { D0[1], D1[1], D2[1], D3[1] }, [R2]!;
        VST4.16 { D0[1], D2[1], D4[1], D6[1] }, [R3]!;
        VST4.32 { D0[1], D1[1], D2[1], D3[1] }, [R2]!;
        VST4.32 { D0[1], D2[1], D4[1], D6[1] }, [R3]!;
        VST4.8  { D0[1], D1[1], D2[1], D3[1] }, [R2], R3;
        VST4.8  { D0[1], D2[1], D4[1], D6[1] }, [R3], R4;
        VST4.16 { D0[1], D1[1], D2[1], D3[1] }, [R2], R3;
        VST4.16 { D0[1], D2[1], D4[1], D6[1] }, [R3], R4;
        VST4.32 { D0[1], D1[1], D2[1], D3[1] }, [R2], R3;
        VST4.32 { D0[1], D2[1], D4[1], D6[1] }, [R3], R4;

        // A8.6.399 VSTM
        // VSTM{<mode>}<c><q>{.<size>} <Rn>{!}, <list>
        // mode=IA, DB.  size=optional, 32, 64.  list={ consecutive extension registers to store }

        VSTMIA R0, { D1 };
        VSTMIA R0!, { D1 };
        VSTMIA R0, { S1 };
        VSTMIA R0!, { S1 };
        VSTMDB R0, { D1 };
        VSTMDB R0!, { D1 };
        VSTMDB R0, { S1 };
        VSTMDB R0!, { S1 };

        // A8.6.400 VSTR
        // VSTR<c><q>{.64} <Dd>, [<Rn>{, #+/-<imm>}] Encoding T1 / A1
        // VSTR<c><q>{.32} <Sd>, [<Rn>{, #+/-<imm>}] Encoding T2 / A2

        VSTR D0, [R1];
        VSTR D0, [R1, #2];
        VSTR S0, [R1];
        VSTR S0, [R1, #2];

        // A8.6.401 VSUB (integer)
        // VSUB<c><q>.<dt> {<Qd>,} <Qn>, <Qm>
        // VSUB<c><q>.<dt> {<Dd>,} <Dn>, <Dm>
        // dt=I8, I16, I32, I64

        VSUB.I8  Q0, Q1;
        VSUB.I8  Q0, Q1, Q2;
        VSUB.I16 Q0, Q1;
        VSUB.I16 Q0, Q1, Q2;
        VSUB.I32 Q0, Q1;
        VSUB.I32 Q0, Q1, Q2;
        VSUB.I64 Q0, Q1;
        VSUB.I64 Q0, Q1, Q2;
        VSUB.I8  D0, D1;
        VSUB.I8  D0, D1, D2;
        VSUB.I16 D0, D1;
        VSUB.I16 D0, D1, D2;
        VSUB.I32 D0, D1;
        VSUB.I32 D0, D1, D2;
        VSUB.I64 D0, D1;
        VSUB.I64 D0, D1, D2;

        // A8.6.402 VSUB (floating-point)
        // VSUB<c><q>.F32 {<Qd>,} <Qn>, <Qm>
        // VSUB<c><q>.F32 {<Dd>,} <Dn>, <Dm>
        // VSUB<c><q>.F64 {<Dd>,} <Dn>, <Dm>
        // VSUB<c><q>.F32 {<Sd>,} <Sn>, <Sm>

        VSUB.F32 Q0, Q1;
        VSUB.F32 Q0, Q1, Q2;
        VSUB.F32 D0, D1;
        VSUB.F32 D0, D1, D2;
        VSUB.F64 D0, D1;
        VSUB.F64 D0, D1, D2;
        VSUB.F32 S0, S1;
        VSUB.F32 S0, S1, S2;

        // A8.6.403 VSUBHN
        // VSUBHN<c><q>.<dt> <Dd>, <Qn>, <Qm>
        // dt=I16, I32, I64

        VSUBHN.I16 D0, Q1, Q2;
        VSUBHN.I32 D0, Q1, Q2;
        VSUBHN.I64 D0, Q1, Q2;

        // A8.6.404 VSUBL, VSUBW
        // VSUBL<c><q>.<dt> <Qd>, <Dn>, <Dm>
        // VSUBW<c><q>.<dt> {<Qd>,} <Qn>, <Dm>
        // dt=S8, S16, S32, U8, U16, U32

        VSUBL.S8  Q0, D1, D2;
        VSUBL.S16 Q0, D1, D2;
        VSUBL.S32 Q0, D1, D2;
        VSUBL.U8  Q0, D1, D2;
        VSUBL.U16 Q0, D1, D2;
        VSUBL.U32 Q0, D1, D2;
        VSUBW.S8  Q0, D1;
        VSUBW.S8  Q0, Q1, D2;
        VSUBW.S16 Q0, D1;
        VSUBW.S16 Q0, Q1, D2;
        VSUBW.S32 Q0, D1;
        VSUBW.S32 Q0, Q1, D2;
        VSUBW.U8  Q0, D1;
        VSUBW.U8  Q0, Q1, D2;
        VSUBW.U16 Q0, D1;
        VSUBW.U16 Q0, Q1, D2;
        VSUBW.U32 Q0, D1;
        VSUBW.U32 Q0, Q1, D2;

        // A8.6.405 VSWP
        // VSWP<c><q>{.<dt>} <Qd>, <Qm>
        // VSWP<c><q>{.<dt>} <Dd>, <Dm>
        // dt=optional, ignored. TODO: add tests with dt?

        VSWP Q0, Q1;
        VSWP D0, D1;

        // A8.6.406 VTBL, VTBX
        // V<op><c><q>.8 <Dd>, <list>, <Dm>
        // op=TBL, TBX.  list= { Dd }, { Dd, Dd+1 }, { Dd, Dd+1, Dd+2 }, { Dd, Dd+1, Dd+2, Dd+3 }

        VTBL.8 D0, { D1 }, D2;
        VTBL.8 D0, { D1, D2 }, D3;
        VTBX.8 D0, { D1 }, D2;
        VTBX.8 D0, { D1, D2 }, D3;

        // A8.6.407 VTRN
        // VTRN<c><q>.<size> <Qd>, <Qm>
        // VTRN<c><q>.<size> <Dd>, <Dm>
        // size=8, 16, 32

        VTRN.8  Q0, Q1;
        VTRN.16 Q0, Q1;
        VTRN.32 Q0, Q1;
        VTRN.8  D0, D1;
        VTRN.16 D0, D1;
        VTRN.32 D0, D1;

        // A8.6.408 VTST
        // VTST<c><q>.<size> {<Qd>,} <Qn>, <Qm> Encoded as Q = 1
        // VTST<c><q>.<size> {<Dd>,} <Dn>, <Dm> Encoded as Q = 0
        // size=8, 16, 32

        VTST.8  Q0, Q1;
        VTST.8  Q0, Q1, Q2;
        VTST.16 Q0, Q1;
        VTST.16 Q0, Q1, Q2;
        VTST.32 Q0, Q1;
        VTST.32 Q0, Q1, Q2;
        VTST.8  D0, D1;
        VTST.8  D0, D1, D2;
        VTST.16 D0, D1;
        VTST.16 D0, D1, D2;
        VTST.32 D0, D1;
        VTST.32 D0, D1, D2;

        // A8.6.409 VUZP
        // VUZP<c><q>.<size> <Qd>, <Qm>
        // VUZP<c><q>.<size> <Dd>, <Dm>
        // size=8, 16, 32

        VUZP.8  Q0, Q1;
        VUZP.16 Q0, Q1;
        VUZP.32 Q0, Q1;
        VUZP.8  D0, D1;
        VUZP.16 D0, D1;

        // A8.6.410 VZIP
        // VZIP<c><q>.<size> <Qd>, <Qm>
        // VZIP<c><q>.<size> <Dd>, <Dm>
        // size=8, 16, 32

        VZIP.8  Q0, Q1;
        VZIP.16 Q0, Q1;
        VZIP.32 Q0, Q1;
        VZIP.8  D0, D1;
        VZIP.16 D0, D1;

    }
}
}


} // version(ARM)

int main()
{
    return 0;
}

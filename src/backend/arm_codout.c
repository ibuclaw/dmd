
#define GEN(c)          (*pgen++ = (c)); FLUSH()
#define GEN4(x) \
    GEN(((uint32_t)x & 0x000000ff)); \
    GEN(((uint32_t)x & 0x0000ff00) >> 8); \
    GEN(((uint32_t)x & 0x00ff0000) >> 16); \
    GEN(((uint32_t)x & 0xff000000) >> 24);
#define OFFSET()        (offset + (pgen - bytes))
#define FLUSH()         \
    if (pgen - bytes) \
    { \
        assert(pgen - bytes < sizeof(bytes)); \
        offset += Obj::bytes(cseg, offset, pgen - bytes, bytes); \
        pgen = bytes; \
    }

static targ_size_t foldimmediate(targ_size_t value)
{
    assert(value <= 0xff);
    return value;
}

static targ_size_t buildShiftImm(code* c)
{
    targ_size_t value = 0;

    if (c->getShiftImm() == 0)
    {
        // bit 0 == 0
        value |= c->getShiftType()   << 1;
        value |= c->getShiftOffset() << 3;
    }
    else
    {
        value |= 1;
        value |= c->getShiftType() << 1;
        // bit 3 == 0
        value |= c->getShiftOffset() << 4;
    }

    return value;
}

unsigned codout(code *c)
{
    targ_size_t offset = Coffset;
    char bytes[100];
    char *pgen = bytes;

#ifdef DEBUG
    if (debugc) printf("codout(%p), Coffset = x%llx\n",c,(unsigned long long)Coffset);
#endif

    for (; c; c = code_next(c))
    {
#ifdef DEBUG
        if (debugc) { printf("off=%02lx, sz=%ld, ",(long)OFFSET(),(long)calccodsize(c)); c->print(); }
        unsigned startoffset = OFFSET();
#endif

        // check for enough space for the largest code sequence
        if (pgen - bytes > sizeof(bytes) - 4) FLUSH();

        switch (c->Iop & ESCAPEmask)
        {
            case ESCAPE:
                switch (c->Iop & ~ESCAPEmask)
                {
                    case ESClinnum:
                        Obj::linnum(c->IEV1.Vsrcpos,OFFSET());
                        break;

                    default:
                        assert(0);
                }
#ifdef DEBUG
                assert(calccodsize(c) == 0);
#endif
                continue;

            case NOP:
                assert(c->Iop == NOP);
#ifdef DEBUG
                assert(calccodsize(c) == 0);
#endif
                // fallthrough
            case PADDING:
                continue;

            case ASM:
                FLUSH();
                offset += Obj::bytes(cseg, offset, c->IEV1.as.len, c->IEV1.as.bytes);
                break;

            case OP_ADC:
            case OP_ADD:
            case OP_ADR:
            case OP_AND:
            case OP_BIC:
            case OP_CMN:
            case OP_CMP:
            case OP_EOR:
            case OP_MOV:
            case OP_MVN:
            case OP_ORR:
            case OP_RSB: // OP_NEG encodes as RSBS
            case OP_RSC:
            case OP_SBC:
            case OP_SUB:
            case OP_TEQ:
            case OP_TST:
            {
                uint32_t iop = c->Iop;
                uint32_t imm = c->IEVsize_t1;

                if (iop == OP_ADR)
                {
                    // convert to add or sub:
                    assert(c->IFL1 == FLblockoff);
                    uint32_t boff = c->IEV1.Vblock->Boffset;
                    if (OFFSET() > boff)
                    {
                        iop = OP_SUB;
                        imm = OFFSET() - boff;
                    }
                    else
                    {
                        iop = OP_ADD;
                        imm = boff - OFFSET();
                    }
                }

                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= (iop - OP_AND) << 21;               // opcode
                instruction |= ((c->Iflags & CFpsw) ? 1 : 0) << 20;   // set cc
                if (!(iop == OP_MOV || iop == OP_MVN))
                    instruction |= c->getRegN() << 16;
                instruction |= c->getRegD() << 12;

                if (c->Iflags & CFimmed)
                {
                    instruction |= 1 << 25; // immediate bit
                    instruction |= foldimmediate(imm);
                }
                else
                {
                    instruction |= c->getRegM();

                    instruction |= buildShiftImm(c) << 4;
                }
                GEN4(instruction);
                break;
            }

            case OP_MLA:
            case OP_MLS:
            case OP_MUL:
            case OP_SMLAL:
            case OP_SMULL:
            case OP_UMAAL:
            case OP_UMLAL:
            case OP_UMULL:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                switch (c->Iop)
                {
                    case OP_MLA:   instruction |= (0x1 << 21); break;
                    case OP_MLS:   instruction |= (0x3 << 21); break;
                    case OP_MUL:                               break;
                    case OP_SMLAL: instruction |= (0x7 << 21); break;
                    case OP_SMULL: instruction |= (0x6 << 21); break;
                    case OP_UMAAL: instruction |= (0x2 << 21); break;
                    case OP_UMLAL: instruction |= (0x5 << 21); break;
                    case OP_UMULL: instruction |= (0x4 << 21); break;
                }
                instruction |= ((c->Iflags & CFpsw) ? 1 : 0) << 20;   // set cc
                instruction |= 0x9 << 4;

                instruction |= c->getRegD() << 16;
                instruction |= c->getRegS() << 12;
                instruction |= c->getRegM() << 8;
                instruction |= c->getRegN();

                GEN4(instruction);
                break;
            }

            case OP_QADD:
            case OP_QDADD:
            case OP_QDSUB:
            case OP_QSUB:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 1 << 24;
                switch (c->Iop)
                {
                    case OP_QADD:                              break;
                    case OP_QSUB:  instruction |= (0x1 << 21); break;
                    case OP_QDADD: instruction |= (0x2 << 21); break;
                    case OP_QDSUB: instruction |= (0x3 << 21); break;
                }
                instruction |= 0x5 << 4;

                instruction |= c->getRegN() << 16;
                instruction |= c->getRegD() << 12;
                instruction |= c->getRegM();

                GEN4(instruction);
                break;
            }

            case OP_SMLABB:
            case OP_SMLABT:
            case OP_SMLALBB:
            case OP_SMLALBT:
            case OP_SMLALTB:
            case OP_SMLALTT:
            case OP_SMLATB:
            case OP_SMLATT:
            case OP_SMLAWB:
            case OP_SMLAWT:
            case OP_SMULBB:
            case OP_SMULBT:
            case OP_SMULTB:
            case OP_SMULTT:
            case OP_SMULWB:
            case OP_SMULWT:
            {
                uint32_t instruction = 0;
                uint32_t M = 0;
                uint32_t N = 0;
                uint32_t op = 0;
                instruction |= c->getCC() << 28;
                instruction |= 1 << 24;
                switch (c->Iop)
                {
                    case OP_SMLABB:  M = 0; N = 0; op = 0; break;
                    case OP_SMLABT:  M = 1; N = 0; op = 0; break;
                    case OP_SMLATB:  M = 0; N = 1; op = 0; break;
                    case OP_SMLATT:  M = 1; N = 1; op = 0; break;

                    case OP_SMLAWB:  M = 0; N = 0; op = 1; break;
                    case OP_SMLAWT:  M = 1; N = 0; op = 1; break;

                    case OP_SMULWB:  M = 0; N = 1; op = 1; break;
                    case OP_SMULWT:  M = 1; N = 1; op = 1; break;

                    case OP_SMLALBB: M = 0; N = 0; op = 2; break;
                    case OP_SMLALBT: M = 1; N = 0; op = 2; break;
                    case OP_SMLALTB: M = 0; N = 1; op = 2; break;
                    case OP_SMLALTT: M = 1; N = 1; op = 2; break;

                    case OP_SMULBB:  M = 0; N = 0; op = 3; break;
                    case OP_SMULBT:  M = 1; N = 0; op = 3; break;
                    case OP_SMULTB:  M = 0; N = 1; op = 3; break;
                    case OP_SMULTT:  M = 1; N = 1; op = 3; break;

                    default:
                        assert(0);
                }
                instruction |= op << 21;

                instruction |= 1 << 7;
                instruction |= M << 6;
                instruction |= N << 5;

                instruction |= c->getRegD() << 16;
                instruction |= c->getRegS() << 12;
                instruction |= c->getRegM() << 8;
                instruction |= c->getRegN();

                GEN4(instruction);
                break;
            }

            case OP_LDRD:
            case OP_LDRH:
            case OP_LDRSB:
            case OP_LDRSH:
            case OP_STRD:
            case OP_STRH:
            {
                uint32_t instruction = 0;
                uint32_t op1 = 0; // bits 24..20: P U x W x
                uint32_t op2 = 0; // bits 6..5
                instruction |= c->getCC() << 28;

                uint32_t mode = c->getAddrMode();
                op1 |= ((mode == IAMMode_off || mode == IAMMode_preidx) ? 1 : 0) << 4; // P
                op1 |= 1 << 2; // 0 == register, 1 == all other forms
                op1 |= (mode == IAMMode_preidx ? 1 : 0) << 1;                          // W

                // fill in registers and U
                switch (c->IFL1)
                {
                    case FLdsymbol: // literal
                        assert(0);
                        break;

                        // encode offset
                        //c->setRegS((c->IEVsize_t1 & 0xf0) >> 4);
                        //c->setRegM(c->IEVsize_t1 & 0x0f);
                        break;

                    case FLunde: // register
                        op1 |= (c->getShiftSubtract() ? 0 : 1) << 3;
                        op1 &= ~(1 << 2); // turn bit 2 off
                        break;

                    case FLconst: // immediate
                        if (c->IEV1.Vllong >= 0)
                            op1 |= 1 << 3;
                        else
                            c->IEVsize_t1 = -c->IEVsize_t1;

                        c->setRegS((c->IEVsize_t1 & 0xf0) >> 4);
                        c->setRegM(c->IEVsize_t1 & 0x0f);
                        break;

                    case FLblockoff: // literal?
                    {
                        assert(0);
                        //ssize_t off = c->IEV1.Vblock->Boffset;
                        //op1 |= 1 << 4;
                        //op1 |= // add vs sub
                        //op1 |= 5;
                        //c->setRegN(0xf);
                        //instruction |= (c->IEVsize_t1 & 0xf0) >> 4;
                        //instruction |=  c->IEVsize_t1 & 0x0f;
                    }

                    default:
                        assert(0);
                }

                // set last bit of op1 and all of op2
                switch (c->Iop)
                {
                    case OP_LDRH:  op1 |= 1; op2 = 1; break;
                    case OP_STRH:            op2 = 1; break;

                    case OP_LDRD:            op2 = 2; break;
                    case OP_LDRSB: op1 |= 1; op2 = 2; break;

                    case OP_LDRSH: op1 |= 1; op2 = 3; break;
                    case OP_STRD:            op2 = 3; break;

                    default: assert(0);
                }

                instruction |= op1 << 20;
                instruction |= (0x9 | (op2 << 1)) << 4; // 1 op2 op2 1

                instruction |= c->getRegN() << 16;
                instruction |= c->getRegD() << 12;
                instruction |= c->getRegS() <<  8;
                instruction |= c->getRegM();

                GEN4(instruction);
                break;
            }

            case OP_LDRHT:
            case OP_LDRSBT:
            case OP_LDRSHT:
            case OP_STRHT:
            {
                unsigned instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 1 << 21;
                instruction |= 1 << 7;
                instruction |= 1 << 4;

                uint32_t U = 0, op1 = 0, op2 = 0;
                switch (c->Iop)
                {
                    case OP_STRHT:   op1 = 0; op2 = 1; break;
                    case OP_LDRHT:   op1 = 1; op2 = 1; break;
                    case OP_LDRSBT:  op1 = 1; op2 = 2; break;
                    case OP_LDRSHT:  op1 = 1; op2 = 3; break;
                    default: assert(0);
                }

                switch (c->IFL1)
                {
                    case FLunde: // register
                        // bit 22 == 0 for register
                        U = c->getShiftSubtract() ? 0 : 1;
                        break;
                    case FLconst: // immediate
                        instruction |= 1 << 22;
                        if (c->IEV1.Vllong >= 0)
                            U = 1;
                        else
                            c->IEVsize_t1 = -c->IEVsize_t1;

                        c->setRegS((c->IEVsize_t1 & 0xf0) >> 4);
                        c->setRegM(c->IEVsize_t1 & 0x0f);
                        break;

                    default: assert(0);
                }

                instruction |= U << 23;
                instruction |= op1 << 20;
                instruction |= op2 << 5;

                instruction |= c->getRegN() << 16;
                instruction |= c->getRegD() << 12;
                instruction |= c->getRegS() << 8;
                instruction |= c->getRegM();

                GEN4(instruction);
                break;
            }

            case OP_SWP:
            case OP_SWPB:
            case OP_LDREX:
            case OP_LDREXB:
            case OP_LDREXH:
            case OP_LDREXD:
            case OP_STREX:
            case OP_STREXB:
            case OP_STREXH:
            case OP_STREXD:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;

                instruction |= 1 << 24;
                instruction |= 1 <<  7;
                instruction |= 1 <<  4;

                uint32_t op = 0;
                switch (c->Iop)
                {
                    case OP_SWP:             break;
                    case OP_SWPB:   op =  4; break;
                    case OP_LDREX:  op =  9; break;
                    case OP_LDREXB: op = 13; break;
                    case OP_LDREXH: op = 15; break;
                    case OP_LDREXD: op = 11; break;
                    case OP_STREX:  op =  8; break;
                    case OP_STREXB: op = 12; break;
                    case OP_STREXH: op = 14; break;
                    case OP_STREXD: op = 10; break;
                    default: assert(0);
                }

                instruction |= op << 20;
                instruction |= c->getRegN() << 16;
                instruction |= c->getRegD() << 12;
                instruction |= c->getRegS() << 8;
                instruction |= c->getRegM();

                GEN4(instruction);
                break;
            }

            case OP_NOP:
            case OP_YIELD:
            case OP_WFE:
            case OP_WFI:
            case OP_SEV:
            case OP_DBG:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 0x19 << 21;
                instruction |= 0xf << 12;

                uint32_t op2 = 0;
                switch (c->Iop)
                {
                    case OP_NOP:   op2 =    0; break;
                    case OP_YIELD: op2 =    1; break;
                    case OP_WFE:   op2 =    2; break;
                    case OP_WFI:   op2 =    3; break;
                    case OP_SEV:   op2 =    4; break;
                    case OP_DBG:   op2 = 0xf0 | c->getRegN(); break;
                    default: assert(0);
                }

                instruction |= op2;

                GEN4(instruction);
                break;
            }

            case OP_MSR:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 0x9 << 21;
                instruction |= 0xf << 12;

                if (c->IFL1 == FLconst)
                {
                    instruction |= foldimmediate(c->IEVsize_t1);
                    instruction |= 1 << 25;
                    instruction |= (c->getRegM() & 0x4) << 20; // bit 22, 4 is already << 2
                }
                else
                {
                    instruction |= c->getRegN();
                    instruction |= (c->getRegM() & 0x3) << 8;  // banked extra mask bits
                    instruction |= (c->getRegM() & 0x4) << 20; // bit 22, 4 is already << 2
                }

                instruction |= c->getRegS() << 16; // mask bits

                GEN4(instruction);
                break;
            }

            case OP_MRS:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 1 << 24;
                instruction |= (c->getRegM() & 0x4) << 20; // bit 22, 4 is already << 2
                instruction |= c->getRegS() << 16; // mask bits
                instruction |= c->getRegD() << 12;
                instruction |= (c->getRegM() & 0x3) << 8;  // banked extra mask bits

                GEN4(instruction);
                break;
            }

            case OP_B:
            case OP_BL:
Lbranch_immed: // adds OP_BLX
            {
                FLUSH();
                ssize_t off = 0;
                switch(c->IFL1)
                {
                    case FLfunc:
                        Obj::reftoident(cseg, OFFSET(), c->IEVsym1, 0, c->Iflags);
                        off = c->IEVoffset1;
                        break;
                    case FLblock:
                        off = c->IEV1.Vblock->Boffset - OFFSET();
                        break;
                    case FLconst:
                        off = c->IEVoffset1;
                        break;
                    case FLblockoff:
                        assert(c->IEV1.Vblock);
                        off = c->IEV1.Vblock->Boffset;
                        //printf("FLblockoff: offset = %x, Boffset = %x, funcoffset = %x\n", offset, uev->Vblock->Boffset, funcoffset);
                        Obj::reftocodeseg(cseg, OFFSET(), c->IEV1.Vblock->Boffset);
                        break;
                    default:
                        assert(0);
                }
                off -= 8; // PC is always 8 bytes ahead
                off >>= 2;
                uint32_t instruction = 0;
                instruction |= (c->Iop == OP_BLX ? 0xf : c->getCC()) << 28;
                instruction |= 0x5 << 25;
                instruction |= (c->Iop == OP_BL ? 1 : 0) << 24;
                instruction |= off & 0x00ffffff;
                GEN4(instruction);
                break;
            }

            case OP_BKPT:
            case OP_BLX:
            case OP_BX:
            case OP_CLZ:
            case OP_ERET:
            case OP_HVC:
            case OP_SMC:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 1 << 24;

                uint32_t op = 0, op1 = 0, op2 = 0, B = 0;
                switch (c->Iop)
                {
                    case OP_BKPT:
                        op = 1;
                        op2 = 7;
                        assert(c->getCC() == COND_AL);
                        assert(c->IFL1 == FLconst);
                        instruction |= (c->IEVsize_t1 & 0xfff0) << 4;
                        instruction |= c->IEVsize_t1 & 0xf;
                        break;
                    case OP_BLX:
                        if (c->IFL1 != FLunde) goto Lbranch_immed;
                        op = 1;
                        op2 = 3;
                        instruction |= 0xfff << 8;
                        instruction |= c->getRegM();
                        break;
                    case OP_BX:
                        if (c->IFL1 != FLunde) goto Lbranch_immed;
                        op = 1;
                        op2 = 1;
                        instruction |= 0xfff << 8;
                        instruction |= c->getRegM();
                        break;
                    case OP_CLZ:
                        op = 3;
                        op2 = 1;
                        instruction |= 0xf0f << 8;
                        instruction |= c->getRegD() << 12;
                        instruction |= c->getRegM();
                        break;
                    case OP_ERET:
                        op = 3;
                        op2 = 6;
                        instruction |= 0xe;
                        break;
                    case OP_HVC:
                        op = 2;
                        op2 = 7;
                        assert(c->getCC() == COND_AL);
                        assert(c->IFL1 == FLconst);
                        instruction |= (c->IEVsize_t1 & 0xfff0) << 4;
                        instruction |= c->IEVsize_t1 & 0xf;
                        break;
                    case OP_SMC:
                        op = 3;
                        op2 = 7;
                        assert(c->IFL1 == FLconst);
                        instruction |= c->IEVsize_t1;
                        break;
                    default: assert(0);
                }

                instruction |= op  << 21;
                instruction |= op2 <<  4;

                GEN4(instruction);
                break;
            }

            case OP_LDR:
            case OP_LDRB:
            case OP_LDRBT:
            case OP_LDRT:
            case OP_STR:
            case OP_STRB:
            case OP_STRBT:
            case OP_STRT:
            {
                bool needFixup = false;
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 1 << 26;

                uint32_t mode = c->getAddrMode(), op1 = 0;
                op1 |= ((mode == IAMMode_off || mode == IAMMode_preidx) ? 1 : 0) << 4; // P
                // op2 bit 3: add (1) vs subtract (0), set in IFL1 switch
                // op1 bit 2: byte (1) vs word (0), set in c->Iop switch
                op1 |= (mode == IAMMode_preidx ? 1 : 0) << 1;                          // W
                // op1 bit 0: ldr (1) vs str (0), set in c->Iop switch

                // fill in registers and U
                switch (c->IFL1)
                {
                    case FLdsymbol: // literal
                        assert(0);
                        break;

                    case FLunde: // register
                        op1 |= (c->getShiftSubtract() ? 0 : 1) << 3;
                        instruction |= buildShiftImm(c) << 4;
                        instruction |= c->getRegM();
                        break;

                    case FLconst: // immediate
                        if (c->IEV1.Vllong >= 0)
                            op1 |= 1 << 3;
                        else
                            c->IEVsize_t1 = -c->IEVsize_t1;

                        instruction |= c->IEVsize_t1 & 0xfff;
                        break;

                    case FLblockoff: // literal?
                        assert(0);
                        break;

                    case FLextern:
                    {
                        if (c->IEV1.Vllong >= 0)
                            op1 |= 1 << 3;
                        else
                            c->IEVsize_t1 = -c->IEVsize_t1;

                        needFixup = true;
                        instruction |= c->IEVpointer1 & 0xfff;
                        break;
                    }

                    default:
                        assert(0);
                }

                uint32_t B = 0;
                switch (c->Iop)
                {
                    case OP_LDR:   op1 |= 0x1; break;
                    case OP_LDRB:  op1 |= 0x5; break;
                    case OP_LDRBT: op1 |= 0x7; op1 &= 0xf; break;
                    case OP_LDRT:  op1 |= 0x3; op1 &= 0xf; break;
                    case OP_STR:               break;
                    case OP_STRB:  op1 |= 0x4; break;
                    case OP_STRBT: op1 |= 0x6; op1 &= 0xf; break;
                    case OP_STRT:  op1 |= 0x2; op1 &= 0xf; break;
                    default: assert(0);
                }

                instruction |= (c->getShiftImm() == 0 ? 1 : 0) << 25;
                instruction |= op1 << 20;
                instruction |= B << 4;

                instruction |= c->getRegN() << 16;
                instruction |= c->getRegD() << 12;

                GEN4(instruction);

                if (needFixup)
                {
                    // generate jmp over reloc instruction
                    uint32_t i2 = COND_AL << 28;
                    i2 |= 0x5 << 25;
                    //i2 |= 0 & 0x00ffffff; // no extra offset needed, the default is right
                    GEN4(i2);

                    // emit relocation
                    FLUSH();
                    offset += Obj::reftoident(cseg, OFFSET(), c->IEVsym1, c->IEVoffset1, CFoff);
                }

                break;
            }

            case OP_BFC:
            case OP_BFI:
            case OP_SBFX:
            case OP_UBFX:
            case OP_UDF:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 3 << 25;
                instruction |= 1 << 4;

                uint32_t op1 = 0, op2 = 0;
                switch (c->Iop)
                {
                    case OP_BFC:
                        assert(c->IFL1 == FLconst);
                        op1 = 0x1c;
                        goto Lencode_lsbwidth;
                    case OP_BFI:
                        assert(c->IFL1 == FLconst);
                        op1 = 0x1c;
                        goto Lencode_lsbwidth;
                    case OP_SBFX:
                        assert(c->IFL1 == FLconst);
                        op1 = 0x1a;
                        op2 = 2;
                        goto Lencode_lsbwidth;
                    case OP_UBFX:
                        assert(c->IFL1 == FLconst);
                        op1 = 0x1e;
                        op2 = 2;
Lencode_lsbwidth:
                        instruction |= (c->IEVsize_t1 & 0x01f) << 7; // lsb
                        instruction |= (c->IEVsize_t1 & 0x3e0) << 11; // width; 16-5==11
                        break;
                    case OP_UDF:
                        assert(c->getCC() == COND_AL);
                        assert(c->IFL1 == FLconst);
                        op1 = 0x1f;
                        op2 = 0x7;
                        instruction |= (c->IEVsize_t1 & 0xfff0) << 4;
                        instruction |= (c->IEVsize_t1 & 0xf);
                        break;
                    default: assert(0);
                }

                instruction |= op1 << 20;
                instruction |= op2 << 5;

                instruction |= c->getRegD() << 12;
                instruction |= c->getRegN();

                GEN4(instruction);
                break;
            }

            case OP_USAD8:
            case OP_USADA8:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 0xf << 23;
                instruction |= c->getRegD() << 16;
                instruction |= c->getRegS() << 12;
                instruction |= c->getRegM() << 8;
                instruction |= 1 << 4;
                instruction |= c->getRegN();
                GEN4(instruction);
                break;
            }

            case OP_QADD16:
            case OP_QADD8:
            case OP_QASX:
            case OP_QSAX:
            case OP_QSUB16:
            case OP_QSUB8:
            case OP_SADD16:
            case OP_SADD8:
            case OP_SASX:
            case OP_SHADD16:
            case OP_SHADD8:
            case OP_SHASX:
            case OP_SHSAX:
            case OP_SHSUB16:
            case OP_SHSUB8:
            case OP_SSAX:
            case OP_SSUB16:
            case OP_SSUB8:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 0x3 << 25;
                instruction |= 0xf << 8;
                instruction |= 0x1 << 4;

                uint32_t op1 = 0, op2 = 0;
                switch (c->Iop)
                {
                    case OP_SADD16:  op1 = 1; op2 = 0; break;
                    case OP_SADD8:   op1 = 1; op2 = 4; break;
                    case OP_SASX:    op1 = 1; op2 = 1; break;
                    case OP_SSAX:    op1 = 1; op2 = 2; break;
                    case OP_SSUB16:  op1 = 1; op2 = 3; break;
                    case OP_SSUB8:   op1 = 1; op2 = 7; break;

                    case OP_QADD16:  op1 = 2; op2 = 0; break;
                    case OP_QADD8:   op1 = 2; op2 = 4; break;
                    case OP_QASX:    op1 = 2; op2 = 1; break;
                    case OP_QSAX:    op1 = 2; op2 = 2; break;
                    case OP_QSUB16:  op1 = 2; op2 = 3; break;
                    case OP_QSUB8:   op1 = 2; op2 = 7; break;

                    case OP_SHADD16: op1 = 3; op2 = 0; break;
                    case OP_SHADD8:  op1 = 3; op2 = 4; break;
                    case OP_SHASX:   op1 = 3; op2 = 1; break;
                    case OP_SHSAX:   op1 = 3; op2 = 2; break;
                    case OP_SHSUB16: op1 = 3; op2 = 3; break;
                    case OP_SHSUB8:  op1 = 3; op2 = 7; break;
                    default: assert(0);
                }

                instruction |= op1 << 20;
                instruction |= op2 << 5;

                instruction |= c->getRegN() << 16;
                instruction |= c->getRegD() << 12;
                instruction |= c->getRegM();

                GEN4(instruction);
                break;
            }

            case OP_UADD16:
            case OP_UADD8:
            case OP_UASX:
            case OP_USAX:
            case OP_USUB16:
            case OP_USUB8:
            case OP_UQADD16:
            case OP_UQADD8:
            case OP_UQASX:
            case OP_UQSAX:
            case OP_UQSUB16:
            case OP_UQSUB8:
            case OP_UHADD16:
            case OP_UHADD8:
            case OP_UHASX:
            case OP_UHSAX:
            case OP_UHSUB16:
            case OP_UHSUB8:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 0x19 << 22;
                instruction |= 0xf << 8;
                instruction |= 0x1 << 4;

                uint32_t op1 = 0, op2 = 0;
                switch (c->Iop)
                {
                    case OP_UADD16:  op1 = 1; op2 = 0; break;
                    case OP_UADD8:   op1 = 1; op2 = 4; break;
                    case OP_UASX:    op1 = 1; op2 = 1; break;
                    case OP_USAX:    op1 = 1; op2 = 2; break;
                    case OP_USUB16:  op1 = 1; op2 = 3; break;
                    case OP_USUB8:   op1 = 1; op2 = 7; break;

                    case OP_UQADD16: op1 = 2; op2 = 0; break;
                    case OP_UQADD8:  op1 = 2; op2 = 4; break;
                    case OP_UQASX:   op1 = 2; op2 = 1; break;
                    case OP_UQSAX:   op1 = 2; op2 = 2; break;
                    case OP_UQSUB16: op1 = 2; op2 = 3; break;
                    case OP_UQSUB8:  op1 = 2; op2 = 7; break;

                    case OP_UHADD16: op1 = 3; op2 = 0; break;
                    case OP_UHADD8:  op1 = 3; op2 = 4; break;
                    case OP_UHASX:   op1 = 3; op2 = 1; break;
                    case OP_UHSAX:   op1 = 3; op2 = 2; break;
                    case OP_UHSUB16: op1 = 3; op2 = 3; break;
                    case OP_UHSUB8:  op1 = 3; op2 = 7; break;
                    default: assert(0);
                }

                instruction |= op1 << 20;
                instruction |= op2 << 5;

                instruction |= c->getRegN() << 16;
                instruction |= c->getRegD() << 12;
                instruction |= c->getRegM();

                GEN4(instruction);
                break;
            }

            case OP_PKHBT:
            case OP_PKHTB:
            case OP_RBIT:
            case OP_REV:
            case OP_REV16:
            case OP_REVSH:
            case OP_SEL:
            case OP_SSAT:
            case OP_SSAT16:
            case OP_SXTAB:
            case OP_SXTAB16:
            case OP_SXTAH:
            case OP_SXTB:
            case OP_SXTB16:
            case OP_SXTH:
            case OP_USAT:
            case OP_USAT16:
            case OP_UXTAB:
            case OP_UXTAB16:
            case OP_UXTAH:
            case OP_UXTB:
            case OP_UXTB16:
            case OP_UXTH:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 0xd << 23;
                instruction |= 1 << 4;

                uint32_t op1 = 0, op2 = 0; // 3 bits, 3 bits
                bool f16 = false;
                switch (c->Iop)
                {
                    case OP_PKHBT:                                 break;
                    case OP_PKHTB:                                 break;
                    case OP_RBIT:    op1 = 7; op2 = 1; f16 = true; break;
                    case OP_REV16:   op1 = 3; op2 = 5; f16 = true; break;
                    case OP_REV:     op1 = 3; op2 = 1; f16 = true; break;
                    case OP_REVSH:   op1 = 7; op2 = 5; f16 = true; break;
                    case OP_SEL:              op2 = 5;             break;
                    case OP_SSAT16:  op1 = 2; op2 = 1;             break;
                    case OP_SSAT:    op1 = 2;                      break;
                    case OP_SXTAB16:          op2 = 3;             break;
                    case OP_SXTAB:   op1 = 2; op2 = 3;             break;
                    case OP_SXTAH:   op1 = 3; op2 = 3;             break;
                    case OP_SXTB16:           op2 = 3; f16 = true; break;
                    case OP_SXTB:    op1 = 2; op2 = 3; f16 = true; break;
                    case OP_SXTH:    op1 = 3; op2 = 3; f16 = true; break;
                    case OP_USAT16:  op1 = 6; op2 = 1;             break;
                    case OP_USAT:    op1 = 6;                      break;
                    case OP_UXTAB16: op1 = 4; op2 = 3;             break;
                    case OP_UXTAB:   op1 = 6; op2 = 3;             break;
                    case OP_UXTAH:   op1 = 7; op2 = 3;             break;
                    case OP_UXTB16:  op1 = 4; op2 = 3; f16 = true; break;
                    case OP_UXTB:    op1 = 6; op2 = 3; f16 = true; break;
                    case OP_UXTH:    op1 = 7; op2 = 3; f16 = true; break;
                    default: assert(0);
                }

                if (op1 == 0 && op2 == 0)
                {
                    instruction |= c->getShiftOffset() << 7;
                    instruction |= (c->getShiftType() == 2 ? 1 : 0) << 6;
                }

                if (op2 & 3)
                    instruction |= (c->getShiftOffset() >> 3) << 10;

                if ((op2 & 3) == 1)
                    instruction |= 0xf << 8;

                if (f16)
                    instruction |= 0xf << 16;

                if ((op1 == 2 || op1 == 6) && (op2 == 0 || op2 == 1))
                {
                    assert(c->IFL1 == FLconst);
                    assert((c->getShiftType() == 0 || c->getShiftType() == 2) && c->getShiftImm() == 0);

                    if (op2 == 0)
                    {
                        instruction |= (c->IEVsize_t1 & 0x1f) << 16;
                        instruction |= buildShiftImm(c) << 4; // bits 1..0 happen to be 0
                    }
                    else
                    {
                        instruction |= (c->IEVsize_t1 & 0xf) << 16;
                    }
                }

                instruction |= op1 << 20;
                instruction |= op2 << 5;

                instruction |= c->getRegN() << 16;
                instruction |= c->getRegD() << 12;
                instruction |= c->getRegM();

                GEN4(instruction);
                break;
            }

            case OP_SDIV:
            case OP_SMLAD:
            case OP_SMLADX:
            case OP_SMLALD:
            case OP_SMLALDX:
            case OP_SMLSD:
            case OP_SMLSDX:
            case OP_SMLSLD:
            case OP_SMLSLDX:
            case OP_SMMLA:
            case OP_SMMLAR:
            case OP_SMMLS:
            case OP_SMMLSR:
            case OP_SMMUL:
            case OP_SMMULR:
            case OP_SMUAD:
            case OP_SMUADX:
            case OP_SMUSD:
            case OP_SMUSDX:
            case OP_UDIV:
            {
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 7 << 24;
                instruction |= 1 << 4;

                uint32_t op1 = 0, op2 = 0, A = 0;
                switch (c->Iop)
                {
                    case OP_SDIV:    op1 = 1; op2 = 0; A = 0xf;          break;
                    case OP_SMLAD:   op1 = 0; op2 = 0; A = c->getRegS(); break;
                    case OP_SMLADX:  op1 = 0; op2 = 1; A = c->getRegS(); break;
                    case OP_SMLALD:  op1 = 4; op2 = 0; A = c->getRegS(); break;
                    case OP_SMLALDX: op1 = 4; op2 = 1; A = c->getRegS(); break;
                    case OP_SMLSD:   op1 = 0; op2 = 2; A = c->getRegS(); break;
                    case OP_SMLSDX:  op1 = 0; op2 = 3; A = c->getRegS(); break;
                    case OP_SMLSLD:  op1 = 4; op2 = 2; A = c->getRegS(); break;
                    case OP_SMLSLDX: op1 = 4; op2 = 3; A = c->getRegS(); break;
                    case OP_SMMLA:   op1 = 5; op2 = 0; A = c->getRegS(); break;
                    case OP_SMMLAR:  op1 = 5; op2 = 1; A = c->getRegS(); break;
                    case OP_SMMLS:   op1 = 5; op2 = 6; A = c->getRegS(); break;
                    case OP_SMMLSR:  op1 = 5; op2 = 7; A = c->getRegS(); break;
                    case OP_SMMUL:   op1 = 5; op2 = 0; A = 0xf;          break;
                    case OP_SMMULR:  op1 = 5; op2 = 1; A = 0xf;          break;
                    case OP_SMUAD:   op1 = 0; op2 = 0; A = 0xf;          break;
                    case OP_SMUADX:  op1 = 0; op2 = 1; A = 0xf;          break;
                    case OP_SMUSD:   op1 = 0; op2 = 2; A = 0xf;          break;
                    case OP_SMUSDX:  op1 = 0; op2 = 3; A = 0xf;          break;
                    case OP_UDIV:    op1 = 3; op2 = 0; A = 0xf;          break;
                    default: assert(0);
                }

                instruction |= op1 << 20;
                instruction |= A << 12;
                instruction |= op2 << 5;
                instruction |= c->getRegD() << 16;
                instruction |= c->getRegM() << 8;
                instruction |= c->getRegN();

                GEN4(instruction);
                break;
            }

            case OP_LDM:
            case OP_LDMDA:
            case OP_LDMDB:
            case OP_LDMEA:
            case OP_LDMED:
            case OP_LDMFA:
            case OP_LDMFD:
            case OP_LDMIA:
            case OP_LDMIB:
            case OP_POP:
            case OP_PUSH:
            case OP_STM:
            case OP_STMDA:
            case OP_STMDB:
            case OP_STMEA:
            case OP_STMED:
            case OP_STMFA:
            case OP_STMFD:
            case OP_STMIA:
            case OP_STMIB:
            {
                // NOTE: LDM/STM exception versions not implemented yet
                uint32_t instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 1 << 27;

                uint32_t op = 0;
                switch (c->Iop)
                {
                    case OP_POP:
                    case OP_LDM: case OP_LDMIA: case OP_LDMFD:
                        op = c->getAddrMode() == IAMMode_postidx ? 0xb : 9;
                        instruction |= c->IEVsize_t1;
                        break;
                    case OP_LDMDA: case OP_LDMFA:
                        op = c->getAddrMode() == IAMMode_postidx ? 3 : 1;
                        instruction |= c->IEVsize_t1;
                        break;
                    case OP_LDMDB: case OP_LDMEA:
                        op = c->getAddrMode() == IAMMode_postidx ? 0x13 : 0x11;
                        instruction |= c->IEVsize_t1;
                        break;
                    case OP_LDMIB: case OP_LDMED:
                        op = c->getAddrMode() == IAMMode_postidx ? 0x1b : 0x19;
                        instruction |= c->IEVsize_t1;
                        break;
                    case OP_STM: case OP_STMIA: case OP_STMEA:
                        op = c->getAddrMode() == IAMMode_postidx ? 0xa : 8;
                        instruction |= c->IEVsize_t1;
                        break;
                    case OP_STMDA: case OP_STMED:
                        op = c->getAddrMode() == IAMMode_postidx ? 2 : 0;
                        instruction |= c->IEVsize_t1;
                        break;
                    case OP_STMIB: case OP_STMFA:
                        op = c->getAddrMode() == IAMMode_postidx ? 0x1a : 0x18;
                        instruction |= c->IEVsize_t1;
                        break;
                    case OP_PUSH:
                    case OP_STMDB: case OP_STMFD:
                        op = c->getAddrMode() == IAMMode_postidx ? 0x12 : 0x10;
                        instruction |= c->IEVsize_t1;
                        break;
                    default: assert(0);
                }

                instruction |= op << 20;
                instruction |= c->getRegN() << 16;

                GEN4(instruction);
                break;
            }

// ==== below here, old versions of the code
/*
            case OP_STR:
            {
                unsigned instruction = 0;
                instruction |= c->getCC() << 28;
                instruction |= 0x1 << 26;  // fixed value for LDR/STR
                instruction |= 0x0 << 25;  // 0 == immediate, 1 == register offset
                instruction |= 0x1 << 24;  // 0 == pos, 1 == pre add offset
                instruction |= 0x0 << 23;  // 0 == subtract, 1 == add offset to base
                instruction |= 0x0 << 22;  // o == word, 1 == byte
                instruction |= 0x0 << 21;  // 0 == no, 1 == yes write address into base
                instruction |= 0x0 << 20;  // 0 == store, 1 == load
                instruction |= c->getRegN() << 16;   // base register
                instruction |= c->getRegD() << 12; // dest register
                switch(c->getAddrMode())
                {
                    case IAMMode_off:
                    {
                        assert(c->IFL1 == FLconst);
                        targ_ptrdiff_t value = c->IEVsize_t1;
                        if (value < 0)
                           value = -value;
                        else
                           instruction |= 1 << 23;
                        printf("value: %llx\n", (targ_size_t)value * 0xfff);
                        printf("valueu: %llux\n", (targ_size_t)value * 0xfff);
                        instruction |= (targ_size_t)value & 0xfff;
                        break;
                    }

                    default:
                        assert(0);
                }
                instruction |= 0; // immediate offset: should use c->IEVsym1 im some whay, I think.
                GEN4(instruction);
                break;
            }
*/

            default:
                assert(0);
        }
#ifdef DEBUG
        size_t csize = calccodsize(c);
        if (OFFSET() - startoffset != csize)
        {
            printf("actual: %d, calc: %d\n", (int)(OFFSET() - startoffset), (int)calccodsize(c));
            c->print();
            assert(0);
        }
#endif
    }
    FLUSH();

    Coffset = offset;
#ifdef DEBUG
    if (debugc) printf("-codout(), Coffset = x%llx\n", Coffset);
#endif
    return offset;  // ending address
}

unsigned calcblksize(code *c)
{
    unsigned size = 0;

    for (; c; c = code_next(c))
    {
        unsigned sz = calccodsize(c);
        //printf("off=%02x, sz = %d, code %p: op=%02x\n", size, sz, c, c->Iop);
        size += sz;
    }
    //printf("calcblksize(c = %p) = %d\n", c, size);
    return size;
}

unsigned calccodsize(code *c)
{
    unsigned size;

    switch(c->Iop & ESCAPEmask)
    {
        case NOP:
        case ESCAPE:
        case PADDING:
            size = 0;
            break;

        case OP_LDR:
        case OP_LDRB:
        case OP_LDRBT:
        case OP_LDRT:
        case OP_STR:
        case OP_STRB:
        case OP_STRBT:
        case OP_STRT:
            size = 4;
            if (c->IFL1 == FLextern)
                size += 8; // jmp over reloc + reloc itself
            break;

        default:
            size = 4;
            break;
    }
    // printf("op = x%02x, size = %d\n", c->Iop, size);
    return size;
}

// used by dwarf.c
int dwarf_regno(int reg)
{
    return reg;
}

// used by elfobj.c
enum ARM_ATTR_TAGS
{   
   Tag_NULL,
   Tag_File,
   Tag_Section,
   Tag_Symbol,
   Tag_CPU_raw_name,
   Tag_CPU_name,
   Tag_CPU_arch,
   Tag_CPU_arch_profile,
   Tag_ARM_ISA_use,
   Tag_THUMB_ISA_use,
   Tag_VFP_arch,
   Tag_WMMX_arch,
   Tag_NEON_arch,
   Tag_PCS_config,
   Tag_ABI_PCS_R9_use,
   Tag_ABI_PCS_RW_data,
   Tag_ABI_PCS_RO_data,
   Tag_ABI_PCS_GOT_use,
   Tag_ABI_PCS_wchar_t,
   Tag_ABI_FP_rounding,
   Tag_ABI_FP_denormal,
   Tag_ABI_FP_exceptions,
   Tag_ABI_FP_user_exceptions,
   Tag_ABI_FP_number_model,
   Tag_ABI_align8_needed,
   Tag_ABI_align8_preserved,
   Tag_ABI_enum_size,
   Tag_ABI_HardFP_use,
   Tag_ABI_VFP_args,
   Tag_ABI_WMMX_args,
   Tag_ABI_optimization_goals,
   Tag_ABI_FP_optimization_goals,
   Tag_compatibility,
};

void elf_arm_attributes()
{
    IDXSEC seg = ElfObj::getsegment(".ARM.attributes", NULL, SHT_ARM_ATTRIBUTES, 0, 0);

    Outbuffer tagbuf;

    tagbuf.writeuLEB128(Tag_File); // section tag

    int sizeoff = tagbuf.size();
    tagbuf.write32(0); // size of tagged block -- place holder to fill in after populating it

    tagbuf.writeuLEB128(Tag_CPU_name);
    tagbuf.writeString("ARM9TDMI");

    tagbuf.writeuLEB128(Tag_CPU_arch);
    tagbuf.writeuLEB128(2); // ARM v4T

    tagbuf.writeuLEB128(Tag_ARM_ISA_use);
    tagbuf.writeuLEB128(1);

    // BUG: need to add more attributes and properly conditionalize the ones that
    // are here to match what dmd does

    // stuff actual size into the right 4 bytes
    int savesize = tagbuf.size();
    tagbuf.setsize(sizeoff);
    tagbuf.write32(savesize);
    tagbuf.setsize(savesize);

    Outbuffer secbuf;
    secbuf.writeString("aeabi"); // vendor name
    secbuf.write(&tagbuf);

    Outbuffer *buf = SegData[seg]->SDbuf;

    buf->writeByte('A'); // version
    buf->write32(4 + secbuf.size()); // size of section (including size itself)
    buf->write(&secbuf);

    // bytes from the empty-main gcc produced .o file.
    // 0073:  41 2d  0  0  0 61 65 61 62 69  0  1 23  0  0  0   A-...aeabi..#...
    // 0083:   5 41 52 4d 39 54 44 4d 49  0  6  2  8  1 12  4   .ARM9TDMI.......
    // 0093:  14  1 15  1 17  3 18  1 19  1 1a  2 1e  6         .............. 
}

void ElfObj::elf_platform_sections()
{
    elf_arm_attributes();
}

void cod3_buildmodulector(Outbuffer* buf, int codeOffset, int refOffset)
{
    //assert(0);
}


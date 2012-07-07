
/***************************
 * Convert short to long.
 * For OPs16_32, OPu16_32, OPnp_fp, OPu32_64, OPs32_64
 */

code *cdshtlng(elem *e,regm_t *pretregs)
{
    assert(0);
#if 0
    code *c,*ce,*c1,*c2,*c3,*c4;
    unsigned reg;
    regm_t retregs;

    //printf("cdshtlng(e = %p, *pretregs = %s)\n", e, regm_str(*pretregs));
    int e1comsub = e->E1->Ecount;
    unsigned char op = e->Eoper;
    if ((*pretregs & (ALLREGS | mBP)) == 0)       // if don't need result in regs
        c = codelem(e->E1,pretregs,FALSE);  /* then conversion isn't necessary */

    else if (op == OPu32_64)
    {
        regm_t regm;
        tym_t tym1;

        retregs = *pretregs & mLSW;
        assert(retregs);
        tym1 = tybasic(e->E1->Ety);
        c = codelem(e->E1,&retregs,FALSE);

        regm = *pretregs & (mMSW & ALLREGS);
        if (regm == 0)                  /* *pretregs could be mES       */
            regm = mMSW & ALLREGS;
        ce = allocreg(&regm,&reg,TYint);
        if (e1comsub)
            ce = cat(ce,getregs(retregs));
        ce = movregconst(ce,reg,0,0);               /* 0 extend     */

        c = cat3(c,ce,fixresult(e,retregs | regm,pretregs));
    }
    else if (op == OPs16_32 || op == OPu16_32)
    {
        elem *e11;

        elem *e1 = e->E1;

        if (e1->Eoper == OPu8_16 && !e1->Ecount &&
                ((e11 = e1->E1)->Eoper == OPvar || (e11->Eoper == OPind && !e11->Ecount))
           )
        {   code cs;

            retregs = *pretregs & BYTEREGS;
            if (!retregs)
                retregs = BYTEREGS;
            c1 = allocreg(&retregs,&reg,TYint);
            c2 = movregconst(NULL,reg,0,0);                         //  XOR reg,reg
            c3 = loadea(e11,&cs,0x8A,reg,0,retregs,retregs);        //  MOV regL,EA
            freenode(e11);
            freenode(e1);
        }
        else if (e1->Eoper == OPvar || (e1->Eoper == OPind && !e1->Ecount))
        {   code cs;
            unsigned opcode;

            if (op == OPu16_32 && config.flags4 & CFG4speed)
                goto L2;
            retregs = *pretregs;
            c1 = allocreg(&retregs,&reg,TYint);
            opcode = (op == OPu16_32) ? 0x0FB7 : 0x0FBF; /* MOVZX/MOVSX reg,EA */
            if (op == OPs32_64)
            {
                assert(I64);
                // MOVSXD reg,e1
                c2 = loadea(e1,&cs,0x63,reg,0,0,retregs);
                code_orrex(c2, REX_W);
            }
            else
                c2 = loadea(e1,&cs,opcode,reg,0,0,retregs);
            c3 = CNIL;
            freenode(e1);
        }
        else
        {
L2:
            retregs = *pretregs;
            if (op == OPs32_64)
                retregs = mAX | (*pretregs & mPSW);
            *pretregs &= ~mPSW;             /* flags are already set        */
            c1 = codelem(e1,&retregs,FALSE);
            c2 = getregs(retregs);
            if (op == OPu16_32 && c1)
            {
                code *cx = code_last(c1);
                if (cx->Iop == 0x81 && (cx->Irm & modregrm(3,7,0)) == modregrm(3,4,0))
                {
                    // Convert AND of a word to AND of a dword, zeroing upper word
                    retregs = mask[cx->Irm & 7];
                    if (cx->Irex & REX_B)
                        retregs = mask[8 | (cx->Irm & 7)];
                    cx->Iflags &= ~CFopsize;
                    cx->IEV2.Vint &= 0xFFFF;
                    goto L1;
                }
            }
            if (op == OPs16_32 && retregs == mAX)
                c2 = gen1(c2,0x98);         /* CWDE                         */
            else if (op == OPs32_64 && retregs == mAX)
            {   c2 = gen1(c2,0x98);         /* CDQE                         */
                code_orrex(c2, REX_W);
            }
            else
            {
                reg = findreg(retregs);
                if (config.flags4 & CFG4speed && op == OPu16_32)
                {   // AND reg,0xFFFF
                    c3 = genc2(NULL,0x81,modregrmx(3,4,reg),0xFFFFu);
                }
                else
                {
                    unsigned iop = (op == OPu16_32) ? 0x0FB7 : 0x0FBF; /* MOVZX/MOVSX reg,reg */
                    c3 = genregs(CNIL,iop,reg,reg);
                }
                c2 = cat(c2,c3);
            }
L1:
            c3 = e1comsub ? getregs(retregs) : CNIL;
        }
        c4 = fixresult(e,retregs,pretregs);
        c = cat4(c1,c2,c3,c4);
    }
    else if (*pretregs & mPSW || config.target_cpu < TARGET_80286)
    {
        // OPs16_32, OPs32_64
        // CWD doesn't affect flags, so we can depend on the integer
        // math to provide the flags.
        retregs = mAX | mPSW;               // want integer result in AX
        *pretregs &= ~mPSW;                 // flags are already set
        c1 = codelem(e->E1,&retregs,FALSE);
        c2 = getregs(mDX);                  // sign extend into DX
        c2 = gen1(c2,0x99);                 // CWD/CDQ
        c3 = e1comsub ? getregs(retregs) : CNIL;
        c4 = fixresult(e,mDX | retregs,pretregs);
        c = cat4(c1,c2,c3,c4);
    }
    else
    {
        // OPs16_32, OPs32_64
        unsigned msreg,lsreg;

        retregs = *pretregs & mLSW;
        assert(retregs);
        c1 = codelem(e->E1,&retregs,FALSE);
        retregs |= *pretregs & mMSW;
        c2 = allocreg(&retregs,&reg,e->Ety);
        msreg = findregmsw(retregs);
        lsreg = findreglsw(retregs);
        c3 = genmovreg(NULL,msreg,lsreg);                           // MOV msreg,lsreg
        assert(config.target_cpu >= TARGET_80286);                  // 8088 can't handle SAR reg,imm8
        c3 = genc2(c3,0xC1,modregrm(3,7,msreg),REGSIZE * 8 - 1);    // SAR msreg,31
        c4 = fixresult(e,retregs,pretregs);
        c = cat4(c1,c2,c3,c4);
    }
    return c;
#endif
}


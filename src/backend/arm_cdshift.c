
/*********************
 * Generate code for shift left or shift right (OPshl,OPshr,OPashr,OProl,OPror).
 */

code *cdshift(elem *e,regm_t *pretregs)
{
    assert(0);
#if 0
    unsigned resreg,shiftcnt,byte;
    unsigned s1,s2,oper;
    tym_t tyml;
    int sz;
    regm_t retregs,rretregs;
    code *cg,*cl,*cr;
    code *c;
    elem *e1;
    elem *e2;
    regm_t forccs,forregs;
    bool e2isconst;

    e1 = e->E1;
    if (*pretregs == 0)                   // if don't want result
    {
        c = codelem(e1,pretregs,FALSE); // eval left leaf
        *pretregs = 0;                  // in case they got set
        return cat(c,codelem(e->E2,pretregs,FALSE));
    }

    tyml = tybasic(e1->Ety);
    sz = tysize[tyml];
    assert(!tyfloating(tyml));
    oper = e->Eoper;
    unsigned rex = (I64 && sz == 8) ? REX_W : 0;
    unsigned grex = rex << 16;

#if SCPP
    // Do this until the rest of the compiler does OPshr/OPashr correctly
    if (oper == OPshr)
        oper = (tyuns(tyml)) ? OPshr : OPashr;
#endif

    switch (oper)
    {
        case OPshl:
        s1 = 4;                     // SHL
        s2 = 2;                     // RCL
        break;
        case OPshr:
        s1 = 5;                     // SHR
        s2 = 3;                     // RCR
        break;
        case OPashr:
        s1 = 7;                     // SAR
        s2 = 3;                     // RCR
        break;
        case OProl:
        s1 = 0;                     // ROL
        break;
        case OPror:
        s1 = 1;                     // ROR
        break;
        default:
        assert(0);
    }

    unsigned sreg = ~0;                   // guard against using value without assigning to sreg
    c = cg = cr = CNIL;                   /* initialize                   */
    e2 = e->E2;
    forccs = *pretregs & mPSW;            /* if return result in CCs      */
    forregs = *pretregs & (ALLREGS | mBP); // mask of possible return regs
    e2isconst = FALSE;                    /* assume for the moment        */
    byte = (sz == 1);
    switch (e2->Eoper)
    {
        case OPconst:
            e2isconst = TRUE;               /* e2 is a constant             */
            shiftcnt = e2->EV.Vint;         /* get shift count              */
            if ((!I16 && sz <= REGSIZE) ||
                    shiftcnt <= 4 ||            /* if sequence of shifts        */
                    (sz == 2 &&
                     (shiftcnt == 8 || config.target_cpu >= TARGET_80286)) ||
                    (sz == 2 * REGSIZE && shiftcnt == 8 * REGSIZE)
               )
            {
                retregs = (forregs) ? forregs : ALLREGS;
                if (byte)
                {
                    retregs &= BYTEREGS;
                    if (!retregs)
                        retregs = BYTEREGS;
                }
                else if (sz > REGSIZE && sz <= 2 * REGSIZE && !(retregs & mMSW))
                    retregs |= mMSW & ALLREGS;
                if (s1 == 7)    /* if arithmetic right shift */
                {
                    if (shiftcnt == 8)
                        retregs = mAX;
                    else if (sz == 2 * REGSIZE && shiftcnt == 8 * REGSIZE)
                        retregs = mDX|mAX;
                }

                if (sz == 2 * REGSIZE && shiftcnt == 8 * REGSIZE &&
                        oper == OPshl &&
                        !e1->Ecount &&
                        (e1->Eoper == OPs16_32 || e1->Eoper == OPu16_32 ||
                         e1->Eoper == OPs32_64 || e1->Eoper == OPu32_64)
                   )
                {   // Handle (shtlng)s << 16
                    regm_t r;

                    r = retregs & mMSW;
                    cl = codelem(e1->E1,&r,FALSE);      // eval left leaf
                    cl = regwithvalue(cl,retregs & mLSW,0,&resreg,0);
                    cg = getregs(r);
                    retregs = r | mask[resreg];
                    if (forccs)
                    {   sreg = findreg(r);
                        cg = gentstreg(cg,sreg);
                        *pretregs &= ~mPSW;             // already set
                    }
                    freenode(e1);
                    freenode(e2);
                    break;
                }

                // See if we should use LEA reg,xxx instead of shift
                if (!I16 && shiftcnt >= 1 && shiftcnt <= 3 &&
                        (sz == REGSIZE || (I64 && sz == 4)) &&
                        oper == OPshl &&
                        e1->Eoper == OPvar &&
                        !(*pretregs & mPSW) &&
                        config.flags4 & CFG4speed
                   )
                {
                    unsigned reg;
                    regm_t regm;

                    if (isregvar(e1,&regm,&reg) && !(regm & retregs))
                    {   code cs;
                        cl = allocreg(&retregs,&resreg,e->Ety);
                        buildEA(&cs,-1,reg,1 << shiftcnt,0);
                        cs.Iop = 0x8D;
                        code_newreg(&cs,resreg);
                        cs.Iflags = 0;
                        cg = gen(NULL,&cs);             // LEA resreg,[reg * ss]
                        freenode(e1);
                        freenode(e2);
                        break;
                    }
                }

                cl = codelem(e1,&retregs,FALSE); // eval left leaf
                //assert((retregs & regcon.mvar) == 0);
                cg = getregs(retregs);          // trash these regs

                {
                    if (sz == 2 * REGSIZE)
                    {   resreg = findregmsw(retregs);
                        sreg = findreglsw(retregs);
                    }
                    else
                    {   resreg = findreg(retregs);
                        sreg = ~0;              // an invalid value
                    }
                    if (config.target_cpu >= TARGET_80286 &&
                            sz <= REGSIZE)
                    {
                        /* SHL resreg,shiftcnt  */
                        assert(!(sz == 1 && (mask[resreg] & ~BYTEREGS)));
                        c = genc2(CNIL,0xC1 ^ byte,grex | modregxrmx(3,s1,resreg),shiftcnt);
                        if (shiftcnt == 1)
                            c->Iop += 0x10;     /* short form of shift  */
                        if (I64 && sz == 1 && resreg >= 4)
                            c->Irex |= REX;
                        // See if we need operand size prefix
                        if (!I16 && oper != OPshl && sz == 2)
                            c->Iflags |= CFopsize;
                        if (forccs)
                            c->Iflags |= CFpsw;         // need flags result
                    }
                    else if (shiftcnt == 8)
                    {
                        if (!(retregs & BYTEREGS) || resreg >= 4)
                        {
                            cl = cat(cl,cg);
                            goto L1;
                        }

                        if (pass != PASSfinal && (!forregs || forregs & (mSI | mDI)))
                        {
                            // e1 might get into SI or DI in a later pass,
                            // so don't put CX into a register
                            cg = cat(cg, getregs(mCX));
                        }

                        assert(sz == 2);
                        switch (oper)
                        {
                            case OPshl:
                                /* MOV regH,regL        XOR regL,regL   */
                                assert(resreg < 4 && !rex);
                                c = genregs(CNIL,0x8A,resreg+4,resreg);
                                genregs(c,0x32,resreg,resreg);
                                break;

                            case OPshr:
                            case OPashr:
                                /* MOV regL,regH    */
                                c = genregs(CNIL,0x8A,resreg,resreg+4);
                                if (oper == OPashr)
                                    gen1(c,0x98);           /* CBW          */
                                else
                                    genregs(c,0x32,resreg+4,resreg+4); /* CLR regH */
                                break;

                            case OPror:
                            case OProl:
                                // XCHG regL,regH
                                c = genregs(CNIL,0x86,resreg+4,resreg);
                                break;

                            default:
                                assert(0);
                        }
                        if (forccs)
                            gentstreg(c,resreg);
                    }
                    else if (shiftcnt == REGSIZE * 8)   // it's an lword
                    {
                        if (oper == OPshl)
                            swap((int *) &resreg,(int *) &sreg);
                        c = genmovreg(CNIL,sreg,resreg);        // MOV sreg,resreg
                        if (oper == OPashr)
                            gen1(c,0x99);                       // CWD
                        else
                            movregconst(c,resreg,0,0);          // MOV resreg,0
                        if (forccs)
                        {       gentstreg(c,sreg);
                            *pretregs &= mBP | ALLREGS | mES;
                        }
                    }
                    else
                    {   c = CNIL;
                        if (oper == OPshl && sz == 2 * REGSIZE)
                            swap((int *) &resreg,(int *) &sreg);
                        while (shiftcnt--)
                        {   c = gen2(c,0xD1 ^ byte,modregrm(3,s1,resreg));
                            if (sz == 2 * REGSIZE)
                                gen2(c,0xD1,modregrm(3,s2,sreg));
                        }
                        if (forccs)
                            code_orflag(c,CFpsw);
                    }
                    if (sz <= REGSIZE)
                        *pretregs &= mBP | ALLREGS;     // flags already set
                }
                freenode(e2);
                break;
            }
            /* FALL-THROUGH */
        default:
            retregs = forregs & ~mCX;               /* CX will be shift count */
            if (sz <= REGSIZE)
            {
                if (forregs & ~regcon.mvar && !(retregs & ~regcon.mvar))
                    retregs = ALLREGS & ~mCX;       /* need something       */
                else if (!retregs)
                    retregs = ALLREGS & ~mCX;       /* need something       */
                if (sz == 1)
                {   retregs &= mAX|mBX|mDX;
                    if (!retregs)
                        retregs = mAX|mBX|mDX;
                }
            }
            else
            {
                if (!(retregs & mMSW))
                    retregs = ALLREGS & ~mCX;
            }
            cl = codelem(e->E1,&retregs,FALSE);     /* eval left leaf       */

            if (sz <= REGSIZE)
                resreg = findreg(retregs);
            else
            {
                resreg = findregmsw(retregs);
                sreg = findreglsw(retregs);
            }
L1:
            rretregs = mCX;                 /* CX is shift count    */
            if (sz <= REGSIZE)
            {
                cr = scodelem(e2,&rretregs,retregs,FALSE); /* get rvalue */
                cg = getregs(retregs);      /* trash these regs             */
                c = gen2(CNIL,0xD3 ^ byte,grex | modregrmx(3,s1,resreg)); /* Sxx resreg,CX */

                if (!I16 && sz == 2 && (oper == OProl || oper == OPror))
                    c->Iflags |= CFopsize;

                // Note that a shift by CL does not set the flags if
                // CL == 0. If e2 is a constant, we know it isn't 0
                // (it would have been optimized out).
                if (e2isconst)
                    *pretregs &= mBP | ALLREGS; // flags already set with result
            }
            else if (sz == 2 * REGSIZE &&
                    config.target_cpu >= TARGET_80386)
            {
                unsigned hreg = resreg;
                unsigned lreg = sreg;
                unsigned rex = I64 ? (REX_W << 16) : 0;
                if (e2isconst)
                {
                    cr = NULL;
                    cg = getregs(retregs);
                    if (shiftcnt & (REGSIZE * 8))
                    {
                        if (oper == OPshr)
                        {   //      SHR hreg,shiftcnt
                            //      MOV lreg,hreg
                            //      XOR hreg,hreg
                            c = genc2(NULL,0xC1,rex | modregrm(3,s1,hreg),shiftcnt - (REGSIZE * 8));
                            c = genmovreg(c,lreg,hreg);
                            c = movregconst(c,hreg,0,0);
                        }
                        else if (oper == OPashr)
                        {   //      MOV     lreg,hreg
                            //      SAR     hreg,31
                            //      SHRD    lreg,hreg,shiftcnt
                            c = genmovreg(NULL,lreg,hreg);
                            c = genc2(c,0xC1,rex | modregrm(3,s1,hreg),(REGSIZE * 8) - 1);
                            c = genc2(c,0x0FAC,rex | modregrm(3,hreg,lreg),shiftcnt - (REGSIZE * 8));
                        }
                        else
                        {   //      SHL lreg,shiftcnt
                            //      MOV hreg,lreg
                            //      XOR lreg,lreg
                            c = genc2(NULL,0xC1,rex | modregrm(3,s1,lreg),shiftcnt - (REGSIZE * 8));
                            c = genmovreg(c,hreg,lreg);
                            c = movregconst(c,lreg,0,0);
                        }
                    }
                    else
                    {
                        if (oper == OPshr || oper == OPashr)
                        {   //      SHRD    lreg,hreg,shiftcnt
                            //      SHR/SAR hreg,shiftcnt
                            c = genc2(NULL,0x0FAC,rex | modregrm(3,hreg,lreg),shiftcnt);
                            c = genc2(c,0xC1,rex | modregrm(3,s1,hreg),shiftcnt);
                        }
                        else
                        {   //      SHLD hreg,lreg,shiftcnt
                            //      SHL  lreg,shiftcnt
                            c = genc2(NULL,0x0FA4,rex | modregrm(3,lreg,hreg),shiftcnt);
                            c = genc2(c,0xC1,rex | modregrm(3,s1,lreg),shiftcnt);
                        }
                    }
                    freenode(e2);
                }
                else if (config.target_cpu >= TARGET_80486 && REGSIZE == 2)
                {
                    cr = scodelem(e2,&rretregs,retregs,FALSE); // get rvalue in CX
                    cg = getregs(retregs);          // modify these regs
                    if (oper == OPshl)
                    {
                        /*
                           SHLD    hreg,lreg,CL
                           SHL     lreg,CL
                           */

                        c = gen2(NULL,0x0FA5,modregrm(3,lreg,hreg));
                        gen2(c,0xD3,modregrm(3,4,lreg));
                    }
                    else
                    {
                        /*
                           SHRD    lreg,hreg,CL
                           SAR             hreg,CL

                           -- or --

                           SHRD    lreg,hreg,CL
                           SHR             hreg,CL
                           */
                        c = gen2(NULL,0x0FAD,modregrm(3,hreg,lreg));
                        gen2(c,0xD3,modregrm(3,s1,hreg));
                    }
                }
                else
                {   code *cl1,*cl2;

                    cr = scodelem(e2,&rretregs,retregs,FALSE); // get rvalue in CX
                    cg = getregs(retregs | mCX);            // modify these regs
                    // TEST CL,0x20
                    c = genc2(NULL,0xF6,modregrm(3,0,CX),REGSIZE * 8);
                    if (oper == OPshl)
                    {
                        /*  TEST    CL,20H
                            JNE     L1
                            SHLD    hreg,lreg,CL
                            SHL     lreg,CL
                            JMP     L2
L1: AND     CL,20H-1
SHL     lreg,CL
MOV     hreg,lreg
XOR     lreg,lreg
L2: NOP
*/

                        cl1 = NULL;
                        if (REGSIZE == 2)
                            cl1 = genc2(NULL,0x80,modregrm(3,4,CX),REGSIZE * 8 - 1);
                        cl1 = gen2(cl1,0xD3,modregrm(3,4,lreg));
                        genmovreg(cl1,hreg,lreg);
                        genregs(cl1,0x31,lreg,lreg);

                        genjmp(c,JNE,FLcode,(block *)cl1);
                        gen2(c,0x0FA5,modregrm(3,lreg,hreg));
                        gen2(c,0xD3,modregrm(3,4,lreg));
                    }
                    else
                    {   if (oper == OPashr)
                        {
                            /*  TEST        CL,20H
                                JNE         L1
                                SHRD        lreg,hreg,CL
                                SAR         hreg,CL
                                JMP         L2
L1: AND         CL,15
MOV         lreg,hreg
SAR         hreg,31
SHRD        lreg,hreg,CL
L2: NOP
*/

                            cl1 = NULL;
                            if (REGSIZE == 2)
                                cl1 = genc2(NULL,0x80,modregrm(3,4,CX),REGSIZE * 8 - 1);
                            cl1 = genmovreg(cl1,lreg,hreg);
                            genc2(cl1,0xC1,modregrm(3,s1,hreg),31);
                            gen2(cl1,0x0FAD,modregrm(3,hreg,lreg));
                        }
                        else
                        {
                            /*  TEST        CL,20H
                                JNE         L1
                                SHRD        lreg,hreg,CL
                                SHR         hreg,CL
                                JMP         L2
L1: AND         CL,15
SHR         hreg,CL
MOV         lreg,hreg
XOR         hreg,hreg
L2: NOP
*/

                            cl1 = NULL;
                            if (REGSIZE == 2)
                                cl1 = genc2(NULL,0x80,modregrm(3,4,CX),REGSIZE * 8 - 1);
                            cl1 = gen2(cl1,0xD3,modregrm(3,5,hreg));
                            genmovreg(cl1,lreg,hreg);
                            genregs(cl1,0x31,hreg,hreg);
                        }
                        genjmp(c,JNE,FLcode,(block *)cl1);
                        gen2(c,0x0FAD,modregrm(3,hreg,lreg));
                        gen2(c,0xD3,modregrm(3,s1,hreg));
                    }
                    cl2 = gennop(NULL);
                    genjmp(c,JMPS,FLcode,(block *)cl2);
                    c = cat3(c,cl1,cl2);
                }
                break;
            }
            else if (sz == 2 * REGSIZE)
            {
                c = CNIL;
                if (!e2isconst)                     // if not sure shift count != 0
                    c = genc2(c,0xE3,0,6);      // JCXZ .+6
                cr = scodelem(e2,&rretregs,retregs,FALSE);
                cg = getregs(retregs | mCX);
                if (oper == OPshl)
                    swap((int *) &resreg,(int *) &sreg);
                c = gen2(c,0xD1,modregrm(3,s1,resreg));
                code_orflag(c,CFtarg2);
                gen2(c,0xD1,modregrm(3,s2,sreg));
                genc2(c,0xE2,0,(targ_uns)-6);               // LOOP .-6
                regimmed_set(CX,0);         // note that now CX == 0
            }
            else
                assert(0);
            break;
    }
    c = cat(c,fixresult(e,retregs,pretregs));
    return cat4(cl,cr,cg,c);
#endif
}


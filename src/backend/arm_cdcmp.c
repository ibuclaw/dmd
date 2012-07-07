
/*****************************
 * Return a jump opcode relevant to the elem for a JMP TRUE.
 */

int jmpopcode(elem *e)
{
    int zero,i,jp;
    static const char jops[][2][6] =
    {   /* <=       >        <        >=       ==       !=     */
        { {COND_LE, COND_GT, COND_LT, COND_GE, COND_EQ ,COND_NE}, /* signed */
        /* <=0      >0       <0       >=0      ==0      !=0    */
          {COND_LE, COND_GT, COND_MI, COND_PL, COND_EQ, COND_NE} }, /* signed   */

        /* <=       >        <        >=       ==       !=     */
        { {COND_LS, COND_HI, COND_CS, COND_CC, COND_EQ, COND_NE},   /* unsigned */
        /* <=0      >0       <0       >=0      ==0      !=0    */
          {COND_EQ, COND_NE, COND_MI, COND_PL, COND_EQ, COND_NE} }, /* unsigned */
    };

#if 0
#define XP      (JP  << 8)
#define XNP     (JNP << 8)
    static const unsigned jfops[1][26] =
        /*   le     gt lt     ge  eqeq    ne     unord lg  leg  ule ul uge  */
    {
        { XNP|JBE,JA,XNP|JB,JAE,XNP|JE, XP|JNE,JP,   JNE,JNP, JBE,JC,XP|JAE,

            /*  ug    ue ngt nge nlt    nle    ord nlg nleg nule nul nuge    nug     nue */
            XP|JA,JE,JBE,JB, XP|JAE,XP|JA, JNP,JE, JP,  JA,  JNC,XNP|JB, XNP|JBE,JNE        }, /* 8087     */
    };
#endif

    assert(e);
    while (e->Eoper == OPcomma ||
            /* The !EOP(e->E1) is to line up with the case in cdeq() where  */
            /* we decide if mPSW is passed on when evaluating E2 or not.    */
            (e->Eoper == OPeq && !EOP(e->E1)))
        e = e->E2;                      /* right operand determines it  */

    int op = e->Eoper;
    if (e->Ecount != e->Ecomsub)          // comsubs just get Z bit set
        return COND_NE;

    if (!OTrel(op))                       // not relational operator
    {
        assert(0);
#if 0
        tym_t tymx = tybasic(e->Ety);
        if (tyfloating(tymx) && config.inline8087 &&
            (tymx == TYldouble || tymx == TYildouble || tymx == TYcldouble ||
            tymx == TYcdouble || tymx == TYcfloat ||
            op == OPind))
        {
            return XP|JNE;
        }
        return (op >= OPbt && op <= OPbts) ? JC : JNE;
#endif
    }

    if (e->E2->Eoper == OPconst)
        zero = !boolres(e->E2);
    else
        zero = 0;

    tym_t tym = e->E1->Ety;
    if (tyfloating(tym))
    {
        assert(0);
#if 0
        i = 0;
        if (config.inline8087)
        {
            i = 1;

            if (rel_exception(op) || config.flags4 & CFG4fastfloat)
            {
                if (zero)
                {
                }
                else if (cmporder87(e->E2))
                    op = swaprel(op);
                else
                    ;
            }
            else
            {
                if (zero && config.target_cpu < TARGET_80386)
                    ;
                else
                    op = swaprel(op);
            }
        }
        jp = jfops[0][op - OPle];
        goto L1;
#endif
    }
    else if (tyuns(tym) || tyuns(e->E2->Ety))
        i = 1;
    else if (tyintegral(tym) || typtr(tym))
        i = 0;
    else
    {
#if DEBUG
        elem_print(e);
        WRTYxx(tym);
#endif
        assert(0);
    }

    jp = jops[i][zero][op - OPle];        /* table starts with OPle       */
L1:
#if DEBUG
    if (jp < 0 || jp > 15)
        WROP(op),
            printf("i %d zero %d op 0x%x jp 0x%x\n", i, zero, op, jp);
#endif
    return jp;
}

int cdcmp_flag; // used by cdcond to control parts of cdcmp.. major hack

code* cdcmp(elem *e, regm_t *pretregs)
{
    regm_t retregs,rretregs;
    unsigned reg,rreg;
    code *cl,*cr,*c,cs,*ce,*cg;
    int fl;

    //printf("cdcmp(e = %p, retregs = %s)\n",e,regm_str(*pretregs));
    // Collect extra parameter. This is pretty ugly...
    int flag = cdcmp_flag;
    cdcmp_flag = 0;

    elem* e1 = e->E1;
    elem* e2 = e->E2;
    if (*pretregs == 0)                   // if don't want result
    {
        cl = codelem(e1,pretregs,FALSE);
        *pretregs = 0;                    // in case e1 changed it
        cr = codelem(e2,pretregs,FALSE);
        return cat(cl,cr);
    }

    unsigned jop = jmpopcode(e);         // must be computed before leaves are free'd
    unsigned op = e->Eoper;
    assert(OTrel(op));
    bool eqorne = (op == OPeqeq) || (op == OPne);
    tym_t tym = tybasic(e1->Ety);
    unsigned sz = tysize[tym];
    unsigned byte = sz == 1;

    cl = cr = CNIL;

    if (tyfloating(tym))                  /* if floating operation        */
    {
        retregs = mPSW;
        assert(0);
        //c = orth87(e,&retregs); // ARM TODO
        goto L3;
    }

    /* If it's a signed comparison of longs, we have to call a library    */
    /* routine, because we don't know the target of the signed branch     */
    /* (have to set up flags so that jmpopcode() will do it right)        */
    if (!eqorne && (tym == TYllong && tybasic(e2->Ety) == TYllong))
    {
        assert(0);
#if 0
        retregs = mDX | mAX;
        cl = codelem(e1,&retregs,FALSE);
        retregs = mCX | mBX;
        cr = scodelem(e2,&retregs,mDX | mAX,FALSE);

        /* Generate:
         *      CMP  EDX,ECX
         *      JNE  C1
         *      XOR  EDX,EDX
         *      CMP  EAX,EBX
         *      JZ   C1
         *      JA   C3
         *      DEC  EDX
         *      JMP  C1
         * C3:  INC  EDX
         * C1:
         */
        c = getregs(mDX);
        c = genregs(c,0x39,CX,DX);         // CMP EDX,ECX
        code *c1 = gennop(CNIL);
        genjmp(c,JNE,FLcode,(block *)c1);  // JNE C1
        movregconst(c,DX,0,0);             // XOR EDX,EDX
        genregs(c,0x39,BX,AX);             // CMP EAX,EBX
        genjmp(c,JE,FLcode,(block *)c1);   // JZ C1
        code *c3 = gen1(CNIL,0x40 + DX);   // INC EDX
        genjmp(c,JA,FLcode,(block *)c3);   // JA C3
        gen1(c,0x48 + DX);                 // DEC EDX
        genjmp(c,JMPS,FLcode,(block *)c1); // JMP C1
        c = cat4(c,c3,c1,getregs(mDX));
        retregs = mPSW;
#endif
        goto L3;
    }

    retregs = allregs;
    if (byte)
        retregs = BYTEREGS;

    c = NULL;
    ce = NULL;
    if (sz > REGSIZE)
        ce = gennop(ce);

    switch (e2->Eoper)
    {
        default:
L2:
            cl = scodelem(e1, &retregs, 0, TRUE);        // compute left leaf
L1:
            rretregs = allregs & ~retregs;
            if (byte)
                rretregs &= BYTEREGS;
            cr = scodelem(e2, &rretregs, retregs, TRUE); // get right leaf
            if (sz < REGSIZE)
                assert(0);
            if (sz == REGSIZE)                           // CMP reg,rreg
            {
                reg = findreg(retregs);                  // get reg that e1 is in
                rreg = findreg(rretregs);
                c = gen_OP_data_proc(NULL, OP_CMP, CFpsw, reg, rreg);
            }
            else
            {
                assert(sz == 2 * REGSIZE);

                /* Compare MSW, if they're equal then compare the LSW       */
                reg = findregmsw(retregs);
                rreg = findregmsw(rretregs);
                c = gen_OP_data_proc(NULL, OP_CMP, CFpsw, reg, rreg);
                genjmp(c, COND_NE, FLcode, (block *) ce); // JNE nop

                reg = findreglsw(retregs);
                rreg = findreglsw(rretregs);
                gen_OP_data_proc(c, OP_CMP, CFpsw, reg, rreg);
            }
            break;
        case OPrelconst:
            fl = el_fl(e2);
            switch (fl)
            {
                case FLfunc:
                    fl = FLextern;          // so it won't be self-relative
                    break;
                case FLdata:
                case FLudata:
                case FLextern:
                    if (sz > REGSIZE)       // compare against DS, not DGROUP
                        goto L2;
                    break;
                default:
                    goto L2;
            }
            cs.IFL1 = fl;
            cs.IEVsym1 = e2->EV.sp.Vsym;
            if (sz > REGSIZE)
            {
                assert(0);
                //cs.Iflags |= CFseg;
                //cs.IEVoffset1 = 0;
            }
            else
            {
                cs.Iflags |= CFoff;
                cs.IEVoffset1 = e2->EV.sp.Voffset;
            }
            goto L4;

        case OPconst:
            // If compare against 0
            if (sz <= REGSIZE && *pretregs == mPSW && !boolres(e2) && isregvar(e1,&retregs,&reg))
            {
                // Just do a TEST instruction
                c = gen_OP_data_proc(NULL, OP_CMP, CFpsw, reg, reg);
                retregs = mPSW;
                break;
            }

            if (!tyuns(tym) && !tyuns(e2->Ety) &&
                !boolres(e2) && !(*pretregs & mPSW) &&
                (sz == REGSIZE))
            {
                assert(*pretregs & (allregs));
                cl = codelem(e1,pretregs,FALSE);
                reg = findreg(*pretregs);
                c = getregs(mask[reg]);
                assert(0);
#if 0
                switch (op)
                {
                    case OPle:
                        c = genc2(c,0x81,grex | modregrmx(3,0,reg & 7),(unsigned)-1);   // ADD reg,-1
                        code_orflag(c, CFpsw);
                        genc2(c,0x81,grex | modregrmx(3,2,reg & 7),0);          // ADC reg,0
                        goto oplt;
                    case OPgt:
                        c = gen2(c,0xF7,grex | modregrmx(3,3,reg & 7));         // NEG reg
                        goto oplt;
                    case OPlt:
oplt:
                        c = genc2(c,0xC1,grex | modregrmx(3,5,reg),sz * 8 - 1); // SHR reg,31
                        break;
                    case OPge:
                        c = genregs(c,0xD1,4,reg);          /* SHL reg,1    */
                        code_orflag(c, CFpsw);
                        genregs(c,0x19,reg,reg);            /* SBB reg,reg  */
                        c = gen1(c,0x40 + reg);                 // INC reg
                        break;

                    default:
                        assert(0);
                }
#endif
                freenode(e2);
                goto ret;
            }

            cs.IFL1 = FLconst;
            if (sz == 16)
                cs.IEV1.Vsize_t = e2->EV.Vcent.msw;
            else if (sz > REGSIZE)
                cs.IEV1.Vint = MSREG(e2->EV.Vllong);
            else
                cs.IEV1.Vsize_t = e2->EV.Vllong;

L4:
            if (evalinregister(e2) && !OTassign(e1->Eoper) && !isregvar(e1,NULL,NULL))
            {
                regm_t m = allregs & ~regcon.mvar;
                if (byte)
                    m &= BYTEREGS;
                if (m & (m - 1))    // if more than one free register
                    goto L2;
            }

            if ((e1->Eoper == OPstrcmp || (OTassign(e1->Eoper) && sz <= REGSIZE)) &&
                !boolres(e2) && !evalinregister(e1))
            {
                retregs = mPSW;
                cl = scodelem(e1, &retregs, 0, FALSE);
                freenode(e2);
                break;
            }

            if (sz <= REGSIZE && !boolres(e2) && e1->Eoper == OPadd && *pretregs == mPSW)
            {
                retregs |= mPSW;
                cl = scodelem(e1, &retregs, 0, FALSE);
                freenode(e2);
                break;
            }

            cl = scodelem(e1, &retregs, 0, TRUE);      /* compute left leaf    */

            if (sz == 1)
            {
                assert(0);
#if 0
                cs.Iop = 0x81 ^ byte;
                reg = findreg(retregs & allregs);   // get reg that e1 is in
                cs.Irm = modregrm(3,7,reg & 7);
                if (reg & 8)
                    cs.Irex |= REX_B;
                if (e1->Eoper == OPvar && e1->EV.sp.Voffset == 1 && e1->EV.sp.Vsym->Sfl == FLreg)
                {   assert(reg < 4);
                    cs.Irm |= 4;                    // use upper register half
                }
                if (I64 && reg >= 4)
                    cs.Irex |= REX;                 // address byte registers
                c = gen(c,&cs);                         /* CMP sucreg,LSW       */
#endif
            }
            else if (sz <= REGSIZE)
            {                                       /* CMP reg,const        */
                reg = findreg(retregs & allregs);   // get reg that e1 is in
                rretregs = allregs & ~retregs;
                if (cs.IFL1 == FLconst && reghasvalue(rretregs, cs.IEV1.Vsize_t, &rreg))
                {
                    c = gen_OP_data_proc(c, OP_CMP, CFpsw, reg, rreg);
                    freenode(e2);
                    break;
                }
                c = gen_OP_data_proc_immed(c, OP_CMP, CFpsw, reg, reg, cs.IEV1.Vsize_t);
            }
            else if (sz <= 2 * REGSIZE)
            {
                assert(0);
#if 0
                cs.Iop = 0x81 ^ byte;
                reg = findregmsw(retregs);          // get reg that e1 is in
                cs.Irm = modregrm(3,7,reg);
                c = gen(CNIL,&cs);                  /* CMP reg,MSW          */
                if (I32 && sz == 6)
                    c->Iflags |= CFopsize;          /* seg is only 16 bits  */
                genjmp(c,JNE,FLcode,(block *) ce);  /* JNE ce               */

                reg = findreglsw(retregs);
                cs.Irm = modregrm(3,7,reg);
                if (e2->Eoper == OPconst)
                    cs.IEV2.Vint = e2->EV.Vlong;
                else if (e2->Eoper == OPrelconst)
                {   /* Turn off CFseg, on CFoff     */
                    cs.Iflags ^= CFseg | CFoff;
                    cs.IEVoffset2 = e2->EV.sp.Voffset;
                }
                else
                    assert(0);
                c = gen(c,&cs);                         /* CMP sucreg,LSW       */
#endif
            }
            else
                assert(0);
            freenode(e2);
            break;

        case OPind:
            if (e2->Ecount)
                goto L2;
            goto L5;

        case OPvar:
            if ((e1->Eoper == OPvar && isregvar(e2,&rretregs,&reg) && sz <= REGSIZE) ||
                (e1->Eoper == OPind && isregvar(e2,&rretregs,&reg) && !evalinregister(e1) && sz <= REGSIZE))
            {
                assert(0);
#if 0
                // CMP EA,e2
                cl = getlvalue(&cs,e1,RMload);
                freenode(e1);
                cs.Iop = 0x39 ^ byte ^ reverse;
                code_newreg(&cs,reg);
                if (I64 && byte && reg >= 4)
                    cs.Irex |= REX;                 // address byte registers
                c = gen(c,&cs);
                freenode(e2);
#endif
                break;
            }
L5:
            cl = scodelem(e1, &retregs, 0, TRUE);   /* compute left leaf    */
            if (sz < REGSIZE)                       /* CMP reg,EA           */
                assert(0);
            else if (sz == REGSIZE)                 /* CMP reg,EA           */
            {
                reg = findreg(retregs & allregs);   // get reg that e1 is in
                cs.Iflags |= CFpsw;
                c = cat(c, loadea(e2, &cs, OP_CMP, reg, 0, RMload | retregs, 0));
            }
            else if (sz <= 2 * REGSIZE)
            {
                assert(0);
#if 0
                reg = findregmsw(retregs);   /* get reg that e1 is in */
                // CMP reg,EA
                c = loadea(e2,&cs,0x3B ^ reverse,reg,REGSIZE,RMload | retregs,0);
                if (I32 && sz == 6)
                    c->Iflags |= CFopsize;          /* seg is only 16 bits  */
                genjmp(c,JNE,FLcode,(block *) ce);          /* JNE ce       */
                reg = findreglsw(retregs);
                if (e2->Eoper == OPind)
                {
                    NEWREG(cs.Irm,reg);
                    getlvalue_lsw(&cs);
                    c = gen(c,&cs);
                }
                else
                    c = cat(c,loadea(e2,&cs,0x3B ^ reverse,reg,0,RMload | retregs,0));
#endif
            }
            else
                assert(0);
            freenode(e2);
            break;
    }
    c = cat(c, ce);

L3:
    if ((retregs = (*pretregs & (ALLREGS))) != 0) // if return result in register
    {
        code *nop = NULL;
        regm_t save = regcon.immed.mval;
        cg = allocreg(&retregs, &reg, TYint);
        regcon.immed.mval = save;
        if ((*pretregs & mPSW) == 0 && (jop == COND_VS || jop == COND_VC))
        {
            cg = cat(cg, getregs(retregs));
            cg = gen_OP_data_proc(cg, OP_SBC, CFpsw, reg, reg);
            if (flag)
                ; // cdcond() will handle it
            else if (jop == COND_VS)
                gen_OP_data_proc_immed(cg, OP_ADD, CFpsw, reg, reg, 1);
            else
                gen_OP_data_proc(cg, OP_MVN, CFpsw, reg, reg);
        }
        else
        {
            assert(!flag);
            cg = gen_OP_data_proc_immed(cg, OP_MOV, CFpsw, reg, reg, 1);
            nop = gennop(nop);
            cg = genjmp(cg, jop, FLcode, (block *) nop);   // Jtrue nop
            gen_OP_data_proc_immed(cg, OP_MOV, CFpsw, reg, reg, 0);
        }
        *pretregs = retregs;
        c = cat3(c, cg, nop);
    }
ret:
    return cat3(cl, cr, c);
}

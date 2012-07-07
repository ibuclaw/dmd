
code* cdaddass(elem *e, regm_t *pretregs)
{
    //printf("cdaddass(e=%p, *pretregs = %s)\n", e, regm_str(*pretregs));

    unsigned reg,op1,op2,mode;
    code *cl,*cr,*c,*ce,cs;
    elem *e2;
    regm_t varregm;
    unsigned varreg;

    unsigned op = e->Eoper;
    regm_t retregs = 0;
    unsigned reverse = 0;
    elem *e1 = e->E1;
    tym_t tyml = tybasic(e1->Ety);              // type of lvalue
    int sz = tysize[tyml];
    int byte = (sz == 1);                     // 1 for byte operation, else 0

    if (tyfloating(tyml))
    {
        assert(0);
#if 0
        if (op == OPnegass)
            c = cdnegass87(e,pretregs);
        else
            c = opass87(e,pretregs);
#endif
        return c;
    }

    unsigned opsize = 0;
    unsigned cflags = 0;
    regm_t forccs = *pretregs & mPSW;            // return result in flags
    regm_t forregs = *pretregs & ~mPSW;          // return result in regs
    /* TRUE if we want the result in a register   */
    unsigned wantres = forregs || (e1->Ecount && EOP(e1));

    switch (op)                   /* select instruction opcodes           */
    {
        case OPpostinc: op = OPaddass;                  /* i++ => +=    */
        case OPaddass:  op1 = OP_ADD; op2 = OP_ADC;
                        cflags = CFpsw;
                        mode = 0;
                        break;
        case OPpostdec: op = OPminass;                  /* i-- => -=    */
        case OPminass:  op1 = OP_SUB; op2 = OP_SBC;
                        cflags = CFpsw;
                        mode = 5;
                        break;
        case OPandass:  op1 = op2 = OP_AND;
                        mode = 4;
                        break;
        case OPorass:   op1 = op2 = OP_ORR;
                        mode = 1;
                        break;
        case OPxorass:  op1 = op2 = OP_EOR;
                        mode = 6;
                        break;
        case OPnegass:  op1 = OP_MVN;
                        break;
        default:
                        assert(0);
    }

    if (op == OPnegass)
    {
        assert(0);
#if 0
        cl = getlvalue(&cs, e1, 0);
        cr = modEA(&cs);
        cs.Irm |= modregrm(0,3,0);
        cs.Iop = op1;
        switch (tysize[tyml])
        {
            case CHARSIZE:
                c = gen(NULL, &cs);
                break;

            case SHORTSIZE:
                c = gen(NULL, &cs);
                if (*pretregs & mPSW)
                    c->Iflags |= CFopsize | CFpsw;
                break;

            case LONGSIZE:
                c = gen(CNIL,&cs);
                c->Iflags |= opsize;
                break;

            case LLONGSIZE:
                getlvalue_msw(&cs);
                c = gen(NULL, &cs);              // NEG EA+2
                getlvalue_lsw(&cs);
                gen(c, &cs);                     // NEG EA
                code_orflag(c, CFpsw);
                cs.Iop = 0x81;
                getlvalue_msw(&cs);
                cs.IFL2 = FLconst;
                cs.IEV2.Vuns = 0;
                gen(c, &cs);                     // SBB EA+2,0
                break;

            default:
                assert(0);
        }
        c = cat3(cl, cr, c);
        forccs = 0;             // flags already set by NEG
        *pretregs &= ~mPSW;
#endif
    }
    else // evaluate e2 into register
    {
        assert(0);
#if 0
        retregs = (byte) ? BYTEREGS : ALLREGS;  // pick working reg
        cr = scodelem(e->E2, &retregs, 0, TRUE);// get rvalue
        cl = getlvalue(&cs, e1, retregs);       // get lvalue
        cs.Iop = op1;
        if (sz <= REGSIZE)
        {
            reg = findreg(retregs);
            cs.setRegN(reg);
        }
        else if (sz == 2 * REGSIZE)
        {
            assert(0);
#if 0
            cs.Irm |= modregrm(0,findreglsw(retregs),0);
            cl = gen(cl,&cs);                           /* OP1 EA,reg+1 */
            code_orflag(cl,cflags);
            cs.Iop = op2;
            NEWREG(cs.Irm,findregmsw(retregs)); /* OP2 EA+1,reg */
            getlvalue_msw(&cs);
#endif
        }
        else
            assert(0);
        cl = gen(cl, &cs);
        c = cat(cr, cl);
        retregs = 0;            /* to trigger a bug if we attempt to use it */
#endif
    }

    /* See if we need to reload result into a register.   */
    /* Need result in registers in case we have a 32 bit  */
    /* result and we want the flags as a result.          */
    if (wantres || (sz > REGSIZE && forccs))
    {
        assert(0);
#if 0
        if (sz <= REGSIZE)
        {
            regm_t possregs = ALLREGS;
            if (byte)
                possregs = BYTEREGS;
            retregs = forregs & possregs;
            if (!retregs)
                retregs = possregs;

            // If reg field is destination
            if (cs.Iop & 2 && cs.Iop < 0x40 && (cs.Iop & 7) <= 5)
            {
                reg = (cs.Irm >> 3) & 7;
                if (cs.Irex & REX_R)
                    reg |= 8;
                retregs = mask[reg];
                ce = allocreg(&retregs,&reg,tyml);
            }
            // If lvalue is a register, just use that register
            else if ((cs.Irm & 0xC0) == 0xC0)
            {
                reg = cs.Irm & 7;
                if (cs.Irex & REX_B)
                    reg |= 8;
                retregs = mask[reg];
                ce = allocreg(&retregs,&reg,tyml);
            }
            else
            {
                ce = allocreg(&retregs,&reg,tyml);
                cs.Iop = 0x8B ^ byte ^ reverse;
                code_newreg(&cs, reg);
                if (I64 && byte && reg >= 4)
                    cs.Irex |= REX_W;
                ce = gen(ce,&cs);               // MOV reg,EA
            }
        }
        else if (sz == 2 * REGSIZE)
        {       regm_t idx;
            code *cm,*cl;

            idx = idxregm(&cs);
            retregs = forregs;
            if (!retregs)
                retregs = ALLREGS;
            ce = allocreg(&retregs,&reg,tyml);
            cs.Iop = 0x8B;
            NEWREG(cs.Irm,reg);
            cm = gen(NULL,&cs);             // MOV reg,EA+2
            NEWREG(cs.Irm,findreglsw(retregs));
            getlvalue_lsw(&cs);
            cl = gen(NULL,&cs);             // MOV reg+1,EA
            if (mask[reg] & idx)
                ce = cat3(ce,cl,cm);
            else
                ce = cat3(ce,cm,cl);
        }
        else
            assert(0);
        c = cat(c,ce);
        if (e1->Ecount)                 /* if we gen a CSE      */
            cssave(e1,retregs,EOP(e1));
#endif
    }
    freenode(e1);
    if (sz <= REGSIZE)
        *pretregs &= ~mPSW;            // flags are already set
    return cat(c,fixresult(e,retregs,pretregs));
}


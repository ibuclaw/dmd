
code* cdeq(elem *e, regm_t *pretregs)
{
    code cs;
    code *c  = NULL;
    code *cl = NULL;
    code *cr = NULL;
    elem *e1 = e->E1;
    elem *e11;
    elem *e2 = e->E2;
    regm_t varregm;
    regm_t retregs = *pretregs;
    targ_int postinc;
    tym_t tyml = tybasic(e1->Ety); // type of lvalue
    bool regvar;
    int e2oper = e2->Eoper;
    unsigned reg, exprreg;
    unsigned sz = tysize[tyml]; // # of bytes to transfer

    assert((int)sz > 0);

    if (tyfloating(tyml))
        assert(0);

    if (retregs == 0) // if no return value
        retregs = allregs; // pick a reg, any reg

    if (retregs == mPSW)
        assert(0);

    if (sz == 1)
        assert(0);

    regvar = false;
    varregm = 0;

    if (config.flags4 & CFG4optimized)
        assert(0);

    if (*pretregs & mPSW && !EOP(e1)) // if evaluating e1 couldn't change flags
    {
        retregs |= mPSW;
        *pretregs &= ~mPSW;
    }

    cr = scodelem(e2, &retregs, 0, TRUE);

    // Look for special case of (*p++ = ...), where p is a register variable
    if (e1->Eoper == OPind &&
        ((e11 = e1->E1)->Eoper == OPpostinc || e11->Eoper == OPpostdec) &&
        e11->E1->Eoper == OPvar &&
        e11->E1->EV.sp.Vsym->Sfl == FLreg
       )
    {
        postinc = e11->E2->EV.Vint;
        if (e11->Eoper == OPpostdec)
            postinc = -postinc;
        cl = getValueToReg(&cs, &exprreg, e11, RMstore | retregs);
        freenode(e11->E2);
    }
    else
    {
        postinc = 0;
        cl = getValueToReg(&cs, &exprreg, e1, RMstore | retregs);       // get lvalue (cl == CNIL if regvar)
    }
    
    cs.Iop = OP_MOV;

    c = getregs(varregm);

    reg = findreg(retregs & ((sz > REGSIZE) ? mLSW : ALLREGS));
    cs.setRegD(exprreg);
    cs.setRegM(reg);
    for (; true; sz -= REGSIZE)
    {
        // Do not generate mov from register onto itself
        if (regvar && reg == cs.getRegD())
            break;

        c = gen(c, &cs);         // MOV EA+offset,reg
        if (sz <= REGSIZE)
            break;

        assert(0);
#if 0
        getlvalue_msw(&cs);
        reg = findregmsw(retregs);
        cs.setRegD(reg);
        cs.setRegN(reg);
#endif
    }

    if (e1->Ecount || regvar)  // if lvalue is a CSE or rvalue can't be a CSE
    {
        c = cat(c, getregs_imm(retregs)); // necessary if both lvalue and
                                          //  rvalue are CSEs (since a reg
                                          //  can hold only one e at a time)
        cssave(e1, retregs, EOP(e1));     // if lvalue is a CSE
    }

    c = cat4(cr, cl, c, fixresult(e, retregs, pretregs));

Lp:
    if (postinc)
    {
        assert(0);
#if 0
        int reg = findreg(idxregm(&cs));
        if (*pretregs & mPSW)
        {
            // Use LEA to avoid touching the flags
            unsigned rm = cs.Irm & 7;
            if (cs.Irex & REX_B)
                rm |= 8;
            c = genc1(c,0x8D,buildModregrm(2,reg,rm),FLconst,postinc);
            if (sz == 8)
                code_orrex(c, REX_W);
        }
        else
        {
            if (postinc == 1)
                c = gen1(c,0x40 + reg);         // INC reg
            else if (postinc == -(targ_int)1)
                c = gen1(c,0x48 + reg);         // DEC reg
            else
            {
                c = genc2(c,0x81,modregrm(3,0,reg),postinc);
            }
        }
#endif
    }
    freenode(e1);
    return c;
}


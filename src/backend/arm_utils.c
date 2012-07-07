
/******************************
 * Given the result of an expression is in retregs,
 * generate necessary code to return result in *pretregs.
 */

code *fixresult(elem *e, regm_t retregs, regm_t *pretregs)
{
    //printf("fixresult(e = %p, retregs = %s, *pretregs = %s)\n", e, regm_str(retregs), regm_str(*pretregs));

    if (*pretregs == 0) return NULL;      // don't want the results

    assert(e && retregs);                 // need something to work with

    code *c = NULL;
    regm_t forccs  = *pretregs & mPSW;
    regm_t forregs = *pretregs & (ALLREGS | mSTACK);
    tym_t tym = tybasic(e->Ety);
    int sz = tysize[tym];

    if (tym == TYstruct)
        assert(0);

    if ((retregs & forregs) == retregs)   // if already in right registers
        *pretregs = retregs;
    else if (forregs)
    {
        // ARM TODO: float support

        code *ce = NULL;
        unsigned opsflag = FALSE;
        unsigned rreg;

        c = allocreg(pretregs, &rreg, tym); /* allocate return regs   */
        if (sz > REGSIZE)
        {
            unsigned msreg = findregmsw(retregs);
            unsigned lsreg = findreglsw(retregs);
            unsigned msrreg = findregmsw(*pretregs);
            unsigned lsrreg = findreglsw(*pretregs);

            ce = gen_OP_data_proc(ce, OP_MOV, 0, msrreg, msreg);
            ce = gen_OP_data_proc(ce, OP_MOV, 0, lsrreg, lsreg);
        }
        else
        {
            unsigned reg = findreg(retregs & ALLREGS);
            ce = gen_OP_data_proc(ce, OP_MOV, 0, rreg, reg);
        }
        c = cat(c, ce);
        cssave(e, retregs | *pretregs, opsflag);
        forregs = 0;                    /* don't care about result in reg  */
                                        /* cuz we have real result in rreg */
        retregs = *pretregs & ~mPSW;
    }

    if (forccs)                           // if return result in flags
        c = cat(c, tstresult(retregs, tym, forregs));

    return c;
}

#if 0
void getlvalue_msw(code *c)
{
    if (c->IFL1 == FLreg)
        c->setRegM(c->IEVsym1->Sregmsw);
    else
    {
        c->setShiftImm(1);
        c->IEVoffset1 += REGSIZE;
    }
}
#endif

/******************
 * Given an expression e, get it into a regsiter and return which one
 * Generate and return sequence of code (if any).
 * Params:
 *      pcs ->  where to set flags (ex, volatile)
 *      reg ->  where to store which register holds the result
 *      e ->    expression to get to a register
 *      keepmsk mask of registers we must not destroy or use
 *              if (keepmsk & RMstore), this will be only a store operation into the lvalue
 *              if (keepmsk & RMload), this will be a read operation only
 */

code *getValueToReg(code *pcs, unsigned *reg, elem *e, regm_t keepmsk)
{
#ifdef DEBUG
    if (debugw)
    {
        printf("getValueToReg(e = %p, km = x%x)\n", e, keepmsk);
        elem_print(e);
    }
#endif
    assert(e);
    elem_debug(e);

    unsigned fl = 0;
    symbol *s = NULL;

    if (e->Eoper == OPvar || e->Eoper == OPrelconst)
    {
        s = e->EV.sp.Vsym;
        fl = s->Sfl;
        if (tyfloating(s->ty()))
            assert(0);
    }
    else
        fl = FLoper;

    tym_t ty = e->Ety;
    unsigned sz = tysize(ty);

    if (tyfloating(ty))
        assert(0);

    if (ty & mTYvolatile)
        pcs->Iflags |= CFvolatile;

    code* c = NULL;
    unsigned offsetReg = NOREG;
    switch (fl)
    {
        case FLauto:
            if (s->Sclass == SCfastpar && regcon.params & s->Spregm())
            {
                if (keepmsk & RMload)
                {
                    if (sz == REGSIZE)      // could this be (sz <= REGSIZE) ?
                    {
                        reg_t tpreg = s->Spreg;
                        if (e->EV.sp.Voffset == REGSIZE)
                            tpreg = s->Spreg2;
                        assert(tpreg != NOREG);
                        if (regcon.params & mask[tpreg])
                        {
                            *reg = tpreg;
                            regcon.used |= mask[tpreg];
                            break;
                        }
                    }
                }
                else
                    regcon.params &= ~s->Spregm();
            }
        case FLfast:
        case FLbprel:
        case FLpara:
            reflocal = TRUE;
            offsetReg = FP;
            goto L2;

        case FLreg:
            if (s->Sclass == SCregpar || s->Sclass == SCparameter)
            {
                refparam = TRUE;
                reflocal = TRUE;
            }
            *reg = s->Sreglsw;
            assert(!e->EV.sp.Voffset);
            break;

        case FLoper:
        {
            elem *e1;
            switch (e->Eoper)
            {
                case OPadd:                 // this way when we want to do LEA
                    e1 = e;
                    break;
                case OPind:
                case OPpostinc:             // when doing (*p++ = ...)
                case OPpostdec:             // when doing (*p-- = ...)
                case OPbt:
                case OPbtc:
                case OPbtr:
                case OPbts:
                    e1 = e->E1;
                    break;
                default:
                    assert(0);
            }

            regm_t idxregs = IDXREGS & ~keepmsk; // only these can be index regs
            assert(idxregs);
            if (sz == REGSIZE && keepmsk & RMstore)
                idxregs |= regcon.mvar;

            c = scodelem(e1, &idxregs, keepmsk, TRUE); /* load index register  */
            offsetReg = findreg(idxregs & ALLREGS);
            break;
        }

        case FLfltreg:
        case FLextern:
        case FLdata:
        case FLudata:
        case FLgot:
        case FLgotoff:
        case FLtlsdata:
        case FLfunc:                                /* reading from code seg */
        case FLstack:
        case FLdatseg:
        case FLpseudo:
            assert(0);

        default:
#ifdef DEBUG
            WRFL((enum FL)fl);
            symbol_print(s);
#endif
            assert(0);

L2:
        assert(offsetReg != NOREG); // how'd we get here without an offset reg?
        assert(sz == REGSIZE); // need to think about how to handle > REGSIZE

        // not yet in a register, must get it into one
 
        regm_t regs = ALLREGS & ~keepmsk;
        assert(regs);
        if (sz == REGSIZE && keepmsk & RMstore)
            regs |= regcon.mvar;
        assert(regs);
        *reg = findreg(regs);

        c = gen_OP_LDR(c, pcs->Iflags, *reg, offsetReg, fl, s, e->EV.sp.Voffset);

        if (sz == 1)
        {
            assert(0);
#if 0
            /* Don't use SI or DI for this variable     */
            s->Sflags |= GTbyte;
            if (e->EV.sp.Voffset > 1)
                s->Sflags &= ~GTregcand;
#endif
        }
        else if (e->EV.sp.Voffset)
        {
            assert(0);
            s->Sflags &= ~GTregcand;
        }
        if (!(keepmsk & RMstore))               // if not store only
            s->Sflags |= SFLread;               // assume we are doing a read
    }
    return c;
}

code *loaddata(elem *e, regm_t *pretregs)
{
    code cs;
    code *c = NULL;

#ifdef DEBUG
    if (debugw)
        printf("loaddata(e = %p,*pretregs = %s)\n",e,regm_str(*pretregs));
    //elem_print(e);
#endif
    assert(e);
    elem_debug(e);

    if (*pretregs == 0)
        return NULL;

    unsigned reg;
    tym_t tym = tybasic(e->Ety);
    int sz = tysize[tym];

    if (tym == TYstruct)
        return cdrelconst(e, pretregs);

    if (tyfloating(tym))
        assert(0);

    if (*pretregs == mPSW)
        assert(0);

    regm_t flags = *pretregs & mPSW; // save incoming flags
    regm_t forregs = *pretregs & ALLREGS;

    if (*pretregs & mSTACK)
        assert(0);

    if (e->Eoper == OPconst)
    {
        code *ce;

        targ_size_t value = e->EV.Vint;
        if (sz == 8)
            value = e->EV.Vullong;

        if (sz == REGSIZE && reghasvalue(forregs, value, &reg))
            forregs = mask[reg];

        regm_t save = regcon.immed.mval;
        c = allocreg(&forregs,&reg,tym);        /* allocate registers   */
        regcon.immed.mval = save;               // KLUDGE!
        if (sz <= REGSIZE)
        {
            if (sz == 1)
                flags |= 1;
            else if (sz == SHORTSIZE &&
                     !(mask[reg] & regcon.mvar) &&
                     !(config.flags4 & CFG4speed)
                    )
                flags |= 2;
            if (sz == 8)
                flags |= 64;
            ce = movregconst(NULL, reg, value, flags);
            flags = 0;                          // flags are already set
        }
        else if (sz == 8)
        {
            targ_long *p = (targ_long *) &e->EV.Vdouble;
            ce = movregconst(NULL, findreglsw(forregs), p[0], 0);
            ce = movregconst(ce, findregmsw(forregs), p[1], 0);
        }
        else
            assert(0);

        c = cat(c, ce);
    }
    else
    {
        // See if we can use register that parameter was passed in
        if (regcon.params && e->EV.sp.Vsym->Sclass == SCfastpar &&
            regcon.params & mask[e->EV.sp.Vsym->Spreg] &&
            !(e->Eoper == OPvar && e->EV.sp.Voffset > 0) && // Must be at the base of that variable
            sz <= REGSIZE)                  // make sure no 'paint' to a larger size happened
        {
            reg = e->EV.sp.Vsym->Spreg;
            forregs = mask[reg];
            mfuncreg &= ~forregs;
            regcon.used |= forregs;
            return fixresult(e, forregs, pretregs);
        }

        c = allocreg(&forregs, &reg, tym);            /* allocate registers   */

        if (sz == 1)
        {
            regm_t nregm;

#ifdef DEBUG
            if (!(forregs & BYTEREGS))
            {
                elem_print(e);
                printf("forregs = x%x\n",forregs);
            }
#endif
            assert(forregs & BYTEREGS);
            c = cat(c,loadea(e, &cs, OP_LDR, reg, 0, 0, 0)); // MOV reg,data
        }
        else if (sz <= REGSIZE)
        {
            c = cat(c, loadea(e, &cs, OP_LDR, reg, 0, RMload, 0)); // MOV reg,data
        }
        else if (sz < 2 * REGSIZE)
            assert(0);
        else if (sz == 2 * REGSIZE)
        {
            if (sz == 8 && (*pretregs & (mSTACK | mPSW)) == mSTACK)
                assert(0);

            reg = findregmsw(forregs);
            c = cat(c, loadea(e, &cs, OP_LDR, reg, REGSIZE, forregs, 0)); /* MOV reg,data+2 */
            reg = findreglsw(forregs);
            c = cat(c, loadea(e, &cs, OP_LDR, reg, 0, forregs, 0));
        }
        else if (sz >= 8)
            assert(0);
        else
            assert(0);
    }

    /* Flags may already be set   */
    *pretregs &= flags | ~mPSW;

    c = cat(c, fixresult(e, forregs, pretregs));
    return c;
}

/******************************
 * Routine to aid in setting things up for gen().
 * Look for common subexpression.
 * Can handle indirection operators, but not if they're common subs.
 * Input:
 *      e ->    elem where we get some of the data from
 *      cs ->   partially filled code to add
 *      op =    opcode
 *      reg =   reg to populate
 *      offset = data to be added to Voffset field
 *      keepmsk = mask of registers we must not destroy
 *      desmsk  = mask of registers destroyed by executing the instruction
 * Returns:
 *      pointer to code generated
 */

code *loadea(elem *e, code *cs, unsigned op, unsigned reg, targ_size_t offset,
        regm_t keepmsk, regm_t desmsk)
{
    code *c, *cg;

#ifdef DEBUG
    if (debugw)
        printf("loadea: e=%p cs=%p op=x%x reg=%d offset=%lld keepmsk=0x%x desmsk=0x%x\n",
                e, cs, op, reg, (unsigned long long)offset, keepmsk, desmsk);
#endif

    assert(e);

    tym_t tym = e->Ety;
    int sz = tysize(tym);

    /* Determine if location we want to get is in a register. If so,      */
    /* substitute the register for the EA.                                */
    /* Note that operators don't go through this. CSE'd operators are     */
    /* picked up by comsub().                                             */
    if (e->Ecount &&                      /* if cse                       */
        e->Ecount != e->Ecomsub)          /* and cse was generated        */
    {
        assert(!EOP(e));                /* can't handle this            */
        regm_t rm = regcon.cse.mval & ~regcon.cse.mops & ~regcon.mvar; // possible regs
        if (sz > REGSIZE)               // value is in 2 or 4 registers
        {
            if (offset)
                rm &= mMSW;             /* only high words      */
            else
                rm &= mLSW;             /* only low words       */
        }
        for (unsigned i = 0; rm; i++)
        {
            if (mask[i] & rm)
            { 
                if (regcon.cse.value[i] == e) // if register has elem
                {
                    // if the caller just wants the data in reg, then switch from LDR to MOV
                    if (op == OP_LDR) op = OP_MOV;
                    // <op> reg,i
                    cs->Iop = op;
                    cs->setRegD(reg);
                    cs->setRegN(reg);
                    cs->setRegM(i);
                    c = NULL;
                    goto L2;
                }
                rm &= ~mask[i];
            }
        }
    }

    assert(offset == 0);

    // value isn't yet in a register, so let's go get it
    if (op != OP_LDR)
    {
        // get value to a register
        unsigned tmpreg;
        c = getValueToReg(cs, &tmpreg, e, keepmsk);

        cs->Iop = op;
        cs->setRegM(tmpreg);
        cs->setRegD(reg);
        cs->setRegN(reg);
    }
    else
    {
        // get value to a register
        unsigned tmpreg;
        c = getValueToReg(cs, &tmpreg, e, keepmsk);
        return c;
#if 0
        // pre-overhaul to getlvalue
        c = getlvalue(cs, e, keepmsk);
        if (offset == REGSIZE)
            getlvalue_msw(cs);
        else
        {
            cs->setShiftImm(1);
            cs->IEVoffset1 += offset;
        }
        cs->Iop = op;
        cs->setRegD(reg);
#endif
    }

L2:
    cg = getregs(desmsk);                 /* save any regs we destroy     */

    // Eliminate MOV reg,reg
    if (cs->Iop == OP_MOV && !(cs->Iflags & CFimmed) && cs->getAddrMode() == 0)
    {
        // ARM TODO: include shifting in confirm it's a nop
        if (cs->getRegD() == cs->getRegM())
            cs->Iop = NOP;
    }

    return cat3(c, cg, gen(NULL, cs));
}

// ARM TODO: this should probably be a method in code itself
regm_t idxregm(code* c)
{
    switch(c->getAddrMode())
    {
        case 0:           return mask[c->getRegN()];
        case IAMMode_off: return mask[c->getRegM()];
        default:          assert(0);
    }
}

code *movregconst(code *c, unsigned reg, targ_size_t value, regm_t flags)
{
    if (!flags)
    {
        regimmed_set(reg, value);
        return gen_OP_data_proc_immed(NULL, OP_MOV, 0, reg, reg, value);
    }

    // TONS of code over in the x86 version
    assert(0);
    return NULL;
}

/*****************************
 * Given a type, return a mask of
 * registers to hold that type.
 * Input:
 *      tyf     function type
 */

regm_t regmask(tym_t tym, tym_t tyf)
{
    assert(I32);

    switch (tybasic(tym))
    {
        case TYvoid:
        case TYstruct:
            return 0;

        case TYbool:
        case TYwchar_t:
        case TYchar16:
        case TYchar:
        case TYschar:
        case TYuchar:
        case TYshort:
        case TYushort:
        case TYint:
        case TYuint:
#if JHANDLE
        case TYjhandle:
#endif
        case TYnullptr:
        case TYnptr:
        case TYlong:
        case TYulong:
        case TYdchar:
            return mR0;

        case TYllong:
        case TYullong:
            return mR0 | mR1;

        case TYcent:
        case TYucent:

        case TYfloat:
        case TYifloat:
        case TYcfloat:

        case TYdouble:
        case TYdouble_alias:
        case TYidouble:
        case TYcdouble:

        case TYldouble:
        case TYildouble:
        case TYcldouble:
        default:
#if DEBUG
            WRTYxx(tym);
#endif
            assert(0);
            return 0;
    }
}

code* cod3_stackadj(code* c, int nbytes)
{
    if (nbytes > 0)
        return gen_OP_data_proc_immed(c, OP_SUB, 0, SP, SP, nbytes);
    else
        return gen_OP_data_proc_immed(c, OP_ADD, 0, SP, SP, -nbytes);
}

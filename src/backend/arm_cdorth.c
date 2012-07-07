
code* cdorth(elem *e, regm_t *pretregs)
{
    code *c = NULL;
    code cs;
    regm_t retregs;

#if DEBUG
    printf("cdorth(e = %p, *pretregs = %s)\n", e, regm_str(*pretregs));
    elem_print(e);
#endif

    elem *e1 = e->E1;
    elem *e2 = e->E2;

    if (*pretregs == 0)                   // if don't want result
    {
        c = codelem(e1, pretregs, FALSE); // eval left leaf
        *pretregs = 0;                    // in case they got set
        return cat(c, codelem(e2, pretregs, FALSE));
    }

    tym_t ty  = tybasic(e->Ety);
    tym_t ty1 = tybasic(e1->Ety);
    tym_t ty2 = tybasic(e2->Ety);
    unsigned sz = tysize[ty];
    int numwords = (sz == 2 * REGSIZE) ? 2 : 1;

    if (tyfloating(ty1)) assert(0);

    bool test  = false;
    unsigned op1 = 0, op2 = 0;
    unsigned reg;
    switch (e->Eoper)
    {
        case OPadd: op1 = OP_ADD; op2 = OP_ADC; break;
        case OPmin: op1 = OP_SUB; op2 = OP_SBC; break;
        case OPor:  op1 = op2 = OP_ORR; break;
        case OPxor: op1 = op2 = OP_EOR; break;
        case OPand: op1 = op2 = OP_AND;
            if (tyreg(ty1) && *pretregs == mPSW)
            {
                test = true;
                op1 = op2 = OP_TST;
            }
            break;
        default:
            assert(0);
    }

    if (test & tysize[ty1] <= REGSIZE &&
        (e1->Eoper == OPvar || (e1->Eoper == OPind && !e1->Ecount)))
    {
        // Handle the case of (var & const)
        if (e2->Eoper == OPconst && el_signx32(e2))
        {
            assert(0);
#if 0
            c = getlvalue(&cs, e1, 0);
            targ_size_t value = e2->EV.Vpointer;
            if (sz == 2)
                value &= 0xffff;
            else if (sz == 4)
                value &= 0xffffffff;

            if (reghasvalue(ALLREGS, value, &reg))
                goto L11;

            if (sz == 8) assert(value == (int)value);

            cs.Iflags |= CFimmed;
            cs.IEV1.Vint = value;
            cs.IFL1 = FLconst;
            goto L10;
#endif
        }

        // handle (exp & reg)
        if (isregvar(e2, &retregs, &reg))
        {
            assert(0);
#if 0
            c = getlvalue(&cs, e1, 0);
L11:
            cs.setRegM(reg);
L10:
            cs.Iop = op1;
            cs.Iflags |= CFpsw;
            freenode(e1);
            freenode(e2);
            return gen(c, &cs);
#endif
        }
    }

    unsigned posregs = ALLREGS;
    retregs = *pretregs & posregs;

    // if no return regs speced (like if wanted flags only), give us some
    if (retregs == 0)
        retregs = ALLREGS & posregs;

    regm_t rretregs;
    code *cl = codelem(e1, &retregs, test);
    code *cr = NULL;
    code *cg = NULL;
    reg = findreg(retregs);

    switch(e2->Eoper)
    {
        case OPind: assert(0); break;
        default:
L2:
        {
            rretregs = ALLREGS & ~retregs;

            cr = scodelem(e2, &rretregs, retregs, TRUE);       /* get rvalue   */
            unsigned rreg = (tysize[ty2] > REGSIZE) ? findreglsw(rretregs) : findreg(rretregs);
            c = NULL;
            if (numwords == 1)
            {
                /* ADD reg,rreg */
                c = gen_OP_data_proc(c, op1, 0, reg, rreg);
            }
            else /* numwords == 2 */
            {
                /* ADD lsreg,lsrreg     */
                reg = findreglsw(retregs);
                rreg = findreglsw(rretregs);
                bool setcc = (e->Eoper == OPadd || e->Eoper == OPmin);
                c = gen_OP_data_proc(c, op1, setcc ? CFpsw : 0, reg, rreg);

                reg = findregmsw(retregs);
                rreg = findregmsw(rretregs);
                // if second operand is 0 and OR or XOR
                if (!(e2->Eoper == OPu16_32 && (op2 == OP_ORR || op2 == OP_EOR)))
                    c = gen_OP_data_proc(c, op2, 0, reg, rreg);  // ADC msreg,msrreg
            }
            break;
        }

        case OPrelconst:
            assert(0);

        case OPconst:
            if (numwords == 1)
            {
                if (!el_signx32(e2))
                    goto L2;
                targ_size_t i = e2->EV.Vpointer;
                if (sz == 2)
                {
                    if (!(*pretregs & mPSW) &&
                            config.flags4 & CFG4speed &&
                            (e->Eoper == OPor || e->Eoper == OPxor || test ||
                             (e1->Eoper != OPvar && e1->Eoper != OPind)))
                    {
                        i &= 0xFFFF;
                    }
                }
                unsigned rreg;
                int rval = reghasvalue(ALLREGS, i, &rreg);
                bool setcc = (*pretregs & mPSW);
                if (rval)
                    c = gen_OP_data_proc(c, op1, setcc, reg, rval);
                else
                    c = gen_OP_data_proc_immed(c, op1, setcc, reg, reg, i);
            }
            else if (numwords == 2)
            {
                c = getregs(retregs);
                reg = findregmsw(retregs);
                unsigned lsreg = findreglsw(retregs);
                bool setcc = (e->Eoper == OPadd || e->Eoper == OPmin);
                c = gen_OP_data_proc_immed(c, op1, setcc ? CFpsw : 0, lsreg, lsreg, e2->EV.Vlong);

                targ_int msw = MSREG(e2->EV.Vllong);
                c = gen_OP_data_proc_immed(c, op2, 0, reg, reg, msw);
            }
            else
                assert(0);
            freenode(e2);
            break;

        case OPvar:
L1:
            c = loadea(e2, &cs, op1, ((numwords == 2) ? findreglsw(retregs) : reg), 0, retregs, retregs);
            if (numwords == 2)
            {
                if (e->Eoper == OPadd || e->Eoper == OPmin)
                    code_orflag(c, CFpsw);
                reg = findregmsw(retregs);
                if (EOP(e2))
                {
                    assert(0);
#if 0
                    getlvalue_msw(&cs);
                    cs.Iop = op2;
                    cs.setRegD(reg);
                    cs.setRegN(reg);
                    c = gen(c, &cs);                 /* ADC reg,data+2 */
#endif
                }
                else
                    c = cat(c, loadea(e2, &cs, op2, reg, REGSIZE, retregs, 0));
            }
            freenode(e2);
            break;
    }

    cg = test ? NULL : getregs(retregs); // trashed  regs
L5:
    c = cat(c, fixresult(e, retregs, pretregs));
    return cat4(cl, cr, cg, c);
}



/*********************************
 * Generate code for a logical expression.
 * Input:
 *      e       elem
 *      jcond
 *         bit 1 if TRUE then goto jump address if e
 *               if FALSE then goto jump address if !e
 *         2    don't call save87()
 *      fltarg   FLcode or FLblock, flavor of target if e evaluates to jcond
 *      targ    either code or block pointer to destination
 */

code *logexp(elem *e,int jcond,unsigned fltarg,code *targ)
{
    assert((jcond & 2) == 0);

    code *c,*ce,*cnop;
    regm_t retregs;
    unsigned op;

    //printf("logexp(e = %p, jcond = %d)\n", e, jcond);
    _chkstack();
    code *cc = docommas(&e);            // scan down commas
    cgstate.stackclean++;

    if (EOP(e) && !e->Ecount)     /* if operator and not common sub */
    {
        con_t regconsave;

        switch (e->Eoper)
        {
            case OPoror:
                if (jcond & 1)
                {
                    c = logexp(e->E1,jcond,fltarg,targ);
                    regconsave = regcon;
                    ce = logexp(e->E2,jcond,fltarg,targ);
                }
                else
                {
                    cnop = gennop(NULL);
                    c = logexp(e->E1,jcond | 1,FLcode,cnop);
                    regconsave = regcon;
                    ce = logexp(e->E2,jcond,fltarg,targ);
                    ce = cat(ce,cnop);
                }
                cnop = NULL;
                goto L1;

            case OPandand:
                if (jcond & 1)
                {
                    cnop = gennop(NULL);    /* a dummy target address */
                    c = logexp(e->E1,jcond & ~1,FLcode,cnop);
                    regconsave = regcon;
                    ce = logexp(e->E2,jcond,fltarg,targ);
                }
                else
                {
                    c = logexp(e->E1,jcond,fltarg,targ);
                    regconsave = regcon;
                    ce = logexp(e->E2,jcond,fltarg,targ);
                    cnop = NULL;
                }
L1:
                andregcon(&regconsave);
                freenode(e);
                c = cat4(cc,c,ce,cnop);
                goto Lret;

            case OPnot:
                jcond ^= 1;
            case OPbool:
            case OPs8_16:
            case OPu8_16:
            case OPs16_32:
            case OPu16_32:
            case OPs32_64:
            case OPu32_64:
            case OPu32_d:
            case OPd_ld:
                c = logexp(e->E1,jcond,fltarg,targ);
                freenode(e);
                goto Lretc;

            case OPcond:
            {
                code *cnop2 = gennop(NULL);   // addresses of start of leaves
                cnop = gennop(NULL);
                c = logexp(e->E1,FALSE,FLcode,cnop2);   /* eval condition */
                con_t regconold = regcon;
                ce = logexp(e->E2->E1,jcond,fltarg,targ);
                ce = genjmp(ce,COND_AL,FLcode,(block *) cnop); /* skip second leaf */

                regconsave = regcon;
                regcon = regconold;

                code_next(cnop2) = logexp(e->E2->E2,jcond,fltarg,targ);
                andregcon(&regconold);
                andregcon(&regconsave);
                freenode(e->E2);
                freenode(e);
                c = cat6(cc,c,NULL,ce,cnop2,cnop);
                goto Lret;
            }
        }
    }

    /* Special code for signed long compare.
     * Not necessary for I64 until we do cents.
     */
    if (OTrel2(e->Eoper) &&               /* if < <= >= >                 */
        !e->Ecount &&
        tybasic(e->E1->Ety) == TYllong &&
        tybasic(e->E2->Ety) == TYllong)
    {
        c = longcmp(e,jcond,fltarg,targ);
        goto Lretc;
    }

    retregs = mPSW;               /* return result in flags               */
    op = jmpopcode(e);            /* get jump opcode                      */
    if (!(jcond & 1))
        op ^= 1;
    c = codelem(e,&retregs,TRUE); /* evaluate elem                        */
    c = cat(c,cse_flush(true));     // flush CSE's to memory
    genjmp(c,op,fltarg,(block *) targ);   /* generate jmp instruction     */
Lretc:
    c = cat(cc,c);
Lret:
    cgstate.stackclean--;
    return c;
}

code* cdloglog(elem *e, regm_t *pretregs)
{
    regm_t retregs;
    unsigned reg;
    code *c;
    code *cr,*cg,*cnop2;
    unsigned sz = tysize(e->Ety);

    cgstate.stackclean++;

    code *cnop1 = gennop(NULL);
    code *cnop3 = gennop(NULL);
    elem *e2 = e->E2;

    code *cl = (e->Eoper == OPoror)
        ? logexp(e->E1,1,FLcode,cnop1)
        : logexp(e->E1,0,FLcode,cnop3);

    con_t regconsave = regcon;
    unsigned stackpushsave = stackpush;

    if (*pretregs == 0)                   // if don't want result
    {
        int noreturn = el_noreturn(e2);

        cr = codelem(e2, pretregs, FALSE);
        if (noreturn)
        {
            regconsave.used |= regcon.used;
            regcon = regconsave;
        }
        else
            andregcon(&regconsave);

        assert(stackpush == stackpushsave);
        c = cat4(cl, cr, cnop3, cnop1);    // eval code, throw away result
        goto Lret;
    }

    cnop2 = gennop(NULL);

    if (tybasic(e2->Ety) == TYbool &&
            sz == tysize(e2->Ety) &&
            !(*pretregs & mPSW) &&
            e2->Eoper == OPcall)
    {
        cr = codelem(e2,pretregs,FALSE);

        andregcon(&regconsave);

        // stack depth should not change when evaluating E2
        assert(stackpush == stackpushsave);

        assert(sz <= 4);                                        // result better be int
        retregs = *pretregs & allregs;
        cnop1 = cat(cnop1, allocreg(&retregs, &reg, TYint));    // allocate reg for result
        cg = genjmp(NULL, COND_AL, FLcode, (block *) cnop2);    // JMP cnop2
        cnop1 = movregconst(cnop1, reg, e->Eoper == OPoror, 0); // reg = 1
        regcon.immed.mval &= ~mask[reg];                        // mark reg as unavail
        *pretregs = retregs;
        if (e->Eoper == OPoror)
            c = cat6(cl, cr, cnop3, cg, cnop1, cnop2);
        else
            c = cat6(cl, cr, cg, cnop3, cnop1, cnop2);

        goto Lret;
    }
    cr = logexp(e2, 1, FLcode, cnop1);
    andregcon(&regconsave);

    // stack depth should not change when evaluating E2
    assert(stackpush == stackpushsave);

    assert(sz <= 4);                      // result better be int
    retregs = *pretregs & ALLREGS;
    if (!retregs) retregs = ALLREGS;      // if mPSW only
    cg = allocreg(&retregs, &reg, TYint); // allocate reg for result
    for (code *c1 = cg; c1; c1 = code_next(c1)) // for each instruction
        gen(cnop1, c1);                   // duplicate it
    cg = movregconst(cg, reg, 0, *pretregs & mPSW);       // MOV reg,0
    regcon.immed.mval &= ~mask[reg];                      // mark reg as unavail
    genjmp(cg, COND_AL, FLcode, (block *) cnop2);         // JMP cnop2
    cnop1 = movregconst(cnop1, reg, 1,* pretregs & mPSW); // reg = 1
    regcon.immed.mval &= ~mask[reg];                      // mark reg as unavail
    *pretregs = retregs;
    c = cat6(cl, cr, cnop3, cg, cnop1, cnop2);
Lret:
    cgstate.stackclean--;
    return c;
}


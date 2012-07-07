
/*******************************************
 * Generate code for OPpair, OPrpair.
 */

code *cdpair(elem *e, regm_t *pretregs)
{
    regm_t retregs;
    regm_t regs1;
    regm_t regs2;
    code *cg;
    code *c1;
    code *c2;

    if (*pretregs == 0)                         // if don't want result
    {
        c1 = codelem(e->E1,pretregs,FALSE);     // eval left leaf
        *pretregs = 0;                          // in case they got set
        return cat(c1,codelem(e->E2,pretregs,FALSE));
    }

    //printf("\ncdpair(e = %p, *pretregs = x%x)\n", e, *pretregs);
    //printf("Ecount = %d\n", e->Ecount);
    retregs = *pretregs & allregs;
    if  (!retregs)
        retregs = allregs;
    regs1 = retregs & mLSW;
    regs2 = retregs & mMSW;
    if (e->Eoper == OPrpair)
    {
        regs1 = regs2;
        regs2 = retregs & mLSW;
    }
    //printf("1: regs1 = x%x, regs2 = x%x\n", regs1, regs2);
    c1 = codelem(e->E1, &regs1, FALSE);
    c2 = scodelem(e->E2, &regs2, regs1, FALSE);

    cg = NULL;
    if (e->E1->Ecount)
        cg = getregs(regs1);
    if (e->E2->Ecount)
        cg = cat(cg, getregs(regs2));

    //printf("regs1 = x%x, regs2 = x%x\n", regs1, regs2);
    return cat4(c1,c2,cg,fixresult(e,regs1 | regs2,pretregs));
}


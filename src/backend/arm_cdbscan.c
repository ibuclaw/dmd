
code *cdbscan(elem *e, regm_t *pretregs)
{
    regm_t retregs;
    unsigned reg;
    code *cl,*cg;
    code cs;

    //printf("cdbscan()\n");
    //elem_print(e);

    if (*pretregs == 0)
        return codelem(e->E1, pretregs, FALSE);

    tym_t tyml = tybasic(e->E1->Ety);
    int sz = tysize[tyml];
    assert(sz == 2 || sz == 4 || sz == 8);

    if ((e->E1->Eoper == OPind && !e->E1->Ecount) || e->E1->Eoper == OPvar)
    {
        assert(0);
#if 0
        cl = getlvalue(&cs, e->E1, RMload);     // get addressing mode
#endif
    }
    else
    {
        retregs = allregs;
        cl = codelem(e->E1, &retregs, FALSE);
        reg = findreg(retregs);
    }

    retregs = *pretregs & allregs;
    if  (!retregs)
        retregs = allregs;
    cg = allocreg(&retregs, &reg, e->Ety);

#if TX86
    cs.Iop = (e->Eoper == OPbsf) ? 0x0FBC : 0x0FBD;        // BSF/BSR reg,EA
    code_newreg(&cs, reg);
#else
    assert(0);
#endif
    cg = gen(cg, &cs);

    return cat3(cl, cg, fixresult(e, retregs, pretregs));
}

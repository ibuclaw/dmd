
code* cdcomma(elem *e, regm_t *pretregs)
{
    regm_t retregs = 0;
    code *cl = codelem(e->E1, &retregs, FALSE); // ignore value from left leaf
    code *cr = codelem(e->E2, pretregs, FALSE); // do right leaf
    return cat(cl, cr);
}


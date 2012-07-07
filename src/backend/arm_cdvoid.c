
code *cdvoid(elem *e,regm_t *pretregs)
{
    assert(*pretregs == 0);
    return codelem(e->E1, pretregs, FALSE);
}


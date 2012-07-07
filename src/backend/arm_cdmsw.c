
/**********************************************
 * Get top 32 bits of 64 bit value (I32)
 * or top 16 bits of 32 bit value (I16)
 * or top 64 bits of 128 bit value (I64).
 * OPmsw
 */

code *cdmsw(elem *e,regm_t *pretregs)
{
    //printf("cdmsw(e->Ecount = %d)\n", e->Ecount);
    assert(e->Eoper == OPmsw);

    regm_t retregs = *pretregs ? ALLREGS : 0;
    code* c = codelem(e->E1,&retregs,FALSE);
    retregs &= mMSW;                    // want MSW only

    // We "destroy" a reg by assigning it the result of a new e, even
    // though the values are the same. Weakness of our CSE strategy that
    // a register can only hold the contents of one elem at a time.
    if (e->Ecount)
        c = cat(c,getregs(retregs));
    else
        useregs(retregs);

#ifdef DEBUG
    if (!(!*pretregs || retregs))
    {
        WROP(e->Eoper);
        printf(" *pretregs = %s, retregs = %s\n",regm_str(*pretregs),regm_str(retregs));
        elem_print(e);
    }
#endif
    assert(!*pretregs || retregs);
    return cat(c,fixresult(e,retregs,pretregs));        // msw only
}


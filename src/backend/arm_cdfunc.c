
/***********************************
 * tyf: type of the function
 */
FuncParamRegs::FuncParamRegs(tym_t tyf)
{
    this->tyf = tyf;
    i = 0;
    regcnt = 0;
    xmmcnt = 0;

    static const unsigned char reglist[] = { R0, R1, R2, R3 };
    argregs = reglist;
    numintegerregs = sizeof(reglist) / sizeof(reglist[0]);

    numfloatregs = 0;
}

/*****************************************
 * Allocate parameter of type t and ty to registers *preg1 and *preg2.
 * Returns:
 *      0       not allocated to any register
 *      1       *preg1 set to allocated register
 *      2       *preg1, *preg2 set to allocated register pair
 */

// t is valid only if ty is a TYstruct or TYarray
static int type_jparam2(type *t, tym_t ty)
{
    ty = tybasic(ty);

    if (tyfloating(ty))
        ;
    else if (ty == TYstruct || ty == TYarray)
    {
        type_debug(t);
        targ_size_t sz = type_size(t);
        return (sz <= NPTRSIZE) &&
               (sz == 1 || sz == 2 || sz == 4 || sz == 8);
    }
    else if (tysize(ty) <= NPTRSIZE)
        return 1;
    return 0;
}

int FuncParamRegs::alloc(type *t, tym_t ty, unsigned char *preg1, unsigned char *preg2)
{
    ++i;
    *preg1 = NOREG;
    *preg2 = NOREG;

    type *t2 = NULL;
    tym_t ty2 = TYMAX;

    // If struct just wraps another type
    if (tybasic(ty) == TYstruct && tybasic(t->Tty) == TYstruct)
    {
        type *targ1 = t->Ttag->Sstruct->Sarg1type;
        type *targ2 = t->Ttag->Sstruct->Sarg2type;
        if (targ1)
        {
            t = targ1;
            ty = t->Tty;
            if (targ2)
            {
                t2 = targ2;
                ty2 = t2->Tty;
            }
        }
        else if (!targ2)
            return 0;
    }

    reg_t *preg = preg1;
    int regcntsave = regcnt;
    int xmmcntsave = xmmcnt;

    for (int j = 0; j < 2; j++)
    {
        if (regcnt < numintegerregs)
        {
            if (type_jparam2(t, ty))
            {
                *preg1 = argregs[regcnt];
                ++regcnt;
                goto Lnext;
            }
        }
        if (xmmcnt < numfloatregs)
        {
            if (tyxmmreg(ty))
            {
                *preg1 = floatregs[xmmcnt];
                ++xmmcnt;
                goto Lnext;
            }
        }
        // Failed to allocate to a register
        if (j == 1)
        {   /* Unwind first preg1 assignment, because it's both or nothing
             */
            *preg1 = NOREG;
            regcnt = regcntsave;
            xmmcnt = xmmcntsave;
        }
        return 0;

     Lnext:
        if (!t2)
            break;
        preg = preg2;
        t = t2;
        ty = ty2;
    }
    return 1;
}


/***************************
 * Generate code to push parameter list.
 * stackpush is incremented by stackalign for each PUSH.
 */

code *params(elem *e,unsigned stackalign)
{
    code *ce;
    unsigned reg;
    regm_t retregs;
    elem *e1;
    symbol *s;
    int fl;

    printf("params(e = %p, stackalign = %d)\n", e, stackalign);
    code* cp = NULL;
    stackchanged = 1;
    assert(e);
    while (e->Eoper == OPparam)           /* if more params               */
    {
        elem* e2 = e->E2;
        cp = cat(cp, params(e->E1, stackalign));  // push them backwards
        freenode(e);
        e = e2;
    }
    //printf("params()\n"); elem_print(e);

    tym_t tym = tybasic(e->Ety);
    if (tyfloating(tym))
        Obj::fltused();

    /* sz = number of bytes pushed        */
    targ_size_t szb; // size before alignment
    if (tyscalar(tym))
        szb = size(tym);
    else if (tym == TYstruct)
        szb = type_size(e->ET);
    else
    {
#ifdef DEBUG
        WRTYxx(tym);
#endif
        assert(0);
    }
    targ_size_t sz = align(stackalign, szb);           /* align on word stack boundary */
    assert((sz & (stackalign - 1)) == 0); /* ensure that alignment worked */
    assert((sz & (REGSIZE - 1)) == 0);

    code* c = NULL;
    code cs;
    switch (e->Eoper)
    {
        case OPstrpar:
        {
            assert(0);
#if 0
            code *cc,*c1,*c2,*c3;
            unsigned rm;
            unsigned seg;           // segment override prefix flags
            bool doneoff;
            unsigned pushsize = REGSIZE;
            unsigned op16 = 0;
            unsigned npushes;

            e1 = e->E1;
            if (sz == 0)
            {
                ce = docommas(&e1); /* skip over any commas         */
                goto L2;
            }
            if ((sz & 3) == 0 && (sz / REGSIZE) <= 4 && e1->Eoper == OPvar)
            {
                freenode(e);
                e = e1;
                goto L1;
            }
            cc = docommas(&e1);     /* skip over any commas         */
            seg = 0;                /* assume no seg override       */
            retregs = sz ? IDXREGS : 0;
            doneoff = FALSE;
            if (sz & 2)     // if odd number of words to push
            {
                pushsize = 2;
                op16 = 1;
            }
            npushes = sz / pushsize;
            switch (e1->Eoper)
            {
                case OPind:
                    c1 = codelem(e1->E1, &retregs, FALSE);
                    freenode(e1);
                    break;
                case OPvar:
                    assert(0);
#if 0
                    /* Symbol is no longer a candidate for a register */
                    e1->EV.sp.Vsym->Sflags &= ~GTregcand;

                    if (!e1->Ecount && npushes > 4)
                    {
                        /* Kludge to point at last word in struct. */
                        /* Don't screw up CSEs.                 */
                        e1->EV.sp.Voffset += sz - pushsize;
                        doneoff = TRUE;
                    }
                    static unsigned segtocf[4] = { CFes,CFcs,CFss,0 };

                    int fl = el_fl(e1);
                    unsigned s = segfl[fl];
                    assert(s < 4);
                    seg = segtocf[s];
                    if (seg == CFss && !(config.wflags & WFssneds))
                        seg = 0;
                    c1 = cdrelconst(e1, &retregs);
                    /* Reverse the effect of the previous add       */
                    if (doneoff)
                        e1->EV.sp.Voffset -= sz - pushsize;
                    freenode(e1);
#endif
                    break;
                case OPstreq:
                    c1 = codelem(e1, &retregs, FALSE);
                    break;
                default:
#ifdef DEBUG
                    elem_print(e1);
#endif
                    assert(0);
            }
            reg = findreglsw(retregs);
            rm = regtorm32[reg];
            if (op16)
                seg |= CFopsize;            // operand size
            if (npushes <= 4)
            {
                assert(!doneoff);
                for (c2 = CNIL; npushes > 1; npushes--)
                {
                    c2 = genc1(c2, OP_PUSH, buildModregrm(2,6,rm), FLconst, pushsize * (npushes - 1));  // PUSH [reg]
                    code_orflag(c2, seg);
                    c2 = genadjesp(c2, pushsize);
                }
                c3 = gen2(NULL, OP_PUSH, buildModregrm(0,6,rm));     // PUSH [reg]
                c3->Iflags |= seg;
                c3 = genadjesp(c3,pushsize);
                ce = cat4(cc, c1, c2, c3);
            }
            else if (sz)
            {
                assert(0);
#if 0
                c2 = getregs_imm(mCX | retregs);
                /* MOV CX,sz/2  */
                c2 = movregconst(c2,CX,npushes,0);
                if (!doneoff)
                {   /* This disgusting thing should be done when    */
                    /* reg is loaded. Too lazy to fix it now.       */
                    /* ADD reg,sz-2 */
                    c2 = genc2(c2,0x81,grex | modregrmx(3,0,reg),sz-pushsize);
                }
                c3 = getregs(mCX);                                  // the LOOP decrements it
                c3 = gen2(c3,0xFF,buildModregrm(0,6,rm));           // PUSH [reg]
                c3->Iflags |= seg | CFtarg2;
                genc2(c3,0x81,grex | buildModregrm(3,5,reg),pushsize);  // SUB reg,2
                int size = ((seg & CFSEG) ? -8 : -7) - op16;
                if (code_next(c3)->Iop != 0x81)
                    size++;
                //genc2(c3,0xE2,0,size);    // LOOP .-7 or .-8
                genjmp(c3,0xE2,FLcode,(block *)c3);         // LOOP c3
                regimmed_set(CX,0);
                genadjesp(c3,sz);
                ce = cat4(cc,c1,c2,c3);
#endif
            }
            else
                ce = cat(cc, c1);
            stackpush += sz;
            goto L2;
#endif
        }
        case OPind:
            assert(0);
#if 0
            if (!e->Ecount)                         /* if *e1       */
            {
                if (sz <= REGSIZE)
                {   // Watch out for single byte quantities being up
                    // against the end of a segment or in memory-mapped I/O
                    if (!(config.exe & EX_flat) && szb == 1)
                        break;
                    goto L1;            // can handle it with loadea()
                }

                // Avoid PUSH MEM on the Pentium when optimizing for speed
                if (config.flags4 & CFG4speed &&
                        (config.target_cpu >= TARGET_80486 &&
                         config.target_cpu <= TARGET_PentiumMMX) &&
                        sz <= 2 * REGSIZE &&
                        !tyfloating(tym))
                    break;

                if (tym == TYldouble || tym == TYildouble || tycomplex(tym))
                    break;
                if (I32)
                {
                    assert(sz == REGSIZE * 2);
                    ce = loadea(e,&cs,0xFF,6,REGSIZE,0,0); /* PUSH EA+4 */
                    ce = genadjesp(ce,REGSIZE);
                }
                else
                {
                    if (sz == DOUBLESIZE)
                    {   ce = loadea(e,&cs,0xFF,6,DOUBLESIZE - REGSIZE,0,0); /* PUSH EA+6        */
                        cs.IEVoffset1 -= REGSIZE;
                        gen(ce,&cs);                    /* PUSH EA+4    */
                        ce = genadjesp(ce,REGSIZE);
                        getlvalue_lsw(&cs);
                        gen(ce,&cs);                    /* PUSH EA+2    */
                    }
                    else /* TYlong */
                        ce = loadea(e,&cs,0xFF,6,REGSIZE,0,0); /* PUSH EA+2 */
                    ce = genadjesp(ce,REGSIZE);
                }
                stackpush += sz;
                getlvalue_lsw(&cs);
                gen(ce, &cs);                            /* PUSH EA      */
                genadjesp(ce, REGSIZE);
                goto L2;
            }
#endif
            break;
        case OPrelconst:
            break;                          /* else must evaluate expression */
        case OPvar:
L1:
            assert(0);
#if 0
            int regsize = REGSIZE;
            unsigned flag = 0;

            ce = loadea(e,&cs,0xFF,6,sz - regsize,RMload,0);    // PUSH EA+sz-2
            code_orflag(ce,flag);
            genadjesp(ce, REGSIZE);
            stackpush += sz;
            while ((targ_int)(sz -= regsize) > 0)
            {
                ce = cat(ce,loadea(e,&cs,0xFF,6,sz - regsize,RMload,0));
                code_orflag(ce,flag);
                genadjesp(ce, REGSIZE);
            }
#endif
L2:
            freenode(e);
            c = cat(c, ce);
            goto ret;
        case OPconst:
        {
            char pushi = 0;
            unsigned flag = 0;
            int regsize = REGSIZE;
            targ_int value;

            if (tycomplex(tym))
                break;

            if (szb == 10)           // special case for long double constants
            {
                assert(0);
#if 0
                assert(sz == 12);
                value = ((unsigned short *)&e->EV.Vldouble)[4];
                stackpush += sz;
                ce = genadjesp(NULL, sz);
                for (int i = 2; i >= 0; i--)
                {
                    if (reghasvalue(allregs, value, &reg))
                        ce = gen1(ce,0x50 + reg);           // PUSH reg
                    else
                        ce = genc2(ce,0x68,0,value);        // PUSH value
                    value = ((unsigned *)&e->EV.Vldouble)[i - 1];
                }
                goto L2;
#endif
            }

            assert(sz <= tysize[TYldouble]);
            assert(sz != 2);
            int i = sz;

#if 0
            if (config.target_cpu >= TARGET_80286)
                pushi = 1;
            else
#endif
                if (i == REGSIZE)
                break;

            assert(0);
#if 0
            stackpush += sz;
            ce = genadjesp(NULL, sz);
            targ_uns *pi = (targ_uns *) &e->EV.Vdouble;
            targ_ushort *ps = (targ_ushort *)pi;
            targ_ullong *pl = (targ_ullong *)pi;
            i /= regsize;
            do
            {
                if (i)                      /* be careful not to go negative */
                    i--;
                targ_size_t value = (regsize == 4) ? pi[i] : ps[i];
                if (regsize == 8)
                    value = pl[i];
                if (pushi)
                {
                    if (regsize == REGSIZE && reghasvalue(allregs,value,&reg))
                        goto Preg;
                    ce = genc2(ce,(szb == 1) ? 0x6A : 0x68,0,value); // PUSH value
                }
                else
                {
                    ce = regwithvalue(ce,allregs,value,&reg,0);
Preg:
                    ce = genpush(ce,reg);         // PUSH reg
                }
                code_orflag(ce,flag);                       /* operand size */
            } while (i);
            goto L2;
#endif
        }
        default:
            break;
    }

    retregs = tybyte(tym) ? BYTEREGS : allregs;
    if (tyfloating(tym))
        assert(0);

    c = cat(c, scodelem(e, &retregs, 0, TRUE));
    if (retregs != mSTACK)                /* if stackpush not already inc'd */
        stackpush += sz;
    if (sz <= REGSIZE)
    {
        c = gen_OP_PUSH(c, 0, mask[findreg(retregs)]);   // PUSH reg
        genadjesp(c, REGSIZE);
    }
    else if (sz == REGSIZE * 2)
    {
        c = gen_OP_PUSH(c, 0, mask[findregmsw(retregs)]); // PUSH msreg
        gen_OP_PUSH(c, 0, mask[findreglsw(retregs)]);    // PUSH lsreg
        genadjesp(c, sz);
    }
ret:
    printf("-params\n");
    return cat(cp, c);
}

/*************************************************
 * Helper function for converting OPparam's into array of Parameters.
 */
struct Parameter { elem *e; unsigned char reg, reg2; unsigned numalign; };

void fillParameters(elem *e, Parameter *parameters, int *pi)
{
    if (e->Eoper == OPparam)
    {
        fillParameters(e->E1, parameters, pi);
        fillParameters(e->E2, parameters, pi);
        freenode(e);
    }
    else
    {
        parameters[*pi].e = e;
        (*pi)++;
    }
}

/***************************
 * Determine size of everything that will be pushed.
 */

targ_size_t paramsize(elem *e,unsigned stackalign)
{
    targ_size_t psize = 0;
    targ_size_t szb;

    while (e->Eoper == OPparam)         /* if more params               */
    {
        elem *e2 = e->E2;
        psize += paramsize(e->E1, stackalign);   // push them backwards
        e = e2;
    }
    tym_t tym = tybasic(e->Ety);
    if (tyscalar(tym))
        szb = size(tym);
    else if (tym == TYstruct)
        szb = type_size(e->ET);
    else
    {
#ifdef DEBUG
        WRTYxx(tym);
#endif
        assert(0);
    }
    psize += align(stackalign, szb);     /* align on word stack boundary */
    return psize;
}

/******************************
 * Call function. All parameters are pushed onto the stack, numpara gives
 * the size of them all.
 */

static code* funccall(elem *e, unsigned numpara, unsigned numalign, regm_t *pretregs, regm_t keepmsk)
{
    code *ce,cs;
    regm_t retregs;
    symbol *s;

    printf("funccall(e = %p, *pretregs = %s, numpara = %d, numalign = %d)\n",e,regm_str(*pretregs),numpara,numalign);
    calledafunc = 1;

    /* Determine if we need frame for function prolog/epilog    */
    elem* e1 = e->E1;
    tym_t tym1 = tybasic(e1->Ety);
    code* c = NULL;
    if (e1->Eoper == OPvar)
    {
        /* Call function directly       */
        code *c1;

#ifdef DEBUG
        if (!tyfunc(tym1)) WRTYxx(tym1);
#endif
        assert(tyfunc(tym1));
        s = e1->EV.sp.Vsym;
        if (s->Sflags & SFLexit)
            // Function doesn't return, so don't worry about registers it may use
            c1 = NULL;
        else if (!tyfunc(s->ty()) || !(config.flags4 & CFG4optimized))
            // so we can replace func at runtime
            c1 = getregs(~fregsaved & (mBP | ALLREGS | XMMREGS));
        else
            c1 = getregs(~s->Sregsaved & (mBP | ALLREGS | XMMREGS));

        if (strcmp(s->Sident, "alloca") == 0)
        {
            assert(0);
#if 0
            s = rtlsym[RTLSYM_ALLOCA];
            makeitextern(s);
            c1 = cat(c1,getregs(mCX));
            c1 = genc(c1,0x8D,modregrm(2,CX,BPRM),FLallocatmp,0,0,0);  // LEA CX,&localsize[BP]
            if (I64)
                code_orrex(c1, REX_W);
            usedalloca = 2;             // new way
#endif
        }
        if (sytab[s->Sclass] & SCSS)    // if function is on stack (!)
        {
            assert(0);
#if 0
            retregs = allregs & ~keepmsk;
            s->Sflags &= ~GTregcand;
            s->Sflags |= SFLread;
            ce = cat(c1,cdrelconst(e1,&retregs));
            goto LF2;
#endif
        }
        else
        {
            int fl = FLfunc;
            if (!tyfunc(s->ty()))
                fl = el_fl(e1);
            ce = gen_OP_BL(NULL, fl, s);
            ce->Iflags |= (CFselfrel | CFoff);
            if (s == tls_get_addr_sym)
            {
                assert(0);
#if 0
                // Append a NOP so GNU linker has patch room
                ce = gen1(ce, 0x90);            // NOP
                code_orflag(ce, CFvolatile);    // don't schedule it
#endif
            }
        }
        ce = cat(c1, ce);
    }
    else
    {
        // Call function via pointer
#ifdef DEBUG
        if (e1->Eoper != OPind) { WRFL((enum FL)el_fl(e1)); WROP(e1->Eoper); }
#endif
        assert(e1->Eoper == OPind);
        elem* e11 = e1->E1;
        tym_t e11ty = tybasic(e11->Ety);

        // if we can't use loadea()
        if ((EOP(e11) || e11->Eoper == OPconst) && (e11->Eoper != OPind || e11->Ecount))
        {
            retregs = allregs & ~keepmsk;
            cgstate.stackclean++;
            ce = scodelem(e11, &retregs, keepmsk, TRUE);
            cgstate.stackclean--;
            /* Kill registers destroyed by an arbitrary function call */
            ce = cat(ce, getregs((mBP | ALLREGS | XMMREGS) & ~fregsaved));
LF2:
            unsigned reg = findreg(retregs);
            ce = gen_OP_BX(ce, reg);
        }
        else
        {
            assert(0);
#if 0
            // CALL [function]
            cs.Iflags = 0;
            cgstate.stackclean++;
            ce = loadea(e11,&cs,0xFF,2,0,keepmsk,(mBP|ALLREGS|mES|XMMREGS) & ~fregsaved);
            cgstate.stackclean--;
            freenode(e11);
#endif
        }
        s = NULL;
    }
    c = cat(c, ce);
    freenode(e1);

    retregs = regmask(e->Ety, tym1);

    // If stack needs cleanup
    if (OTbinary(e->Eoper) && !typfunc(tym1) && !(s && s->Sflags & SFLexit))
    {
        if (tym1 == TYhfunc)
        {   // Hidden parameter is popped off by the callee
            c = genadjesp(c, -REGSIZE);
            stackpush -= REGSIZE;
            if (numpara + numalign > REGSIZE)
                c = genstackclean(c, numpara + numalign - REGSIZE, retregs);
        }
        else
            c = genstackclean(c, numpara + numalign, retregs);
    }
    else
    {
        c = genadjesp(c, -numpara);
        stackpush -= numpara;
        if (numalign)
            c = genstackclean(c, numalign, retregs);
    }

    printf("-funccall\n");
    return cat(c, fixresult(e, retregs, pretregs));
}

code* cdfunc(elem *e, regm_t *pretregs)
{
    unsigned numpara = 0;
    unsigned numalign = 0;

#if DEBUG
    //printf("cdfunc()\n"); elem_print(e);
#endif
    assert(e);

    unsigned stackpushsave = stackpush;   // so we can compute # of parameters
    cgstate.stackclean++;

    code *c = NULL;
    regm_t keepmsk = 0;
    int xmmcnt = 0;
    tym_t tyf = tybasic(e->E1->Ety);


    // Easier to deal with parameters as an array: parameters[0..np]
    int np = OTbinary(e->Eoper) ? el_nparams(e->E2) : 0;
    Parameter *parameters = (Parameter *)alloca(np * sizeof(Parameter));

    if (np)
    {
        int n = 0;
        fillParameters(e->E2, parameters, &n);
        assert(n == np);
    }

    /* Special handling for call to __tls_get_addr, we must save registers
     * before evaluating the parameter, so that the parameter load and call
     * are adjacent.
     */
    if (np == 1 && e->E1->Eoper == OPvar)
    {
        symbol *s = e->E1->EV.sp.Vsym;
        if (s == tls_get_addr_sym)
            c = getregs(~s->Sregsaved & ALLREGS);
    }

    unsigned stackalign = REGSIZE;

    // Figure out which parameters go in registers
    // Compute numpara, the total bytes pushed on the stack
    FuncParamRegs fpr(tyf);
    for (int i = np; --i >= 0;)
    {
        elem *ep = parameters[i].e;
        if (fpr.alloc(ep->ET, ep->Ety, &parameters[i].reg, &parameters[i].reg2))
            continue;   // goes in register, not stack

        // Parameter i goes on the stack
        parameters[i].reg = NOREG;
        unsigned alignsize = el_alignsize(ep);
        parameters[i].numalign = 0;
        if (alignsize > stackalign)
        {
            unsigned newnumpara = (numpara + (alignsize - 1)) & ~(alignsize - 1);
            parameters[i].numalign = newnumpara - numpara;
            numpara = newnumpara;
        }
        numpara += paramsize(ep, stackalign);
    }

    assert((numpara & (REGSIZE - 1)) == 0);
    assert((stackpush & (REGSIZE - 1)) == 0);

    /* Should consider reordering the order of evaluation of the parameters
     * so that args that go into registers are evaluated after args that get
     * pushed. We can reorder args that are constants or relconst's.
     */

    /* Adjust start of the stack so after all args are pushed,
     * the stack will be aligned.
     */
    if (STACKALIGN == 16 && (numpara + stackpush) & (STACKALIGN - 1))
    {
        numalign = STACKALIGN - ((numpara + stackpush) & (STACKALIGN - 1));
        c = cod3_stackadj(c, numalign);
        c = genadjesp(c, numalign);
        stackpush += numalign;
        stackpushsave += numalign;
    }

    int regsaved[R15 + 1];
    memset(regsaved, -1, sizeof(regsaved));
    code *crest = NULL;
    regm_t saved = 0;

    /* Parameters go into the registers R0, R1, R2, R3
     */
    for (int i = 0; i < np; i++)
    {
        elem *ep = parameters[i].e;
        int preg = parameters[i].reg;
        if (preg == NOREG)
        {
            /* Push parameter on stack, but keep track of registers used
             * in the process. If they interfere with keepmsk, we'll have
             * to save/restore them.
             */
            code *csave = NULL;
            regm_t overlap = msavereg & keepmsk;
            msavereg |= keepmsk;
            code *cp = params(ep, stackalign);
            regm_t tosave = keepmsk & ~msavereg;
            msavereg &= ~keepmsk | overlap;

            // tosave is the mask to save and restore
            for (int j = 0; tosave; j++)
            {
                regm_t mi = mask[j];
                assert(j <= R15);
                if (mi & tosave)
                {
                    unsigned idx;
                    csave = regsave.save(csave, j, &idx);
                    crest = regsave.restore(crest, j, idx);
                    saved |= mi;
                    keepmsk &= ~mi;             // don't need to keep these for rest of params
                    tosave &= ~mi;
                }
            }

            c = cat4(c, csave, cp, NULL);

            // Alignment for parameter comes after it got pushed
            unsigned numalign = parameters[i].numalign;
            if (numalign)
            {
                c = cod3_stackadj(c, numalign);
                c = genadjesp(c, numalign);
                stackpush += numalign;
            }
        }
        else
        {
            // Goes in register preg, not stack
            regm_t retregs = mask[preg];
            int preg2 = parameters[i].reg2;
            if (preg2 != NOREG)
                assert(0);
            else if (ep->Eoper == OPstrthis)
            {
                assert(0);
#if 0
                code *c1 = getregs(retregs);
                // LEA preg,np[RSP]
                unsigned np = stackpush - ep->EV.Vuns;   // stack delta to parameter
                code *c2 = genc1(CNIL,0x8D,(REX_W << 16) |
                        (modregrm(0,4,SP) << 8) |
                        modregxrm(2,preg,4), FLconst,np);
                c = cat3(c,c1,c2);
#endif
            }
            else
            {
                code *cp = scodelem(ep, &retregs, keepmsk, FALSE);
                c = cat(c, cp);
            }
            keepmsk |= retregs;      // don't change preg when evaluating func address
        }
    }

    // Restore any register parameters we saved
    c = cat4(c, getregs(saved), crest, NULL);
    keepmsk |= saved;

#if 0
    // Variadic functions store the number of XMM registers used in AL
    if (e->Eflags & EFLAGS_variadic)
    {
        assert(0);
        code *c1 = getregs(mAX);
        c1 = movregconst(c1,AX,xmmcnt,1);
        c = cat(c, c1);
        keepmsk |= mAX;
    }
#endif

    cgstate.stackclean--;

    if (numpara != stackpush - stackpushsave)
        printf("numpara = %d, stackpush = %d, stackpushsave = %d\n", numpara, stackpush, stackpushsave);
    assert(numpara == stackpush - stackpushsave);

#if DEBUG
    //printf("-cdfunc()\n"); elem_print(e);
#endif
    return cat(c, funccall(e, numpara, numalign, pretregs, keepmsk));
}


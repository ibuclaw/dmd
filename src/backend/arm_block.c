
/****************************************
 * Clean parameters off stack.
 * Input:
 *      numpara         amount to adjust stack pointer
 *      keepmsk         mask of registers to not destroy
 */

code *genstackclean(code *c, unsigned numpara, regm_t keepmsk)
{
    //dbg_printf("genstackclean(numpara = %d, stackclean = %d)\n", numpara, cgstate.stackclean);

    if (numpara && (cgstate.stackclean || STACKALIGN == 16))
    {
        c = gen_OP_data_proc_immed(c, OP_ADD, 0, SP, SP, numpara);

        stackpush -= numpara;
        c = genadjesp(c,-numpara);
    }
    return c;
}

// identical to version in cod3.c, move to common file?
void assignaddr(block *bl)
{
    int EBPtoESPsave = EBPtoESP;
    int hasframesave = hasframe;

    if (bl->Bflags & BFLoutsideprolog)
    {
        EBPtoESP = -REGSIZE;
        hasframe = 0;
    }
    assignaddrc(bl->Bcode);
    hasframe = hasframesave;
    EBPtoESP = EBPtoESPsave;
}

void assignaddrc(code *c)
{
    int sn;
    symbol *s;
    unsigned char rm;
    targ_size_t soff;

    targ_size_t base = EBPtoESP;
    for (; c; c = code_next(c))
    {
#ifdef DEBUG
        if (0)
        {       printf("assignaddrc()\n");
                c->print();
        }
        if (code_next(c) && code_next(code_next(c)) == c)
            assert(0);
#endif
        if ((c->Iop & ESCAPEmask) == ESCAPE)
        {
            if (c->Iop == (ESCAPE | ESCadjesp))
            {
                //printf("adjusting EBPtoESP (%d) by %ld\n",EBPtoESP,c->IEV1.Vint);
                EBPtoESP += c->IEV1.Vint;
                c->Iop = NOP;
            }
            if (c->Iop == (ESCAPE | ESCframeptr))
            {   // Convert to load of frame pointer
                // c->Irm is the register to use
                assert(0);
#if 0
                if (hasframe)
                {   // MOV reg,EBP
                    c->Iop = 0x89;
                    if (c->Irm & 8)
                        c->Irex |= REX_B;
                    c->Irm = modregrm(3,BP,c->Irm & 7);
                }
                else
                {   // LEA reg,EBPtoESP[ESP]
                    c->Iop = 0x8D;
                    if (c->Irm & 8)
                        c->Irex |= REX_R;
                    c->Irm = modregrm(2,c->Irm & 7,4);
                    c->Isib = modregrm(0,4,SP);
                    c->Iflags = CFoff;
                    c->IFL1 = FLconst;
                    c->IEV1.Vuns = EBPtoESP;
                }
#endif
            }
            continue;
        }

#if 0
        unsigned char ins = inssize[c->Iop & 0xFF];
        if (!(ins & M) ||
            ((rm = c->Irm) & 0xC0) == 0xC0)
            goto do2;           /* if no first operand          */
        if (is32bitaddr(I32,c->Iflags))
        {

            if (
                ((rm & 0xC0) == 0 && !((rm & 7) == 4 && (c->Isib & 7) == 5 || (rm & 7) == 5))
               )
                goto do2;       /* if no first operand  */
        }
        else
        {
            if (
                ((rm & 0xC0) == 0 && !((rm & 7) == 6))
               )
                goto do2;       /* if no first operand  */
        }
#endif
        s = c->IEVsym1;
        switch (c->IFL1)
        {
#if OMFOBJ
            case FLdata:
                if (s->Sclass == SCcomdat)
                {   c->IFL1 = FLextern;
                    goto do2;
                }
#if MARS
                c->IEVseg1 = s->Sseg;
#else
                c->IEVseg1 = DATA;
#endif
                c->IEVpointer1 += s->Soffset;
                c->IFL1 = FLdatseg;
                goto do2;
            case FLudata:
#if MARS
                c->IEVseg1 = s->Sseg;
#else
                c->IEVseg1 = UDATA;
#endif
                c->IEVpointer1 += s->Soffset;
                c->IFL1 = FLdatseg;
                goto do2;
#else                                   // don't loose symbol information
            case FLdata:
            case FLudata:
            case FLtlsdata:
                c->IFL1 = FLextern;
                goto do2;
#endif
            case FLdatseg:
                c->IEVseg1 = DATA;
                goto do2;

            case FLpseudo:
                goto do2;

            case FLstack:
                //printf("Soffset = %d, EBPtoESP = %d, base = %d, pointer = %d\n",
                //s->Soffset,EBPtoESP,base,c->IEVpointer1);
                c->IEVpointer1 += s->Soffset + EBPtoESP - base - EEStack.offset;
                break;

            case FLfast:
                soff = Fast.size;
                goto L1;
            case FLreg:
            case FLauto:
                soff = Auto.size;
            L1:
                if (s->Sflags & SFLunambig && !(s->Sflags & SFLread) && // if never loaded
                    !anyiasm &&
                    // if not optimized, leave it in for debuggability
                    (config.flags4 & CFG4optimized || !config.fulltypes))
                {
                    c->Iop = NOP;               // remove references to it
                    continue;
                }
                if (s->Sfl == FLreg && c->IEVpointer1 < 2)
                {
                    int reg = s->Sreglsw;

                    assert(!(s->Sregm & ~mask[reg]));
                    if (c->IEVpointer1 == 1)
                    {
                        assert(0);
#if 0
                        assert(reg < 4);    /* must be a BYTEREGS   */
                        reg |= 4;           /* convert to high byte reg */
#endif
                    }
                    printf("assignaddrc, before: "); c->print();
// broken, need to figure out what the actual requirements are
                    assert(c->Iop == OP_LDR);
                    c->Iop = OP_MOV;
                    c->setAddrMode(IAMMode_none);
                    c->setRegM(reg); // really the indexreg, using src for now
                    printf("assignaddrc, after: "); c->print();
                    //c->Irm = (c->Irm & modregrm(0,7,0)) | modregrm(3,0,reg);
                    goto do2;
                }
                else
                {
                    c->IEVpointer1 += s->Soffset + soff + BPoff;
                    if (s->Sflags & SFLunambig)
                        c->Iflags |= CFunambig;
            L2:
                    if (!hasframe)
                    {
                        assert(0);
#if 0
                        /* Convert to ESP relative address instead of EBP */
                        unsigned char rm;

                        assert(!I16);
                        c->IEVpointer1 += EBPtoESP;
                        rm = c->Irm;
                        if ((rm & 7) == 4)              // if SIB byte
                        {
                            assert((c->Isib & 7) == BP);
                            assert((rm & 0xC0) != 0);
                            c->Isib = (c->Isib & ~7) | modregrm(0,0,SP);
                        }
                        else
                        {
                            assert((rm & 7) == 5);
                            c->Irm = (rm & modregrm(0,7,0))
                                    | modregrm(2,0,4);
                            c->Isib = modregrm(0,4,SP);
                        }
#endif
                    }
                }
                break;
            case FLpara:
                soff = Para.size - BPoff;    // cancel out add of BPoff
                goto L1;
            case FLfltreg:
                c->IEVpointer1 += Foff + BPoff;
                c->Iflags |= CFunambig;
                goto L2;
            case FLallocatmp:
                c->IEVpointer1 += Alloca.offset + BPoff;
                goto L2;
            case FLbprel:
                c->IEVpointer1 += s->Soffset;
                break;
            case FLcs:
                sn = c->IEV1.Vuns;
                if (!CSE_loaded(sn))            // if never loaded
                {       c->Iop = NOP;
                        continue;
                }
                c->IEVpointer1 = sn * REGSIZE + CSoff + BPoff;
                c->Iflags |= CFunambig;
                goto L2;
            case FLregsave:
                sn = c->IEV1.Vuns;
                c->IEVpointer1 = sn + regsave.off + BPoff;
                c->Iflags |= CFunambig;
                goto L2;
            case FLoffset:
                break;
            case FLlocalsize:
                c->IEVpointer1 += localsize;
                break;
            case FLconst:
            default:
                goto do2;
        }
        c->IFL1 = FLconst;
    do2:
        ;
    }
}

int branch(block *bl, int flag)
{
    //assert(0);
    return 0;
}

void jmpaddr(code *c)
{
    //printf("jmpaddr()\n");
    code* cstart = c; // remember start of code
    while (c)
    {
        unsigned op = c->Iop;
        if ((op == OP_BL || op == OP_B) && c->IFL1 == FLcode)
        {
            code* ctarg = c->IEV1.Vcode;  // target code
            printf("looking for %p\n", ctarg);

            // try looking forward for jump target instruction
            code* ci = c; //code_next(c);
            targ_size_t ad = 0;           // IP displacement
            while (ci && ci != ctarg)
            {
                ad += calccodsize(ci);
                ci = code_next(ci);
            }

            if (ci)
            {
                c->IEVoffset1 = ad;
                c->IFL1 = FLconst;
            }
            else
            {
                // didn't find the target, try looking forward from start
                for (ci = cstart; ci != ctarg; ci = code_next(ci))
                    if (!ci || ci == c)
                        assert(0);
                ad = 0;                       // IP displacement
                while (ci != c)
                {
                    assert(ci);
                    ad += calccodsize(ci);
                    ci = code_next(ci);
                }
                c->IEVoffset1 = -ad;
                c->IFL1 = FLconst;
            }
        }

        c = code_next(c);
    }
}

void pinholeopt(code *c, block *b)
{
    // For now, no optimizations
}

code* prolog_ifunc(tym_t* tyf)
{
    assert(0);
    return NULL;
}

code* prolog_ifunc2(tym_t tyf, tym_t tym, bool pushds)
{
    if (tym == TYifunc)
        assert(0);

    return NULL;
}

code* prolog_16bit_windows_farfunc(tym_t* tyf, bool* pushds)
{
    assert(0);
    return NULL;
}

code* prolog_frame(unsigned farfunc, unsigned* xlocalsize, bool* enter, int* cfa_offset)
{
    code* c = NULL;
    *cfa_offset = 0;
    c = gen_OP_PUSH(c, config.fulltypes ? CFvolatile : 0, mFP | mLR);
    c = gen_OP_data_proc(c, OP_MOV, config.fulltypes ? CFvolatile : 0, FP, SP);

    if (usednteh & ~NTEHjmonitor && (config.exe == EX_WIN32))
    {
        code *ce = nteh_prolog();
        c = cat(c,ce);
        int sz = nteh_contextsym_size();
        assert(sz != 0);        // should be 5*4, not 0
        *xlocalsize -= sz;      // sz is already subtracted from ESP by nteh_prolog()
    }

    if (config.fulltypes)
    {   int off = 2 * REGSIZE;
        dwarf_CFA_set_loc(1);              // address after PUSH EBP
        dwarf_CFA_set_reg_offset(SP, off); // CFA is now 8[ESP]
        dwarf_CFA_offset(BP, -off);        // EBP is at 0[ESP]
        dwarf_CFA_set_loc(3);              // address after MOV EBP,ESP
        // Yes, I know the parameter is 8 when we mean 0!
        // But this gets the cfa register set to EBP correctly
        dwarf_CFA_set_reg_offset(BP, off); // CFA is now 0[EBP]
        *cfa_offset = off;  // remember the difference between the CFA and the frame pointer
    }

    return c;
}

code* prolog_frameadj(tym_t tyf, unsigned xlocalsize, bool enter, bool* pushalloc)
{
    return prolog_frameadj2(tyf, xlocalsize, pushalloc);
}

code* prolog_frameadj2(tym_t tyf, unsigned xlocalsize, bool* pushalloc)
{
    code* c = NULL;
    if (xlocalsize == REGSIZE)
    {
        *pushalloc = true;
        c = gen_OP_PUSH(NULL, 0, mR0);
    }
    else
        c = cod3_stackadj(NULL, xlocalsize);

    return c;
}

code* prolog_setupalloca()
{
    assert(0);
    return NULL;
}

code* prolog_saveregs(code *c, regm_t topush, int cfa_offset)
{
    return gen_OP_PUSH(c, 0, topush);
}

code* prolog_trace(bool farfunc, unsigned* regsaved)
{
    assert(0);
    return NULL;
}

code* prolog_genvarargs(symbol* sv, regm_t* namedargs)
{
    assert(0);
    return NULL;
}

code* prolog_gen_win64_varargs()
{
    assert(0);
    return NULL;
}

code* prolog_loadparams(tym_t tyf, bool pushalloc, regm_t* namedargs)
{
#ifdef DEBUG
    for (SYMIDX si = 0; si < globsym.top; si++)
    {   symbol *s = globsym.tab[si];
        if (debugr && s->Sclass == SCfastpar)
        {
            printf("symbol '%s' is fastpar in register [%s,%s]\n", s->Sident,
                regm_str(mask[s->Spreg]),
                (s->Spreg2 == NOREG ? "NOREG" : regm_str(mask[s->Spreg2])));
            if (s->Sfl == FLreg)
                printf("\tassigned to register %s\n", regm_str(mask[s->Sreglsw]));
        }
    }
#endif

    code* c = NULL;
    for (SYMIDX si = 0; si < globsym.top; si++)
    {
        symbol *s = globsym.tab[si];

        if (s->Sclass == SCfastpar && s->Sfl != FLreg)
        {   // Argument is passed in a register

            type *t = s->Stype;
            if (tybasic(t->Tty) == TYstruct)
            {   type *targ1 = t->Ttag->Sstruct->Sarg1type;
                if (targ1)
                    t = targ1;
            }

            if (s->Sflags & SFLdead ||
                    (!anyiasm && !(s->Sflags & SFLread) && s->Sflags & SFLunambig &&
#if MARS
                     // This variable has been reference by a nested function
                     !(s->Stype->Tty & mTYvolatile) &&
#endif
                     (config.flags4 & CFG4optimized || !config.fulltypes)))
            {
                // Ignore it, as it is never referenced
                ;
            }
            else
            {
                targ_size_t offset = Auto.size + BPoff + s->Soffset;
                if (!hasframe)
                    offset += EBPtoESP;

                unsigned preg = s->Spreg;
                for (int i = 0; i < 2; ++i)     // twice, once for each possible parameter register
                {
                    if (mask[preg] & XMMREGS)
                    {
                        assert(0);
                        // op = xmmstore(t->Tty);
                        // gen code to deal with xmm stuff
                    }
                    else if (!(pushalloc && preg == R0))
                        c = gen_OP_STR(c, 0, preg, (hasframe ? FP : SP), FLconst, NULL, offset);

                    preg = s->Spreg2;
                    if (preg == NOREG)
                        break;
                    offset += REGSIZE;
                }
            }
        }
    }

    /* Copy SCfastpar (parameters passed in registers) that were assigned registers
     * into their assigned registers.
     * Note that we have a big problem if Pa is passed in R1 and assigned to R2,
     * and Pb is passed in R2 but assigned to R1. Detect it and assert.
     */
    regm_t assignregs = 0;
    for (SYMIDX si = 0; si < globsym.top; si++)
    {
        symbol *s = globsym.tab[si];
        unsigned sz = type_size(s->Stype);

        if (s->Sclass == SCfastpar)
            *namedargs |= s->Spregm();

        if (s->Sclass == SCfastpar && s->Sfl == FLreg)
        {
            // Argument is passed in a register
            unsigned preg = s->Spreg;
            assert(s->Spreg2 == NOREG); // currently register pairs are never assigned to
                                        // parameters passed in a pair

            assert(!(mask[preg] & assignregs));         // not already stepped on
            assignregs |= mask[s->Sreglsw];

            // MOV reg,preg
            if (mask[preg] & XMMREGS)
            {
                assert(0);
#if 0
                type *t = s->Stype;
                if (tybasic(t->Tty) == TYstruct)
                {   type *targ1 = t->Ttag->Sstruct->Sarg1type;
                    if (targ1)
                        t = targ1;
                }

                unsigned op = xmmload(t->Tty);      // MOVSS/D xreg,preg
                unsigned xreg = s->Sreglsw - XMM0;
                c = gen2(c,op,modregxrmx(3,xreg,preg - XMM0));
#endif
            }
            else
            {
                c = genmovreg(c, s->Sreglsw, preg);
            }
        }
    }

    /* For parameters that were passed on the stack, but are enregistered,
     * initialize the registers with the parameter stack values.
     * Do not use assignaddr(), as it will replace the stack reference with
     * the register.
     */
    for (SYMIDX si = 0; si < globsym.top; si++)
    {
        symbol *s = globsym.tab[si];
        unsigned sz = type_size(s->Stype);

        if ((s->Sclass == SCregpar || s->Sclass == SCparameter) &&
            s->Sfl == FLreg &&
            (refparam
#if MARS
                // This variable has been reference by a nested function
                || s->Stype->Tty & mTYvolatile
#endif
                ))
        {  
            /* MOV reg,param[BP]        */
            //assert(refparam);
            if (mask[s->Sreglsw] & XMMREGS)
            {
                assert(0);
#if 0
                unsigned op = xmmload(s->Stype->Tty);  // MOVSS/D xreg,mem
                unsigned xreg = s->Sreglsw - XMM0;
                code *c2 = genc1(CNIL,op,modregxrm(2,xreg,BPRM),FLconst,Para.size + s->Soffset);
                if (!hasframe)
                {   // Convert to ESP relative address rather than EBP
                    c2->Irm = modregxrm(2,xreg,4);
                    c2->Isib = modregrm(0,4,SP);
                    c2->IEVpointer1 += EBPtoESP;
                }
                c = cat(c,c2);
#endif
            }
            else
            {
                targ_size_t off = Para.size + s->Soffset;
                if (!hasframe) off += EBPtoESP;

                unsigned flags = 0;
                if (sz == SHORTSIZE)
                {
                    assert(0);
                    //flags |= CFopsize;
                }
                c = gen_OP_LDR(c, flags, s->Sreglsw, (hasframe ? FP : SP), FLconst, NULL, off);

                if (sz > REGSIZE)
                {
                    off += REGSIZE;
                    c = gen_OP_LDR(c, 0, s->Sregmsw, (hasframe ? FP : SP), FLconst, NULL, off);
                }
            }
        }
    }

    return c;
}

// pseudo return value from epilog
extern targ_size_t retsize;

void epilog(block *b)
{
    tym_t tyf = funcsym_p->ty();
    tym_t tym = tybasic(tyf);
    targ_size_t xlocalsize = localsize;
    code *c = NULL;

    if (!(b->Bflags & BFLepilog))       // if no epilog code
        goto Lret;                      // just generate RET

    retsize = 0;

    if (tyf & mTYnaked)                 // if no prolog/epilog
        return;

    if (config.flags & CFGtrace &&
        (!(config.flags4 & CFG4allcomdat) ||
        funcsym_p->Sclass == SCcomdat ||
        funcsym_p->Sclass == SCglobal ||
        (config.flags2 & CFG2comdat && SymInline(funcsym_p))
        ))
        assert(0); // call trace function

    if (usednteh & ~NTEHjmonitor && (config.exe == EX_WIN32 || MARS))
        c = cat(c, nteh_epilog());

    {
        // ARM: can this be done with a single multi-register pop?
        unsigned reg = R15;
        regm_t regm = 1 << reg;
        regm_t topop = fregsaved & ~mfuncreg;
        while (topop)
        {
            if (topop & regm)
            {
                c = gen_OP_POP(c, 0, regm);
                topop ^= regm;
            }
            regm >>= 1;
            reg--;
        }
    }

    if (usednteh & NTEHjmonitor)
    {
        regm_t retregs = 0;
        if (b->BC == BCretexp)
            retregs = regmask(b->Belem->Ety, tym);
        code *cn = nteh_monitor_epilog(retregs);
        c = cat(c, cn);
        xlocalsize += 8;
    }

    if (needframe || (xlocalsize && hasframe))
    {
        if (xlocalsize | Alloca.size)
        {
            c = gen_OP_data_proc(c, OP_MOV, 0, SP, FP);
            c = gen_OP_POP(c, 0, mFP | mLR);
        }
        else
            c = gen_OP_POP(c, 0, mFP | mLR);
        // do I need to have code that deals with xlocalsize?
    }
    else if (xlocalsize == REGSIZE)
    {
        mfuncreg &= ~mR0;
        c = gen_OP_POP(c, 0, mR0);
    }
    else
        c = cod3_stackadj(c, -xlocalsize);

    if (b->BC == BCret || b->BC == BCretexp)
    {
Lret:
        if (tym == TYhfunc)
            assert(0);

        // if caller cleans the stack or nothing pushed on the stack anyway do a regular RET
        else if (!typfunc(tym) || Para.offset == 0)
            c = gen_OP_BX(c, LR);

        else
            assert(0);
    }

    // re-add the SP calculation optimization logic from x86's code

    retsize += calcblksize(c);          // compute size of function epilog
    b->Bcode = cat(b->Bcode, c);
}

code* gen_spill_reg(Symbol* s, bool toreg)
{
    elem *e = el_var(s);
    int sz = type_size(s->Stype);

#if 0
    code cs;
    cs.Iop = toreg ? 0x8B : 0x89; // MOV reg,mem[ESP] : MOV mem[ESP],reg
    cs.Iop ^= (sz == 1);
    code* c = getlvalue(&cs, e, toreg ? RMload : RMstore); //keepmsk);
    cs.orReg(s->Sreglsw);
    if (I64 && sz == 1 && s->Sreglsw >= 4)
        cs.Irex |= REX;
    c = gen(c,&cs);
    if (sz > REGSIZE)
    {
        cs.setReg(s->Sregmsw);
        getlvalue_msw(&cs);
        c = gen(c,&cs);
    }

    el_free(e);

    return c;
#else
    assert(0);
    return NULL;
#endif
}

/*******************************
 * Generate block exit code
 */
void outblkexitcode(block *b, code*& c, int& anyspill, const char* sflsave, symbol** retsym, const regm_t mfuncregsave)
{
    elem *e = b->Belem;
    block *nextb;
    regm_t retregs = 0;

    switch (b->BC)                     /* block exit condition         */
    {
        case BCiftrue:
        {
            bool jcond = TRUE;
            block* bs1 = list_block(b->Bsucc);
            block* bs2 = list_block(list_next(b->Bsucc));
            if (bs1 == b->Bnext)
            {   // Swap bs1 and bs2
                jcond = !jcond;
                block* btmp = bs1;
                bs1 = bs2;
                bs2 = btmp;
            }
            c = cat(c, logexp(e, jcond, FLblock, (code *) bs1));
            nextb = bs2;
            b->Bcode = NULL;
        }
        L2:
            if (nextb != b->Bnext)
            {   if (configv.addlinenumbers && b->Bsrcpos.Slinnum &&
                    !(funcsym_p->ty() & mTYnaked))
                    cgen_linnum(&c,b->Bsrcpos);
                assert(!(b->Bflags & BFLepilog));
                c = cat(c, genjmp(NULL, COND_AL, FLblock, nextb));
            }
            b->Bcode = cat(b->Bcode,c);
            break;
        case BCjmptab:
        case BCifthen:
        case BCswitch:
            assert(!(b->Bflags & BFLepilog));
            doswitch(b);               /* hide messy details           */
            b->Bcode = cat(c,b->Bcode);
            break;
#if MARS
        case BCjcatch:
            // Mark all registers as destroyed. This will prevent
            // register assignments to variables used in catch blocks.
            c = cat(c,getregs(allregs));
            goto case_goto;
#endif
#if SCPP
        case BCcatch:
            // Mark all registers as destroyed. This will prevent
            // register assignments to variables used in catch blocks.
            c = cat(c,getregs(allregs));
            goto case_goto;

        case BCtry:
            usednteh |= EHtry;
            if (config.exe == EX_WIN32)
                usednteh |= NTEHtry;
            goto case_goto;
#endif
        case BCgoto:
            nextb = list_block(b->Bsucc);
            if ((funcsym_p->Sfunc->Fflags3 & Fnteh ||
                 (MARS /*&& config.exe == EX_WIN32*/)) &&
                b->Btry != nextb->Btry &&
                nextb->BC != BC_finally)
            {   int toindex;
                int fromindex;

                b->Bcode = NULL;
                c = gencodelem(c,e,&retregs,TRUE);
                toindex = nextb->Btry ? nextb->Btry->Bscope_index : -1;
                assert(b->Btry);
                fromindex = b->Btry->Bscope_index;
#if MARS
                if (toindex + 1 == fromindex)
                {   // Simply call __finally
                    if (b->Btry &&
                        list_block(list_next(b->Btry->Bsucc))->BC == BCjcatch)
                    {
                        goto L2;
                    }
                }
#endif
                if (config.exe == EX_WIN32)
                    c = cat(c,nteh_unwind(0,toindex));
#if MARS && (TARGET_LINUX || TARGET_OSX || TARGET_FREEBSD || TARGET_OPENBSD || TARGET_SOLARIS)
                else if (toindex + 1 <= fromindex)
                {
                    //c = cat(c, linux_unwind(0, toindex));
                    block *bt;

                    //printf("B%d: fromindex = %d, toindex = %d\n", b->Bdfoidx, fromindex, toindex);
                    bt = b;
                    while ((bt = bt->Btry) != NULL && bt->Bscope_index != toindex)
                    {   block *bf;

                        //printf("\tbt->Bscope_index = %d, bt->Blast_index = %d\n", bt->Bscope_index, bt->Blast_index);
                        bf = list_block(list_next(bt->Bsucc));
                        // Only look at try-finally blocks
                        if (bf->BC == BCjcatch)
                            continue;

                        if (bf == nextb)
                            continue;
                        //printf("\tbf = B%d, nextb = B%d\n", bf->Bdfoidx, nextb->Bdfoidx);
                        if (nextb->BC == BCgoto &&
                            !nextb->Belem &&
                            bf == list_block(nextb->Bsucc))
                            continue;

#if 0
                        // call __finally
                        code *cs;
                        code *cr;
                        int nalign = 0;

                        gensaverestore(retregs,&cs,&cr);
                        if (STACKALIGN == 16)
                        {   int npush = (numbitsset(retregs) + 1) * REGSIZE;
                            if (npush & (STACKALIGN - 1))
                            {   nalign = STACKALIGN - (npush & (STACKALIGN - 1));
                                cs = genc2(cs,0x81,modregrm(3,5,SP),nalign); // SUB ESP,nalign
                                if (I64)
                                    code_orrex(cs, REX_W);
                            }
                        }
                        cs = genc(cs,0xE8,0,0,0,FLblock,(long)list_block(bf->Bsucc));
                        if (nalign)
                        {   cs = genc2(cs,0x81,modregrm(3,0,SP),nalign); // ADD ESP,nalign
                            if (I64)
                                code_orrex(cs, REX_W);
                        }
                        c = cat3(c,cs,cr);
#else
                        assert(0);
#endif
                    }
                }
#endif
                goto L2;
            }
        case_goto:
            c = gencodelem(c,e,&retregs,TRUE);
            if (anyspill)
            {   // Add in the epilog code
                code *cstore = NULL;
                code *cload = NULL;

                for (int i = 0; i < anyspill; i++)
                {   symbol *s = globsym.tab[i];

                    if (s->Sflags & SFLspill &&
                        vec_testbit(dfoidx,s->Srange))
                    {
                        s->Sfl = sflsave[i];    // undo block register assignments
                        cgreg_spillreg_epilog(b,s,&cstore,&cload);
                    }
                }
                c = cat3(c,cstore,cload);
            }

        L3:
            b->Bcode = NULL;
            nextb = list_block(b->Bsucc);
            goto L2;

        case BC_try:
            if (config.exe == EX_WIN32)
            {   usednteh |= NTEH_try;
                nteh_usevars();
            }
            else
                usednteh |= EHtry;
            goto case_goto;

        case BC_finally:
            // Mark all registers as destroyed. This will prevent
            // register assignments to variables used in finally blocks.
            assert(!getregs(allregs));
            assert(!e);
            assert(!b->Bcode);
#if TARGET_LINUX || TARGET_OSX || TARGET_FREEBSD || TARGET_OPENBSD || TARGET_SOLARIS
            if (config.flags3 & CFG3pic)
            {
#if 0
                int nalign = 0;
                if (STACKALIGN == 16)
                {   nalign = STACKALIGN - REGSIZE;
                    c = genc2(c,0x81,modregrm(3,5,SP),nalign); // SUB ESP,nalign
                    if (I64)
                        code_orrex(c, REX_W);
                }
                // CALL b->Bsucc
                c = genc(c,0xE8,0,0,0,FLblock,(long)list_block(b->Bsucc));
                if (nalign)
                {   c = genc2(c,0x81,modregrm(3,0,SP),nalign); // ADD ESP,nalign
                    if (I64)
                        code_orrex(c, REX_W);
                }
                // JMP list_next(b->Bsucc)
                nextb = list_block(list_next(b->Bsucc));
                goto L2;
#else
                assert(0);
#endif
            }
            else
#endif
            {
                // Generate a PUSH of the address of the successor to the
                // corresponding BC_ret
                //assert(list_block(list_next(b->Bsucc))->BC == BC_ret);
                // PUSH &succ
#if 0
                c = genc(c,0x68,0,0,0,FLblock,(long)list_block(list_next(b->Bsucc)));
                nextb = list_block(b->Bsucc);
                goto L2;
#else
                assert(0);
#endif
            }

        case BC_ret:
            c = gencodelem(c,e,&retregs,TRUE);
#if 0
            b->Bcode = gen1(c,0xC3);   // RET
#else
            assert(0);
#endif
            break;

#if NTEXCEPTIONS
        case BC_except:
            assert(!e);
            usednteh |= NTEH_except;
#if 0
            c = cat(c,nteh_setsp(0x8B));
#else
            assert(0);
#endif
            getregs(allregs);
            goto L3;

        case BC_filter:
            c = cat(c,nteh_filter(b));
            // Mark all registers as destroyed. This will prevent
            // register assignments to variables used in filter blocks.
            getregs(allregs);
            retregs = regmask(e->Ety, TYnfunc);
            c = gencodelem(c,e,&retregs,TRUE);
#if 0
            b->Bcode = gen1(c,0xC3);   // RET
#else
            assert(0);
#endif
            break;
#endif

        case BCretexp:
            retregs = regmask(e->Ety, funcsym_p->ty());

            // For the final load into the return regs, don't set regcon.used,
            // so that the optimizer can potentially use retregs for register
            // variable assignments.

            if (config.flags4 & CFG4optimized)
            {   regm_t usedsave;

                c = cat(c,docommas(&e));
                usedsave = regcon.used;
                if (EOP(e))
                    c = gencodelem(c,e,&retregs,TRUE);
                else
                {
                    if (e->Eoper == OPconst)
                        regcon.mvar = 0;
                    c = gencodelem(c,e,&retregs,TRUE);
                    regcon.used = usedsave;
                    if (e->Eoper == OPvar)
                    {   symbol *s = e->EV.sp.Vsym;

                        if (s->Sfl == FLreg && s->Sregm != mR0)
                            *retsym = s;
                    }
                }
            }
            else
            {
        case BCret:
        case BCexit:
                c = gencodelem(c,e,&retregs,TRUE);
            }
            b->Bcode = c;
#if TX86
            if (retregs == mST0)
            {   assert(stackused == 1);
                pop87();                // account for return value
            }
            else if (retregs == mST01)
            {   assert(stackused == 2);
                pop87();
                pop87();                // account for return value
            }
#endif
            if (b->BC == BCexit && config.flags4 & CFG4optimized)
                mfuncreg = mfuncregsave;
            if (MARS || usednteh & NTEH_try)
            {   block *bt;

                bt = b;
                while ((bt = bt->Btry) != NULL)
                {   block *bf;

                    bf = list_block(list_next(bt->Bsucc));
#if MARS
                    // Only look at try-finally blocks
                    if (bf->BC == BCjcatch)
                    {
                        continue;
                    }
#endif
                    if (config.exe == EX_WIN32)
                    {
                        if (bt->Bscope_index == 0)
                        {
                            // call __finally
                            code *cs;
                            code *cr;

                            c = cat(c,nteh_gensindex(-1));
                            gensaverestore(retregs,&cs,&cr);
#if 0
                            cs = genc(cs,0xE8,0,0,0,FLblock,(long)list_block(bf->Bsucc));
#else
                            assert(0);
#endif
                            b->Bcode = cat3(c,cs,cr);
                        }
                        else
                            b->Bcode = cat(c,nteh_unwind(retregs,~0));
                        break;
                    }
                    else
                    {
#if 0
                        // call __finally
                        code *cs;
                        code *cr;
                        int nalign = 0;

                        gensaverestore(retregs,&cs,&cr);
                        if (STACKALIGN == 16)
                        {   int npush = (numbitsset(retregs) + 1) * REGSIZE;
                            if (npush & (STACKALIGN - 1))
                            {   nalign = STACKALIGN - (npush & (STACKALIGN - 1));
                                cs = genc2(cs,0x81,modregrm(3,5,SP),nalign); // SUB ESP,nalign
                                if (I64)
                                    code_orrex(cs, REX_W);
                            }
                        }
                        // CALL bf->Bsucc
                        cs = genc(cs,0xE8,0,0,0,FLblock,(long)list_block(bf->Bsucc));
                        if (nalign)
                        {   cs = genc2(cs,0x81,modregrm(3,0,SP),nalign); // ADD ESP,nalign
                            if (I64)
                                code_orrex(cs, REX_W);
                        }
                        b->Bcode = c = cat3(c,cs,cr);
#else
                        assert(0);
#endif
                    }
                }
            }
            break;

#if SCPP || MARS
        case BCasm:
            assert(!e);
            // Mark destroyed registers
            assert(!c);
            c = cat(c,getregs(iasm_regs(b)));
            if (b->Bsucc)
            {   nextb = list_block(b->Bsucc);
                if (!b->Bnext)
                    goto L2;
                if (nextb != b->Bnext &&
                    b->Bnext &&
                    !(b->Bnext->BC == BCgoto &&
                     !b->Bnext->Belem &&
                     nextb == list_block(b->Bnext->Bsucc)))
                {   code *cl;

                    // See if already have OP_B(ranch) at end of block
                    cl = code_last(b->Bcode);
                    if (!cl || cl->Iop != OP_B)
                        goto L2;
                }
            }
            break;
#endif
        default:
#ifdef DEBUG
            printf("b->BC = %d\n",b->BC);
#endif
            assert(0);
    }
}


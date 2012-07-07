
/*********************************
 * Load the offset portion of the address represented by e into
 * reg.
 */

code *getoffset(elem *e, unsigned reg)
{
    //printf("getoffset(e = %p, reg = %d)\n", e, reg);
    assert(e->Eoper == OPvar || e->Eoper == OPrelconst);

    code cs;
    code *c;

    enum FL fl = el_fl(e);
    switch (fl)
    {
        case FLdatseg:
            cs.IEVpointer1 = e->EV.Vpointer;
            goto L3;

        case FLtlsdata:
            assert(0);
#if TARGET_LINUX || TARGET_OSX || TARGET_FREEBSD || TARGET_OPENBSD || TARGET_SOLARIS
            {
L5:
                assert(0);
#if 0
                /* Generate:
                 *      MOV reg,GS:[00000000]
                 *      ADD reg, offset s@TLS_LE
                 * for locals, and for globals:
                 *      MOV reg,GS:[00000000]
                 *      ADD reg, s@TLS_IE
                 * note different fixup
                 */
                int stack = 0;
                c = NULL;
                if (reg == STACK)
                {   regm_t retregs = ALLREGS;

                    c = allocreg(&retregs,&reg,TYoffset);
                    reg = findreg(retregs);
                    stack = 1;
                }

                code css;
                css.Irex = rex;
                css.Iop = 0x8B;
                css.Irm = modregrm(0, 0, BPRM);
                code_newreg(&css, reg);
                css.Iflags = CFgs;
                css.IFL1 = FLconst;
                css.IEV1.Vuns = 0;
                c = gen(c, &css);               // MOV reg,GS:[00000000]

                if (e->EV.sp.Vsym->Sclass == SCstatic || e->EV.sp.Vsym->Sclass == SClocstat)
                {   // ADD reg, offset s
                    cs.Irex = rex;
                    cs.Iop = 0x81;
                    cs.Irm = modregrm(3,0,reg & 7);
                    if (reg & 8)
                        cs.Irex |= REX_B;
                    cs.Iflags = CFoff;
                    cs.IFL2 = fl;
                    cs.IEVsym2 = e->EV.sp.Vsym;
                    cs.IEVoffset2 = e->EV.sp.Voffset;
                }
                else
                {   // ADD reg, s
                    cs.Irex = rex;
                    cs.Iop = 0x03;
                    cs.Irm = modregrm(0,0,BPRM);
                    code_newreg(&cs, reg);
                    cs.Iflags = CFoff;
                    cs.IFL1 = fl;
                    cs.IEVsym1 = e->EV.sp.Vsym;
                    cs.IEVoffset1 = e->EV.sp.Voffset;
                }
                c = gen(c, &cs);                // ADD reg, xxxx

                if (stack)
                {
                    c = gen1(c,0x50 + (reg & 7));      // PUSH reg
                    if (reg & 8)
                        code_orrex(c, REX_B);
                    c = genadjesp(c,REGSIZE);
                    stackchanged = 1;
                }
                break;
#endif
            }
#else
            goto L4;
#endif

        case FLfunc:
            fl = FLextern;                  /* don't want PC relative addresses */
            goto L4;

        case FLextern:
#if TARGET_LINUX || TARGET_OSX || TARGET_FREEBSD || TARGET_OPENBSD || TARGET_SOLARIS
            if (e->EV.sp.Vsym->ty() & mTYthread)
                goto L5;
#endif
        case FLdata:
        case FLudata:
#if TARGET_LINUX || TARGET_OSX || TARGET_FREEBSD || TARGET_OPENBSD || TARGET_SOLARIS
        case FLgot:
        case FLgotoff:
#endif
L4:
            cs.IEVsym1 = e->EV.sp.Vsym;
            cs.IEVoffset1 = e->EV.sp.Voffset;
L3:
            if (reg == STACK)
            {
                assert(0);
#if 0
                stackchanged = 1;
                cs.Iop = 0x68;              /* PUSH immed16                 */
                c = genadjesp(NULL,REGSIZE);
#endif
            }
            else
            {
                cs.Iop = OP_LDR;
                cs.setRegN(PC);  // we'll get that space via a pc relative address
                cs.setRegD(reg); // and store it in reg
                if (config.flags3 & CFG3pic)
                    assert(0);
#if 0
                cs.Iop = 0xB8 + (reg & 7);  // MOV reg,immed16
                if (reg & 8)
                    cs.Irex |= REX_B;
                if (I64)
                {   cs.Irex |= REX_W;
                    if (config.flags3 & CFG3pic)
                    {   // LEA reg,immed32[RIP]
                        cs.Iop = 0x8D;
                        cs.Irm = modregrm(0,reg & 7,5);
                        if (reg & 8)
                            cs.Irex = (cs.Irex & ~REX_B) | REX_R;
                        cs.IFL1 = fl;
                        cs.IEVsym1 = cs.IEVsym2;
                        cs.IEVoffset1 = cs.IEVoffset2;
                    }
                }
#endif
                c = NULL;
            }
            cs.setAddrMode(IAMMode_off);
            cs.setShiftImm(1);
            cs.Iflags = CFoff;              /* want offset only             */
            cs.IFL1 = fl;
            c = gen(c,&cs);
            break;

        case FLreg:
            /* Allow this since the tree optimizer puts & in front of       */
            /* register doubles.                                            */
            goto L2;
        case FLauto:
        case FLfast:
        case FLbprel:
        case FLfltreg:
            reflocal = TRUE;
            goto L2;
        case FLpara:
            refparam = TRUE;
L2:
            if (reg == STACK)
            {
                assert(0);
#if 0
                regm_t retregs = ALLREGS;

                c = allocreg(&retregs,&reg,TYoffset);
                reg = findreg(retregs);
                c = cat(c,loadea(e,&cs,0x8D,reg,0,0,0));    /* LEA reg,EA   */
                if (I64)
                    code_orrex(c, REX_W);
                c = gen1(c,0x50 + (reg & 7));               // PUSH reg
                if (reg & 8)
                    code_orrex(c, REX_B);
                c = genadjesp(c,REGSIZE);
                stackchanged = 1;
#endif
            }
            else
            {
                //assert(0);
                c = loadea(e,&cs,OP_MOV,reg,0,0,0);   /* LEA reg,EA           */
            }
            break;
        default:
#ifdef DEBUG
            elem_print(e);
            debugx(WRFL(fl));
#endif
            assert(0);
    }
    return c;
}

code* cdrelconst(elem *e, regm_t *pretregs)
{
    //printf("cdrelconst(e = %p)\n", e);

    code *c = NULL;

    /* The following should not happen, but cgelem.c is a little stupid.  */
    /* Assertion can be tripped by func("string" == 0); and similar       */
    /* things. Need to add goals to optelem() to fix this completely.     */
    /*assert((*pretregs & mPSW) == 0);*/
    if (*pretregs & mPSW)
    {
        *pretregs &= ~mPSW;
        c = gen_OP_data_proc(c, OP_TST, CFpsw, SP, SP); // SP is never 0
    }
    if (!*pretregs)
        return c;

    assert(e);
    tym_t tym = tybasic(e->Ety);
    switch (tym)
    {
        case TYstruct:
        case TYarray:
        case TYldouble:
        case TYildouble:
        case TYcldouble:
            tym = TYnptr;               // don't confuse allocreg()
            break;
        default:
            if (tyfunc(tym))
                tym = TYnptr;
            break;
    }
    /*assert(tym & typtr);*/              /* don't fail on (int)&a        */

    unsigned lreg;
    c = cat(c, allocreg(pretregs, &lreg, tym));
    return cat(c, getoffset(e, lreg));
}


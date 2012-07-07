
code* gen_OP_BL(code* c, unsigned fl, symbol* s)
{
    //printf("gen BL: symbol = %s\n", s->Sident);
    code cs;

    cs.Iop     = OP_BL;
    cs.IFL1    = fl;
    cs.IEVsym1 = s;

    return gen(c, &cs);
}

code* gen_OP_BX(code* c, unsigned reg)
{
    //printf("gen BX: reg = %d\n", reg);
    assert(reg <= 0xf);

    code cs;

    cs.Iop   = OP_BX;
    cs.setRegM(reg);

    return gen(c, &cs);
}

code* gen_OP_PUSH(code* c, unsigned flags, regm_t regs)
{
    //printf("gen PUSH: regs = 0x%x\n", regs);
    assert(regs <= 0xffff);

    code cs;

    cs.Iop = OP_PUSH;
    cs.Iflags = flags;
    cs.IEVsize_t1 = regs;
    cs.setRegN(SP);
    cs.setAddrMode(IAMMode_postidx);

    return gen(c, &cs);
}

code* genpush(code* c, unsigned reg)
{
    return gen_OP_PUSH(c, 0, mask[reg]);
}

code* gen_OP_POP(code* c, unsigned flags, unsigned regs)
{
    //printf("gen POP: regs = 0x%x\n", regs);
    assert(regs <= 0xffff);

    code cs;

    cs.Iop = OP_POP;
    cs.Iflags = flags;
    cs.IEVsize_t1 = regs;
    cs.setRegN(SP);
    cs.setAddrMode(IAMMode_postidx);

    return gen(c, &cs);
}

// TODO: make not suck horribly
static const char* getOpCodeName(unsigned op)
{
    static struct { const char * name; unsigned iop; } opcodelist[] = {
    #define X(a, b, c, d, e, f) {#a, b},
        OPCODES
    #undef X
    };

    for (size_t i = 0; i < sizeof(opcodelist) / sizeof(opcodelist[0]); ++i)
    {
        if (opcodelist[i].iop == op)
            return opcodelist[i].name;
    }
    return "unknown";
}

code* gen_OP_data_proc(code* c, unsigned op, unsigned flags, unsigned rd, unsigned rm)
{
    assert(rd <= 0xf);
    assert(rm <= 0xf);
    assert(op >= OP_AND && op <= OP_MVN);

    code cs;

    cs.Iop    = op;
    cs.Iflags = flags;
    if (op < OP_TST || op > OP_CMN)
        cs.setRegD(rd);
    cs.setRegN(rd);
    cs.setRegM(rm);

    //printf("gen data_proc: "); cs.print();
    return gen(c, &cs);
}

code* gen_OP_data_proc_immed(code* c, unsigned op, unsigned flags, unsigned rd, unsigned rn, targ_size_t value)
{
    assert(rd <= 0xf);
    assert(op >= OP_AND && op <= OP_MVN);

    code cs;

    cs.Iop        = op;
    cs.Iflags     = CFimmed | flags;
    cs.setRegD(rd);
    cs.setRegN(rn);
    cs.IFL1       = FLconst;
    cs.IEVsize_t1 = value;

    //printf("gen data_proc_immed: "); cs.print();
    return gen(c, &cs);
}

code* gen_OP_LDR(code* c, unsigned flags, unsigned reg, unsigned offReg, unsigned fl, symbol *s, targ_size_t off)
{
    code cs;
    cs.Iop = OP_LDR;
    cs.Iflags = flags | CFoff;
    cs.setRegD(reg);
    cs.setRegN(offReg);

    cs.setAddrMode(IAMMode_off);
    cs.setShiftImm(1);
    cs.IFL1 = fl;

    switch(fl)
    {
        case FLconst:
            cs.IEVsize_t1 = off;
            break;

        case FLauto:
        case FLbprel:
        case FLpara:
            cs.IEVsym1 = s;
            cs.IEVoffset1 = off;
            break;

        default:
            assert(0);
            break;
    }

    return gen(c, &cs);
}

code* gen_OP_STR(code* c, unsigned flags, unsigned reg, unsigned offReg, unsigned fl, symbol *s, targ_size_t off)
{
    code cs;
    cs.Iop = OP_STR;
    cs.Iflags = flags | CFoff;
    cs.setRegD(reg);
    cs.setRegN(offReg);

    cs.setAddrMode(IAMMode_off);
    cs.setShiftImm(1);
    cs.IFL1 = fl;

    switch(fl)
    {
        case FLconst:
            cs.IEVsize_t1 = off;
            break;

        case FLauto:
        case FLbprel:
        case FLpara:
            cs.IEVsym1 = s;
            cs.IEVoffset1 = off;
            break;

        default:
            assert(0);
            break;
    }

    return gen(c, &cs);
}

code *genjmp(code *c, unsigned cc, unsigned fltarg, block *targ)
{
    assert(cc >= 0x0 && cc <= 0xf);
    code cs;

    cs.Iop = OP_B;
    cs.setCC(cc);
    cs.IFL1 = fltarg;                   /* FLblock (or FLcode)          */
    cs.IEV1.Vblock = targ;              /* target block (or code)       */
    if (fltarg == FLcode)
        ((code *)targ)->Iflags |= CFtarg;

    //printf("genjmp: "); cs.print();
    return gen(c,&cs);
}

void code::print()
{
    if (Iop == OP_PUSH || Iop == OP_POP)
        printf("Iop = %s(%x), cc = %x, regs = %s\n",
                getOpCodeName(Iop), Iop, getCC(), regm_str(IEVsize_t1));
    else
        printf("Iop = %s(%x), cc = %x, Rd = %x, Rn = %x, Rm = %x, Rs = %x, addrmode = %x, fl = %x, value = %llx\n",
                getOpCodeName(Iop), Iop, getCC(), getRegD(), getRegN(), getRegM(), getRegS(), getAddrMode(), IFL1, IEVsize_t1);
}


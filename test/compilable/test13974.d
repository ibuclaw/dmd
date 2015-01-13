
// Helpers for floor/ceil.
bool isNaN(double x) @nogc @trusted pure nothrow
{
    return x != x;
}

bool isInfinity(double x) @nogc @trusted pure nothrow
{
    return (x - 1) == x;
}

double floorImpl(double x) @trusted pure nothrow @nogc
{
    version(LittleEndian)
    {
        enum EXPPOS_SHORT = 3;
        int pos = 0;
    }
    else
    {
        enum EXPPOS_SHORT = 0;
        int pos = 3;
    }

    ushort[4] vu = *(cast(ushort[4]*)&x);

    // Find the exponent (power of 2)
    int exp = ((vu[EXPPOS_SHORT] >> 4) & 0x7ff) - 0x3ff;

    if (exp < 0)
    {
        if (x < 0.0)
            return -1.0;
        else
            return 0.0;
    }

    exp = (double.mant_dig - 1) - exp;

    // Zero 16 bits at a time.
    while (exp >= 16)
    {
        version (LittleEndian)
            vu[pos++] = 0;
        else
            vu[pos--] = 0;
        exp -= 16;
    }

    // Clear the remaining bits.
    if (exp > 0)
        vu[pos] &= 0xffff ^ ((1 << exp) - 1);

    double rv = *(cast(double*)&vu);
    if ((x < 0.0) && (x != rv))
        rv -= 1.0;

    return rv;
}

double ceil(double x) @trusted pure nothrow @nogc
{
    // Special cases.
    if (isNaN(x) || isInfinity(x))
        return x;

    double y = floorImpl(x);
    if (y < x)
        y += 1.0;

    return y;
}

double floor(double x) @trusted pure nothrow @nogc
{
    // Special cases.
    if (isNaN(x) || isInfinity(x) || x == 0.0)
        return x;

    return floorImpl(x);
}

void main()
{
    static assert(floor(+123.456) == +123);
    static assert(floor(-123.456) == -124);
    static assert(floor(-1.234) == -2);
    static assert(floor(-0.123) == -1);
    static assert(floor(0.0) == 0);
    static assert(floor(+0.123) == 0);
    static assert(floor(+1.234) == 1);
    static assert(floor(double.infinity) == double.infinity);
    static assert(isNaN(floor(double.nan)));
    static assert(isNaN(floor(double.init)));

    static assert(ceil(+123.456) == +124);
    static assert(ceil(-123.456) == -123);
    static assert(ceil(-1.234) == -1);
    static assert(ceil(-0.123) == 0);
    static assert(ceil(0.0) == 0);
    static assert(ceil(+0.123) == 1);
    static assert(ceil(+1.234) == 2);
    static assert(ceil(double.infinity) == double.infinity);
    static assert(isNaN(ceil(double.nan)));
    static assert(isNaN(ceil(double.init)));
}


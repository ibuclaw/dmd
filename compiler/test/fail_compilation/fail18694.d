/*
TEST_OUTPUT:
---
fail_compilation/fail18694.d(23): Error: `pure` function `fail18694.a18694` cannot access mutable static data `s18694`
fail_compilation/fail18694.d(28): Error: `pure` function `fail18694.b18694` cannot access mutable static data `s18694`
fail_compilation/fail18694.d(33): Error: `pure` function `fail18694.c18694` cannot access mutable static data `s18694`
fail_compilation/fail18694.d(38): Error: `pure` function `fail18694.d18694` cannot access mutable static data `s18694`
fail_compilation/fail18694.d(43): Error: `pure` function `fail18694.e18694` cannot access mutable static data `s18694`
fail_compilation/fail18694.d(53): Error: `pure` function `fail18694.f18694` cannot access mutable static data `s18694`
fail_compilation/fail18694.d(58): Error: `pure` function `fail18694.gf18694` cannot access mutable static data `s18694`
---
*/
struct S18694
{
    int x();
    int y() pure;
    int z;
}
static S18694 s18694;

pure int a18694(int x)
{
    return s18694.x + x;
}

pure int b18694(int x)
{
    return s18694.x() + x;
}

pure int c18694(int x)
{
    return s18694.y + x;
}

pure int d18694(int x)
{
    return s18694.y() + x;
}

pure int e18694(int x)
{
    return s18694.z + x;
}

pure int ufc(S18694)
{
    return 0;
}

pure int f18694(int x)
{
    return s18694.ufc + x;
}

pure int gf18694(int x)
{
    return s18694.ufc() + x;
}

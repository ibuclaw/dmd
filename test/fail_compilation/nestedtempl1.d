/*
TEST_OUTPUT:
---
fail_compilation/nestedtempl1.d(13): Error: function `nestedtempl1.main.bar!(a).bar` function requires a dual-context
fail_compilation/nestedtempl1.d(25):        instantiated from here: `bar!(a)`
---
*/

auto foo(ref inout(int) x)
{
    struct S
    {
        ref inout(int) bar(alias a)() inout
        {
            return x;
        }
    }
    return S();
}

void main()
{
    int a;
    auto o = foo(a);
    o.bar!a() = 1;      // bad!
}

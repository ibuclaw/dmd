/*
TEST_OUTPUT:
---
fail_compilation/nestedtempl2.d(16): Error: function `nestedtempl2.B.func!(n).func` function requires a dual-context
fail_compilation/nestedtempl2.d(28):        instantiated from here: `func!(n)`
---
*/

class B
{
    int n;
}

void test()
{
    auto func(alias a)()
    {
        return a;
    }

    class N(alias a)
    {
    }

    auto b = new B();
    b.n = 1;

    func!(b.n)();
    auto dg = &func!(b.n);

    new N!(b.n)();
}

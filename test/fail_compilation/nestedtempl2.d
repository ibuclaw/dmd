/*
TEST_OUTPUT:
---
fail_compilation/nestedtempl2.d(18): Error: need `this` for `n` of type `int`
fail_compilation/nestedtempl2.d(28): Error: template instance `nestedtempl2.B.func!(n)` error instantiating
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

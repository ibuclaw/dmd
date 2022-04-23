/*
TEST_OUTPUT:
---
fail_compilation/nestedtempl2.d(18): Error: function `nestedtempl2.B.func!(n).func` function requires a dual-context
fail_compilation/nestedtempl2.d(30):        instantiated from here: `func!(n)`
fail_compilation/nestedtempl2.d(33): Error: `this` is only defined in non-static member functions, not `test`
fail_compilation/nestedtempl2.d(33): Error: need `this` of type `B` needed to `new` nested class `N`
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

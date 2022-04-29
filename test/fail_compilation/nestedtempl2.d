/*
TEST_OUTPUT:
---
fail_compilation/nestedtempl2.d(28): Error: need `this` for `func` of type `pure nothrow @nogc @safe int()`
fail_compilation/nestedtempl2.d(31): Error: cannot construct nested class `N` because no implicit `this` reference to outer class `B` is available
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

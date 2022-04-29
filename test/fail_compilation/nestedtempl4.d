/*
TEST_OUTPUT:
---
fail_compilation/nestedtempl4.d(17): Error: class `nestedtempl4.K.D!(1, B!(a)).D` is nested within `K`, but super class `B` is nested within `main`
fail_compilation/nestedtempl4.d(27): Error: template instance `nestedtempl4.K.D!(1, B!(a))` error instantiating
fail_compilation/nestedtempl4.d(17): Error: class `nestedtempl4.main.fun.D!(b, B!(a)).D` is nested within `fun`, but super class `B` is nested within `main`
fail_compilation/nestedtempl4.d(32): Error: template instance `nestedtempl4.main.fun.D!(b, B!(a))` error instantiating
---
*/
class K
{
    class B(alias a)
    {

    }

    class D(alias a, T) : T
    {

    }
}

void main()
{
    int a;
    auto k = new K;
    auto d = k.new K.D!(1, K.B!a);

    auto fun()
    {
        int b;
        auto o = k.new K.D!(b, K.B!a);
    }
}

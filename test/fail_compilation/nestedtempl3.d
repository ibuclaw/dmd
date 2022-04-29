/*
TEST_OUTPUT:
---
fail_compilation/nestedtempl3.d(31): Error: `.new` is only for allocating nested classes
fail_compilation/nestedtempl3.d(21): Error: class `nestedtempl3.main.E!(a).E` is nested within `main`, but super class `B` is nested within `K`
fail_compilation/nestedtempl3.d(32): Error: template instance `nestedtempl3.main.E!(a)` error instantiating
---
*/
class K
{
    class B(alias a)
    {

    }

    class D(alias a) : B!a
    {

    }

    class E(alias a) : B!1
    {

    }
}

void main()
{
    int a;
    auto k = new K;
    auto d = k.new K.D!a;
    auto e = k.new K.E!a;
}

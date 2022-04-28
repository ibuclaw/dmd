// PERMUTE_ARGS: -inline
/* TEST_OUTPUT:
---
fail_compilation/template14.d(45): Error: need `this` to access member `makeR`
fail_compilation/template14.d(46): Error: need `this` to access member `inc`
---
*/

/********************************************/
// inline tests
// https://issues.dlang.org/show_bug.cgi?id=14730

void test6a()
{
    if (__ctfe)
        return; // nested classes not yet ctfeable

    int i = 10;
    int j, k;

    class A
    {
        auto makeR(alias a)()
        {
            int m = 1;
            class O
            {
                class R
                {
                    pragma(inline, true)
                    final auto inc(alias v)()
                    {
                        ++v;
                        ++m;
                        ++i;
                    }
                    auto getM() { return m; }
                }
            }
            return new O().new R();
        }
    }

    auto a = new A;
    auto r = a.makeR!j();
    r.inc!k();          // inlined
    assert(i == 11);
    assert(k == 1);
    assert(r.getM == 2);
}

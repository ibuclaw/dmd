// PERMUTE_ARGS: -inline
/* TEST_OUTPUT:
---
fail_compilation/template13.d(34): Error: need `this` to access member `exec`
---
*/

/********************************************/
// inline tests
// https://issues.dlang.org/show_bug.cgi?id=14730

struct S6c
{
    int m;
    auto exec(alias f)()
    {
        return f(); // inlined
    }
}

void test6c()
{
    int a;

    auto f0()
    {
        auto f()
        {
            pragma(inline, true);
            return ++a;
        }

        auto s = S6c();
        s.exec!f();
        assert(a == 1);
    }

    f0();
}

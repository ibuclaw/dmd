// PERMUTE_ARGS: -inline
/* TEST_OUTPUT:
---
fail_compilation/template12.d(41): Error: need `this` to access member `f0`
---
*/

/********************************************/
// inline tests
// https://issues.dlang.org/show_bug.cgi?id=14730

auto get6b()
{
    struct S
    {
        auto f0(alias a)()
        {
            auto seta(int m) {
                pragma(inline, true);
                a = m;
            }

            struct T(alias f)
            {
                int m = 10;
                void exec()
                {
                    f(m); // inlined
                }
            }
            return T!seta();
        }
    }
    return S();
}

void test6b()
{
    int a = 1;
    auto s = get6b();
    auto t = s.f0!a();
    t.exec();
    assert(a == 10);
}

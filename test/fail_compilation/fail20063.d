/* TEST_OUTPUT:
---
fail_compilation/fail20063.d(10): Error: function `fail20063.main.f!(delegate () pure nothrow @safe => new C).f` function requires a dual-context
fail_compilation/fail20063.d(19):        instantiated from here: `f!(delegate () pure nothrow @safe => new C)`
---
*/

struct S
{
    void f(alias fun)() {}
}

auto handleLazily(T)(lazy T expr) {}

void main()
{
    class C {}

    S().f!(() => new C()).handleLazily;
}

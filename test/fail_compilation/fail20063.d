/* TEST_OUTPUT:
---
fail_compilation/fail20063.d(18): Error: cannot pass function `S() , f` as a function argument
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

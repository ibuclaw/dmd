/*
TEST_OUTPUT:
---
fail_compilation/nestedtempl1.d(24): Error: cannot modify `inout` expression `(o , bar)()`
---
*/

auto foo(ref inout(int) x)
{
    struct S
    {
        ref inout(int) bar(alias a)() inout
        {
            return x;
        }
    }
    return S();
}

void main()
{
    int a;
    auto o = foo(a);
    o.bar!a() = 1;      // bad!
}

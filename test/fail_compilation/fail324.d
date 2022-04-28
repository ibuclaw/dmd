/* TEST_OUTPUT:
---
fail_compilation/fail324.d(14): Error: need `this` to access member `doStuff`
---
*/
struct Foo
{
    void doStuff(alias fun)() {}
}

void main()
{
    Foo foo;
    foo.doStuff!( (i) { return i; })();
}

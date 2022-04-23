/* TEST_OUTPUT:
---
fail_compilation/fail324.d(17): Error: function `fail324.main.doStuff!((i)
{
return i;
}
).doStuff` function requires a dual-context
fail_compilation/fail324.d(23):        instantiated from here: `doStuff!((i)
{
return i;
}
)`
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

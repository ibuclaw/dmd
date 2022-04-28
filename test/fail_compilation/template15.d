// PERMUTE_ARGS: -inline
/* TEST_OUTPUT:
---
fail_compilation/template15.d(48): Error: need `this` to access member `getI`
---
*/

/********************************************/

auto getC13a()
{
    int i = 10;

    auto makeC() // should be a closure
    {
        int j;
        auto incJ() { ++j; }

        class C
        {
            auto getI(alias a)()
            {
                return i;
            }
        }

        return new C();
    }
    return makeC();
}

auto test13a()
{
    if (__ctfe)
        return; // nested classes not yet ctfeable

    int a;
    auto c = getC13a();

    function()
    {
        // clean stack
        pragma(inline, false);
        long[0x500] a = 0;
    }();

    assert(*cast(size_t*)c.outer != 0); // error: frame of getC.makeC wasn't made a closure
    auto i = c.getI!a(); // segfault
}

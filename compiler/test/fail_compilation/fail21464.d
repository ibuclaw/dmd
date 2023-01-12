// https://issues.dlang.org/show_bug.cgi?id=21464
// Test has been moved from compilable to fail_compilation
// EXTRA_FILES: imports/test21464a.d
/*
TEST_OUTPUT:
---
fail_compilation/fail21464.d(13): Error: `pure` function `fail21464.foo` cannot access mutable static data `instance`
---
*/
void foo() pure
{
    import imports.test21464a : Mallocator;
    auto a = Mallocator.instance; // mutable static, but empty struct
}

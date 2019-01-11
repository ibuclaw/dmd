// https://issues.dlang.org/show_bug.cgi?id=19499
/*
TEST_OUTPUT:
---
true
---
*/

enum __c_long_double : double;
enum A(T : double) = true;
enum A(T : __c_long_double) = false;
static assert(A!double == true);

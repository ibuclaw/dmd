// REQUIRED_ARGS: -D -Dd${RESULTS_DIR}/fail_compilation -o- -w -c
/* TEST_OUTPUT:
---
compilable/ddoc_18083.d(12): Ddoc: function declaration has no parameter 'this'
compilable/ddoc_18083.d(12): Ddoc: parameter count mismatch, expected 0, got 1
---
*/
/**
Params:
  this = non-existent parameter
*/
int foo()
{
    return 1;
}

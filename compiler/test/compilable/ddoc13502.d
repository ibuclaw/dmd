// PERMUTE_ARGS:
// REQUIRED_ARGS: -D -Dd${RESULTS_DIR}/compilable -wi -o-
/*
TEST_OUTPUT:
---
compilable/ddoc13502.d(14): Ddoc: Stray '('. This may cause incorrect Ddoc output. Use $(LPAREN) instead for unpaired left parentheses.
compilable/ddoc13502.d(17): Ddoc: Stray '('. This may cause incorrect Ddoc output. Use $(LPAREN) instead for unpaired left parentheses.
compilable/ddoc13502.d(21): Ddoc: Stray '('. This may cause incorrect Ddoc output. Use $(LPAREN) instead for unpaired left parentheses.
compilable/ddoc13502.d(24): Ddoc: Stray '('. This may cause incorrect Ddoc output. Use $(LPAREN) instead for unpaired left parentheses.
---
*/

/// (
enum isSomeString(T) = true;

/// (
enum bool isArray(T) = true;


/// (
extern(C) alias int T1;

/// (
extern(C) alias T2 = int;

/* PERMUTE_ARGS:
REQUIRED_ARGS: -D -Dd${RESULTS_DIR}/compilable -o-
POST_SCRIPT: compilable/extra-files/ddocAny-postscript.sh
TEST_OUTPUT:
---
compilable/ddoc15475.d(41): Ddoc: Stray '('. This may cause incorrect Ddoc output. Use $(LPAREN) instead for unpaired left parentheses.
compilable/ddoc15475.d(41): Ddoc: Stray ')'. This may cause incorrect Ddoc output. Use $(RPAREN) instead for unpaired right parentheses.
compilable/ddoc15475.d(41): Ddoc: Stray '('. This may cause incorrect Ddoc output. Use $(LPAREN) instead for unpaired left parentheses.
compilable/ddoc15475.d(41): Ddoc: Stray ')'. This may cause incorrect Ddoc output. Use $(RPAREN) instead for unpaired right parentheses.
compilable/ddoc15475.d(41): Ddoc: Stray ')'. This may cause incorrect Ddoc output. Use $(RPAREN) instead for unpaired right parentheses.
---
*/

/**
My module
----
   // Computes the interval [x,y)
   auto interval = computeInterval(x, y);
----

Backslash-escape parentheses with `\(` and `\)`.

---
(
---

---
)
---

---
    Here are some nested `backticks`
    // Another interval [x,y)
---

---
    This won't end the code block: --- )
    // Yet another interval [x,y)
---
*/
module ddoc15475;

/*
PERMUTE_ARGS:
REQUIRED_ARGS: -o- -Xf=${RESULTS_DIR}/compilable/json2.out -Xi=compilerInfo -Xi=buildInfo -Xi=modules -Xi=semantics
POST_SCRIPT: compilable/extra-files/json-postscript.sh
TEST_OUTPUT:
---
compilable/json.d(192): Deprecation: `extern(Pascal)` is deprecated. You might want to use `extern(Windows)` instead.
compilable/json.d(200): Deprecation: `extern(Pascal)` is deprecated. You might want to use `extern(Windows)` instead.
---
*/
import json;

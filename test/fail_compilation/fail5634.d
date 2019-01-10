/*
TEST_OUTPUT:
---
fail_compilation/fail5634.d(8): Error: only one `main` allowed. Previously found `main` at fail_compilation/fail5634.d(7)
---
*/
void main() { }
void main() { }


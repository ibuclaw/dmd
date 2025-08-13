struct S
{
  int i;
  short v;
} __attribute__((aligned(2)));	// ignored as smaller than int

_Static_assert(sizeof(struct S) == sizeof(int) * 2, "sizeof(S)");
_Static_assert(_Alignof(struct S) == _Alignof(int), "_Alignof(S)");

struct T
{
  int i;
  short v;
} __declspec(align(2));       	// ignored as smaller than int

_Static_assert(sizeof(struct T) == sizeof(int) * 2, "sizeof(T)");
_Static_assert(_Alignof(struct T) == _Alignof(int), "_Alignof(T)");

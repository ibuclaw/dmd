// https://issues.dlang.org/show_bug.cgi?id=18030
/*
TEST_OUTPUT:
---
Inside S: func() is public
---
*/

struct S(T)
{
	T var;
	static assert(__traits(getProtection, __traits(getMember, T, "func")) == "public");
}

class C
{
	alias Al = S!C;
	
	static void func(U)(U var) { }
}

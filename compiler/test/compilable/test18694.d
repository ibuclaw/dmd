// https://issues.dlang.org/show_bug.cgi?id=18694

struct S1 { enum int x = 42; }
static S1 a18694;
pure int test18694a(int x)
{
    return a18694.x + x;
}

struct S2 { enum int x = 42; int y; }
static S2 b18694;
pure int test18694b(int x)
{
    return b18694.x + x;
}

struct S3 { enum int x = 42; }
pure int test18694c(int x)
{
    return S3.x + x;
}

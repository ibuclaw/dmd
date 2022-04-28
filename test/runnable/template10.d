// PERMUTE_ARGS: -inline

/********************************************/

void test1a()
{
    int a = 1;

    auto func(alias b)()
    {
        ++a;
        ++b;
    }

    int b = 1;
    func!b;
    assert(a == 2);
    assert(b == 2);

    auto dg = &func!b;
    dg();
    assert(a == 3);
    assert(b == 3);

    template TA(alias i) { int v = i; }
    mixin TA!a;
    assert(v == 3);
}

auto test1b()
{
    auto f(alias a)() { return a; }

    auto f0()
    {
        int a = 10;
        return f!a();
    }
    assert(f0() == 10);
}

void test1()
{
    test1a();
    test1b();
}

/********************************************/

struct A9
{
    int m;
    auto add(alias a)()
    {
        m += a;
    }
}

struct B9
{
    int i;
    A9 a;
    alias a this;
}

struct S9
{
    int j;
    B9 b;
    alias b this;
}

void test9()
{
    int a = 9;
    auto o = S9(1, B9(1, A9(10)));
    o.add!9();
    assert(o.b.a.m == 10+9);
}

/********************************************/

void test14()
{
    int i;
    struct S(alias a)
    {
        auto get() { return a; }
        auto inc() { ++i; }
    }

    auto f0()
    {
        int a = 10;

        auto s = S!a();
        static assert(typeof(s).sizeof == size_t.sizeof * 2);

        assert(s.get() == 10);
        s.inc();
        assert(i == 1);
    }

    f0();
}

/********************************************/

int runTests()
{
    test1();
    test9();
    test14();
    return 0;
}

void main()
{
    runTests();          // runtime
    enum _ = runTests(); // ctfe
}

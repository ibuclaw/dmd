// PERMUTE_ARGS: -inline
/* TEST_OUTPUT:
---
fail_compilation/template10.d(33): Error: function `template10.test1c.v!(c).sum` cannot access variable `b` in frame of function `template10.test1c.f0.f1`
fail_compilation/template10.d(47):        `b` declared here
fail_compilation/template10.d(48): Error: template instance `template10.test1c.v!(c)` error instantiating
fail_compilation/template10.d(73): Error: need `this` of type `C2` to access member `m` from static function `test2`
fail_compilation/template10.d(85): Error: template instance `template10.test2.K!(a)` error instantiating
fail_compilation/template10.d(96): Error: need `this` for `m` of type `int`
fail_compilation/template10.d(96): Error: function `template10.test3.set!(a).set(int c)` is not callable using argument types `(_error_)`
fail_compilation/template10.d(96):        cannot pass argument `__error` of type `_error_` to parameter `int c`
fail_compilation/template10.d(106): Error: template instance `template10.test3.exec!(set)` error instantiating
fail_compilation/template10.d(133): Error: need `this` of type `S4` to access member `m` from static function `get4i`
fail_compilation/template10.d(162): Error: template instance `template10.get4i.inner!(a)` error instantiating
fail_compilation/template10.d(122): Error: need `this` for `m` of type `int`
fail_compilation/template10.d(169): Error: template instance `template10.test4.add!(a)` error instantiating
fail_compilation/template10.d(133): Error: need `this` of type `S4` to access member `m` from static function `test4`
fail_compilation/template10.d(180): Error: template instance `template10.test4.inner!(a)` error instantiating
fail_compilation/template10.d(182): Error: static assert:  `is(T0 == T1)` is false
---
*/

/********************************************/

template t1c(alias a)
{
    template u(alias b)
    {
        template v(alias c)
        {
            auto sum()
            {
                return a + b + c;
            }
        }
    }
}

void test1c()
{
    int c = 3;
    auto f0()
    {
        int a = 1;
        auto f1()
        {
            int b = 2;
            auto r = t1c!a.u!b.v!c.sum();
            assert(r == 6);
        }
        f1();
    }
    f0();
}

void test1()
{
    test1c();
}

/********************************************/

class C2
{
    int m = 10;

    class K(alias a)
    {
        int n;
        this(int i) { n = i; }
        auto sum()
        {
            return a + m + n;
        }
    }
}

void test2()
{
    if (__ctfe)
        return; // nested classes not yet ctfeable

    int a = 20;
    auto o = new C2;
    auto k = o.new C2.K!a(30);
    assert(k.sum() == 10+20+30);
}

/********************************************/

struct S3
{
    int m;
    auto exec(alias f)()
    {
        f(m);
    }
}

void test3()
{
    int a = 1;

    auto set(alias b)(int c) { b = c; }

    S3(10).exec!(set!a)();
    assert(a == 10);

    auto dg = &S3(20).exec!(set!a);
    dg();
    assert(a == 20);
}

/********************************************/

struct S4
{
    int m;

    auto add(alias a)(int b)
    {
        return a + m + b;
    }

    auto inner(alias a)(int i)
    {
        class I
        {
            int n;
            this() { n = i; }
            auto add(int b)
            {
                return a + n + m + b;
            }
            auto add2(alias c)(int b)
            {
                return a + n + m + b + c;
            }
        }
        return new I;
    }
}

void test4i(T)(T i)
{
    if (__ctfe)
        return; // nested classes not yet ctfeable

    int b = 2;

    assert(i.add(7) == 5+1+6+7);
    auto dg = &i.add;
    assert(dg(8) == 5+1+6+8);

    assert(i.add2!b(7) == 5+1+6+7+2);
    auto dg2 = &i.add2!b;
    assert(dg2(8) == 5+1+6+8+2);
}

auto get4i(int a)
{
    return new S4(5).inner!a(6);
}

void test4()
{
    int a = 1;

    assert(S4(4).add!a(10) == 4+1+10);

    auto dg = &S4(4).add!a;
    assert(dg(11) == 4+1+11);

    auto i = get4i(a);
    test4i(i);

    // type checking
    auto o0 = S4(10);
    auto o1 = S4(11);
    alias T0 = typeof(o0.inner!a(1));
    alias T1 = typeof(o1.inner!a(1));
    static assert(is(T0 == T1));
}

// PERMUTE_ARGS: -inline
/* TEST_OUTPUT:
---
fail_compilation/template11.d(51): Error: need `this` for `m` of type `int`
fail_compilation/template11.d(51): Error: function `template11.test5.fun(int m)` is not callable using argument types `(_error_)`
fail_compilation/template11.d(51):        cannot pass argument `__error` of type `_error_` to parameter `int m`
fail_compilation/template11.d(47): Error: template instance `template11.test5.add!(fun).add.exec2!(fun, add)` error instantiating
fail_compilation/template11.d(60):        instantiated from here: `add!(fun)`
fail_compilation/template11.d(190): Error: need `this` for `m` of type `int`
fail_compilation/template11.d(198): Error: need `this` of type `C7` to access member `m` from static function `test7`
fail_compilation/template11.d(215): Error: template instance `template11.test7.C!(c)` error instantiating
fail_compilation/template11.d(244): Error: `super` is only allowed in non-static class member functions
fail_compilation/template11.d(262): Error: template instance `template11.test8.__ctor!(a)` error instantiating
fail_compilation/template11.d(248): Error: need `this` for `m` of type `int`
fail_compilation/template11.d(264): Error: template instance `template11.test8.add!(b)` error instantiating
fail_compilation/template11.d(226): Error: need `this` for `get` of type `int()`
fail_compilation/template11.d(265): Error: template instance `template11.test8.sub!(b)` error instantiating
fail_compilation/template11.d(280): Error: need `this` for `m` of type `int`
fail_compilation/template11.d(280): Error: function `template11.test10.fun(int m)` is not callable using argument types `(_error_)`
fail_compilation/template11.d(280):        cannot pass argument `__error` of type `_error_` to parameter `int m`
fail_compilation/template11.d(276): Error: template instance `template11.test10.add!(fun).add.exec2!(fun, add)` error instantiating
fail_compilation/template11.d(289):        instantiated from here: `add!(fun)`
fail_compilation/template11.d(299): Error: `this` is only defined in non-static member functions, not `getVal`
fail_compilation/template11.d(319): Error: template instance `template11.test11.getVal!(a)` error instantiating
fail_compilation/template11.d(303): Error: `this` is only defined in non-static member functions, not `getRef`
fail_compilation/template11.d(323): Error: template instance `template11.test11.getRef!(a)` error instantiating
fail_compilation/template11.d(335): Error: need `this` for `n` of type `int`
fail_compilation/template11.d(369): Error: template instance `template11.N12.sum!(n)` error instantiating
fail_compilation/template11.d(345): Error: need `this` for `n` of type `int`
fail_compilation/template11.d(375): Error: template instance `template11.N12.inner!(n)` error instantiating
fail_compilation/template11.d(356): Error: need `this` for `n` of type `int`
fail_compilation/template11.d(380): Error: template instance `template11.N12.R!(n)` error instantiating
fail_compilation/template11.d(478): Error: function `template11.test13b.getC.C.fun!(n).fun` cannot access variable `i` in frame of function `template11.test13b.getC`
fail_compilation/template11.d(474):        `i` declared here
fail_compilation/template11.d(486): Error: template instance `template11.test13b.getC.C.fun!(n)` error instantiating
---
*/

/********************************************/

struct S5
{
    int m;
    auto add(alias f)(int a)
    {
        auto add(int b) { return a + b; }
        return exec2!(f, add)();
    }
    auto exec2(alias f, alias g)()
    {
        return g(f(m));
    }
}

void test5()
{
    int a = 10;
    auto o = S5(1);
    auto fun(int m) { return m + a; }
    assert(o.add!fun(20) == 1+10+20);
}

/********************************************/
// inline tests
// https://issues.dlang.org/show_bug.cgi?id=14730

void test6a()
{
    if (__ctfe)
        return; // nested classes not yet ctfeable

    int i = 10;
    int j, k;

    class A
    {
        auto makeR(alias a)()
        {
            int m = 1;
            class O
            {
                class R
                {
                    pragma(inline, true)
                    final auto inc(alias v)()
                    {
                        ++v;
                        ++m;
                        ++i;
                    }
                    auto getM() { return m; }
                }
            }
            return new O().new R();
        }
    }

    auto a = new A;
    auto r = a.makeR!j();
    r.inc!k();          // inlined
    assert(i == 11);
    assert(k == 1);
    assert(r.getM == 2);
}


auto get6b()
{
    struct S
    {
        auto f0(alias a)()
        {
            auto seta(int m) {
                pragma(inline, true);
                a = m;
            }

            struct T(alias f)
            {
                int m = 10;
                void exec()
                {
                    f(m); // inlined
                }
            }
            return T!seta();
        }
    }
    return S();
}

void test6b()
{
    int a = 1;
    auto s = get6b();
    auto t = s.f0!a();
    t.exec();
    assert(a == 10);
}

struct S6c
{
    int m;
    auto exec(alias f)()
    {
        return f(); // inlined
    }
}

void test6c()
{
    int a;

    auto f0()
    {
        auto f()
        {
            pragma(inline, true);
            return ++a;
        }

        auto s = S6c();
        s.exec!f();
        assert(a == 1);
    }

    f0();
}

void test6()
{
    test6a();
    test6b();
    test6c();
}

/********************************************/

class C7
{
    int m = 10;
    template A(alias a)
    {
        template B(alias b)
        {
            template C(alias c)
            {
                auto sum()
                {
                    return a + b + c + m;
                }
                class K
                {
                    int n;
                    this(int i) { n = i; }
                    auto sum()
                    {
                        return a + b + c + m + n;
                    }
                }
            }
        }
    }
}

void test7()
{
    if (__ctfe)
        return; // nested classes not yet ctfeable

    auto a = 1;
    auto b = 2;
    auto c = 3;

    assert(new C7().A!a.B!b.C!c.sum() == 1+2+3+10);
    assert(new C7().new C7.A!a.B!b.C!c.K(20).sum() == 1+2+3+10+20);
}

/********************************************/

interface I8
{
    int get();
    int sub(alias v)()
    {
        return get() - v;
    }
}

class A8
{
    int m;
    this(int m)
    {
        this.m = m;
    }
    this() {}
}

class B8 : A8, I8
{
    this(alias i)()
    {
        super(i);
    }
    auto add(alias v)()
    {
        return super.m + v;
    }
    int get()
    {
        return m;
    }
    this() {}
}

void test8()
{
    int a = 4;
    int b = 2;
    auto o = new B8;
    o.__ctor!a;
    assert(o.m == 4);
    assert(o.add!b == 4+2);
    assert(o.sub!b == 4-2);
}

/********************************************/

struct S10
{
    int m;
    auto add(alias f)(int a)
    {
        auto add(int b) { return a + b; }
        return exec2!(f, add)();
    }
    auto exec2(alias f, alias g)()
    {
        return g(f(m));
    }
}

void test10()
{
    int a = 10;
    auto o = S10(1);
    auto fun(int m) { return m + a; }
    assert(o.add!fun(20) == 1+10+20);
}

/********************************************/

struct S11
{
    int m;
    S11 getVal(alias a)()
    {
        return this;
    }
    ref getRef(alias a)()
    {
        return this;
    }
}

void test11()
{
    int a;
    auto s = S11(1);

    if (__ctfe)
    {
        // 'this' is currently only
        // returned by ref in __ctfe
    }
    else
    {
        ++s.getVal!a().m;
        assert(s.m == 1);
    }

    ++s.getRef!a().m;
    assert(s.m == 2);
}

/********************************************/

class B12
{
    int m;

    auto sum(alias a)()
    {
        return a + m;
    }

    auto inner(alias a)()
    {
        class I
        {
            int i = 20;
            auto sum()
            {
                return i + a;
            }
        }
        return new I();
    }

    class R(alias a)
    {
        int i = 30;
        auto sum()
        {
            return i + a;
        }
    }
}

class N12
{
    int n;

    auto sum()
    {
        auto o = new B12();
        o.m = 10;
        return o.sum!n();
    }

    auto sumi()
    {
        auto o = new B12();
        return o.inner!n().sum();
    }

    auto sumr()
    {
        auto o = new B12().new B12.R!n;
        return o.sum();
    }
}

void test12a()
{
    int i = 10;
    class A(alias a)
    {
        int sum()
        {
            return a + i;
        }
    }

    class B
    {
        int n;
        int sum()
        {
            return new A!n().sum();
        }
    }

    auto b = new B();
    b.n = 1;
    assert(b.sum() == 11);
}

void test12()
{
    auto o = new N12();
    o.n = 1;
    assert(o.sum() == 11);

    if (!__ctfe) // nested classes not yet ctfeable
    {
        assert(o.sumi() == 21);
        assert(o.sumr() == 31);
        test12a();
    }
}

/********************************************/

auto getC13a()
{
    int i = 10;

    auto makeC() // should be a closure
    {
        int j;
        auto incJ() { ++j; }

        class C
        {
            auto getI(alias a)()
            {
                return i;
            }
        }

        return new C();
    }
    return makeC();
}

auto test13a()
{
    if (__ctfe)
        return; // nested classes not yet ctfeable

    int a;
    auto c = getC13a();

    function()
    {
        // clean stack
        pragma(inline, false);
        long[0x500] a = 0;
    }();

    assert(*cast(size_t*)c.outer != 0); // error: frame of getC.makeC wasn't made a closure
    auto i = c.getI!a(); // segfault
}

void test13b()
{
    if (__ctfe)
        return; // nested classes not yet ctfeable

    auto getC()
    {
        int i = 10; // error: value lost, no closure made

        auto fun(alias a)()
        {
            return i;
        }

        class C
        {
            int n;
            auto getI()
            {
                return fun!n();
            }
        }
        return new C();
    }

    auto c = getC();
    assert(c.getI() == 10);
}

void test13()
{
    test13a();
    test13b();
}

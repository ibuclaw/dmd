/+
REQUIRED_ARGS: -HC=silent -c -o- -Icompilable/extra-files
PERMUTE_ARGS:
EXTRA_FILES: extra-files/dtoh_imports.d extra-files/dtoh_imports2.d

TEST_OUTPUT:
---
// Automatically generated by Digital Mars D Compiler

#pragma once

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>

class C;

extern void importFunc();

class ImportsC
{
};

typedef int32_t MyStdcInt;

enum
{
    IgnoreErrors = 0,
};

typedef int32_t T;

extern "C" int32_t x;

extern "C" int32_t foo(int32_t x);

extern int32_t foo2(int32_t x);

struct S;

typedef S aliasS;

struct S2;

typedef S2 aliasS2;

typedef C* aliasC;

class C2;

typedef C2* aliasC2;

typedef size_t(*F)(size_t x);

template <typename T, typename U>
struct TS final
{
    TS()
    {
    }
};

template <typename T, typename U>
using TSD = TS<T, U>;
typedef TSD<int32_t, int16_t > TSI;

using aliasName = ImportsC;

using MyStdc = MyStdcInt;

struct FullImport final
{
    using ImportsC = ::ImportsC;
    using MyStdcInt = ::MyStdcInt;
    FullImport()
    {
    }
};

struct SelectiveImports final
{
    using aliasName = ::ImportsC;
    SelectiveImports()
    {
    }
};

struct PrivateImport final
{
    PrivateImport()
    {
    }
};

typedef /* noreturn */ char Impossible[0];

template <typename T>
struct Array final
{
    uint32_t length;
    Array()
    {
    }
};

typedef Array<char > DString;
---
+/

extern (C++):

alias T = int;

extern (C) int x;

alias u = x;

extern (C) int foo(int x)
{
    return x * 42;
}

alias fun = foo;

extern (C++) int foo2(int x)
{
    return x * 42;
}

alias fun2 = foo2;

extern (C) struct S;

alias aliasS = S;

extern (C++) struct S2;

alias aliasS2 = S2;

extern (C) class C;

alias aliasC = C;

extern (C++) class C2;

alias aliasC2 = C2;

alias F = size_t function (size_t x);

extern(C++) struct TS(T, U) {}
alias TSD = TS;
alias TSI = TSD!(int, short);

public import dtoh_imports :
    importFunc,
    aliasName = ImportsC,
    MyStdc = MyStdcInt;

struct FullImport
{
    public import dtoh_imports;

}

struct SelectiveImports
{
    public import dtoh_imports :
        importFunc,
        aliasName = ImportsC;

    public static import dtoh_imports;
}

struct PrivateImport
{
    import dtoh_imports;

}

alias Impossible = noreturn;

struct Array(T)
{
    uint length;
    alias opDollar = length;
    alias dim = length;
}
alias DString = Array!(char);

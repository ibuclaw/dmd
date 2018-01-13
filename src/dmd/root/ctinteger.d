/**
 * Compiler implementation of the
 * $(LINK2 http://www.dlang.org, D programming language).
 *
 * Copyright:   Copyright (C) 2018 by The D Language Foundation, All Rights Reserved
 * Authors:     $(LINK2 http://www.digitalmars.com, Walter Bright)
 * License:     $(LINK2 http://www.boost.org/LICENSE_1_0.txt, Boost License 1.0)
 * Source:      $(LINK2 https://github.com/dlang/dmd/blob/master/src/dmd/root/ctinteger.d, root/_ctinteger.d)
 * Documentation: https://dlang.org/phobos/dmd_root_ctinteger.html
 * Coverage:    https://codecov.io/gh/dlang/dmd/src/master/src/dmd/root/ctinteger.d
 */

module dmd.root.ctinteger;

import core.stdc.stdint;
import core.checkedint;
import dmd.root.ctfloat;

// Types used by the front-end for compile-time integers

// Because int64_t and friends may be any integral type of the
// correct size, we have to explicitly ask for the correct
// integer type to get the correct mangling with dmd

static if (0)
{
    struct Int64
    {
        long v;

        this(real_t v)
        {
            this.v = cast(long)v;
        }

        this(long v)
        {
            this.v = v;
        }

        void opAssign(long v)
        {
            this.v = v;
        }

        void opAssign(T)(T other) if (isXInt!T)
        {
            this.v = other.v;
        }

        int opCmp(T)(T other) const if (isIntegral!T)
        {
            if (v > other)
                return 1;
            else if (v < other)
                return -1;
            else
                return 0;
        }

        int opCmp(T)(T other) const if (isXInt!T)
        {
            if (v > other.v)
                return 1;
            else if (v < other.v)
                return -1;
            else
                return 0;
        }

        d_int8 opCast(T : d_int8)() const
        {
            return cast(d_int8)v;
        }

        d_uns8 opCast(T : d_uns8)() const
        {
            return cast(d_uns8)v;
        }

        d_int16 opCast(T : d_int16)() const
        {
            return cast(d_int16)v;
        }

        d_uns16 opCast(T : d_uns16)() const
        {
            return cast(d_uns16)v;
        }

        d_int32 opCast(T : d_int32)() const
        {
            return cast(d_int32)v;
        }

        d_uns32 opCast(T : d_uns32)() const
        {
            return cast(d_uns32)v;
        }

        d_int64 opCast(T : d_int64)() const
        {
            return cast(d_int64)v;
        }

        bool opEquals(long other) const
        {
            return v == other;
        }

        Uint64 opCast(T : Uint64)()
        {
            return Uint64(v);
        }

        Int64 opNeg() const
        {
            return Int64(-v);
        }

        T opCast(T)() if (isFloat!T)
        {
            return cast(T)v;
        }

        bool opEquals(T)(T other) if (isFloat!T)
        {
            return v == other;
        }

        bool opEquals(T)(T other) if (isXInt!T)
        {
            return v == other.v;
        }

        Int64 opBinary(string op, T)(T other) if (isXInt!T)
        {
            return Int64(mixin("this.v " ~ op ~ " other.v"));
        }

        Int64 opBinary(string op, T)(T other) if (isIntegral!T)
        {
            return Int64(mixin("this.v " ~ op ~ " v"));
        }

        void opOpAssign(string op, T)(T other) if (isXInt!T)
        {
            mixin("this.v " ~ op ~ "= other.v;");
        }

        alias XCast = opCast!Uint64;
        alias XCast this;
    }

    enum isIntegral(T) = is(T : long) || is(T : ulong);
    enum isFloat(T) = is(T == float) || is(T == double) || is(T == real_t);
    enum isXInt(T) = is(T : Uint64) || is(T : Int64);

    struct Uint64
    {
        ulong v;

        this(ulong v)
        {
            this.v = v;
        }

        this(real_t v)
        {
            this.v = cast(ulong)v;
        }

        void opAssign(ulong v)
        {
            this.v = v;
        }

        int opCmp(T)(T other) const if (isIntegral!T)
        {
            if (v > other)
                return 1;
            else if (v < other)
                return -1;
            else
                return 0;
        }

        int opCmp(T)(T other) const if (isXInt!T)
        {
            if (v > other.v)
                return 1;
            else if (v < other.v)
                return -1;
            else
                return 0;
        }

        bool opCast(T : bool)() const
        {
            return v != 0;
        }

        float opCast(T : float)() const
        {
            return cast(float)v;
        }

        d_int8 opCast(T : d_int8)() const
        {
            return cast(d_int8)v;
        }

        d_uns8 opCast(T : d_uns8)() const
        {
            return cast(d_uns8)v;
        }

        d_int16 opCast(T : d_int16)() const
        {
            return cast(d_int16)v;
        }

        d_uns16 opCast(T : d_uns16)() const
        {
            return cast(d_uns16)v;
        }

        d_int32 opCast(T : d_int32)() const
        {
            return cast(d_int32)v;
        }

        d_uns32 opCast(T : d_uns32)() const
        {
            return cast(d_uns32)v;
        }

        d_uns64 opCast(T : d_uns64)() const
        {
            return cast(d_uns64)v;
        }

        dchar opCast(T : dchar)() const
        {
            return cast(dchar)v;
        }

        char opCast(T : char)() const
        {
            return cast(char)v;
        }

        bool opEquals(long v) const
        {
            return this.v == v;
        }

        bool opEquals(Uint64 other) const
        {
            return this.v == other.v;
        }

        Uint64 opNeg() const
        {
            return Uint64(-v);
        }

        Uint64 opCom() const
        {
            return Uint64(~v);
        }

        Int64 opCast(T : Int64)()
        {
            return Int64(this.v);
        }

        T opCast(T)() if (isFloat!T)
        {
            return cast(T)v;
        }

        bool opEquals(T)(T other) if (isFloat!T)
        {
            return v == other;
        }

        Uint64 opBinary(string op, T)(T other) const if (isXInt!T)
        {
            return Uint64(mixin("this.v " ~ op ~ " other.v"));
        }

        Uint64 opBinary(string op, T)(T other) const if (isIntegral!T)
        {
            return Uint64(mixin("this.v " ~ op ~ " v"));
        }

        Uint64 opBinaryRight(string op, T)(T other) const if (isIntegral!T)
        {
            return Uint64(mixin("v " ~ op ~ " this.v"));
        }

        ref typeof(this) opOpAssign(string op, T)(T other) if (isXInt!T)
        {
            mixin("this.v " ~ op ~ "= other.v;");
            return this;
        }

        ref typeof(this) opOpAssign(string op, T)(T other) if (isIntegral!T)
        {
            mixin("this.v " ~ op ~ "= v;");
            return this;
        }

        alias XCast = opCast!Int64;
        alias XCast this;
    }

    Uint64 mulu(Uint64 a, Uint64 b, ref bool overflow)
    {
        return Uint64(core.checkedint.mulu(a.v, b.v, overflow));
    }

    Uint64 mulu(Uint64 a, long b, ref bool overflow)
    {
        return Uint64(core.checkedint.mulu(a.v, b, overflow));
    }

    Uint64 addu(Uint64 a, long b, ref bool overflow)
    {
        return Uint64(core.checkedint.addu(a.v, b, overflow));
    }
}
else
{
    alias Uint64 = ulong;
    alias Int64 = long;
}

// Be careful not to care about sign when using dinteger_t
// use this instead of integer_t to
// avoid conflicts with system #include's
alias dinteger_t = Uint64;
// Signed and unsigned variants
alias sinteger_t = Int64;
alias uinteger_t = Uint64;

alias d_int8 = int8_t;
alias d_uns8 = uint8_t;
alias d_int16 = int16_t;
alias d_uns16 = uint16_t;
alias d_int32 = int32_t;
alias d_uns32 = uint32_t;
alias d_int64 = int64_t;
alias d_uns64 = uint64_t;

/**
 * Common type declarations used in the compiler implementation.
 *
 * Copyright:   Copyright (C) 1999-2021 by The D Language Foundation, All Rights Reserved
 * Authors:     $(LINK2 http://www.digitalmars.com, Walter Bright)
 * License:     $(LINK2 http://www.boost.org/LICENSE_1_0.txt, Boost License 1.0)
 * Source:      $(LINK2 https://github.com/dlang/dmd/blob/master/src/dmd/globaltypes.d, _globaltypes.d)
 * Documentation:  https://dlang.org/phobos/dmd_globaltypes.html
 * Coverage:    https://codecov.io/gh/dlang/dmd/src/master/src/dmd/globaltypes.d
 */

module dmd.globaltypes;

import core.stdc.stdint;

/// Bit decoding of the TargetOS
enum TargetOS : ubyte
{
    /* These are mutually exclusive; one and only one is set.
     * Match spelling and casing of corresponding version identifiers
     */
    linux        = 1,
    Windows      = 2,
    OSX          = 4,
    OpenBSD      = 8,
    FreeBSD      = 0x10,
    Solaris      = 0x20,
    DragonFlyBSD = 0x40,

    // Combination masks
    all = linux | Windows | OSX | OpenBSD | FreeBSD | Solaris | DragonFlyBSD,
    Posix = linux | OSX | OpenBSD | FreeBSD | Solaris | DragonFlyBSD,
}

template xversion(string s)
{
    enum xversion = mixin(`{ version (` ~ s ~ `) return true; else return false; }`)();
}

enum TARGET : bool
{
    Linux        = xversion!`linux`,
    OSX          = xversion!`OSX`,
    FreeBSD      = xversion!`FreeBSD`,
    OpenBSD      = xversion!`OpenBSD`,
    Solaris      = xversion!`Solaris`,
    Windows      = xversion!`Windows`,
    DragonFlyBSD = xversion!`DragonFlyBSD`,
}

enum DiagnosticReporting : ubyte
{
    error,        // generate an error
    inform,       // generate a warning
    off,          // disable diagnostic
}

enum MessageStyle : ubyte
{
    digitalmars,  // filename.d(line): message
    gnu,          // filename.d:line: message, see https://www.gnu.org/prep/standards/html_node/Errors.html
}

enum CHECKENABLE : ubyte
{
    _default,     // initial value
    off,          // never do checking
    on,           // always do checking
    safeonly,     // do checking only in @safe functions
}

enum CHECKACTION : ubyte
{
    D,            // call D assert on failure
    C,            // call C assert on failure
    halt,         // cause program halt on failure
    context,      // call D assert with the error context on failure
}

enum PIC : ubyte
{
    fixed,              /// located at a specific address
    pic,                /// Position Independent Code
    pie,                /// Position Independent Executable
}

/**
Each flag represents a field that can be included in the JSON output.

NOTE: set type to uint so its size matches C++ unsigned type
*/
enum JsonFieldFlags : uint
{
    none         = 0,
    compilerInfo = (1 << 0),
    buildInfo    = (1 << 1),
    modules      = (1 << 2),
    semantics    = (1 << 3),
}

enum CppStdRevision : uint
{
    cpp98 = 1997_11,
    cpp11 = 2011_03,
    cpp14 = 2014_02,
    cpp17 = 2017_03,
    cpp20 = 2020_02,
}

/// Configuration for the C++ header generator
enum CxxHeaderMode : uint
{
    none,   /// Don't generate headers
    silent, /// Generate headers
    verbose /// Generate headers and add comments for hidden declarations
}

/// Trivalent boolean to represent the state of a `revert`able change
enum FeatureState : byte
{
    default_ = -1, /// Not specified by the user
    disabled = 0,  /// Specified as `-revert=`
    enabled = 1    /// Specified as `-preview=`
}

alias structalign_t = uint;

// magic value means "match whatever the underlying C compiler does"
// other values are all powers of 2
enum STRUCTALIGN_DEFAULT = (cast(structalign_t)~0);

// Because int64_t and friends may be any integral type of the
// correct size, we have to explicitly ask for the correct
// integer type to get the correct mangling with dmd

// Be careful not to care about sign when using dinteger_t
// use this instead of integer_t to
// avoid conflicts with system #include's
alias dinteger_t = ulong;
// Signed and unsigned variants
alias sinteger_t = long;
alias uinteger_t = ulong;

alias d_int8 = int8_t;
alias d_uns8 = uint8_t;
alias d_int16 = int16_t;
alias d_uns16 = uint16_t;
alias d_int32 = int32_t;
alias d_uns32 = uint32_t;
alias d_int64 = int64_t;
alias d_uns64 = uint64_t;

enum LINK : ubyte
{
    default_,
    d,
    c,
    cpp,
    windows,
    objc,
    system,
}

enum CPPMANGLE : ubyte
{
    def,
    asStruct,
    asClass,
}

enum MATCH : int
{
    nomatch,   // no match
    convert,   // match with conversions
    constant,  // match with conversion to const
    exact,     // exact match
}

enum PINLINE : ubyte
{
    default_,     // as specified on the command line
    never,   // never inline
    always,  // always inline
}

alias StorageClass = uinteger_t;


/* Compiler implementation of the D programming language
 * Copyright (C) 1999-2021 by The D Language Foundation, All Rights Reserved
 * written by Walter Bright
 * http://www.digitalmars.com
 * Distributed under the Boost Software License, Version 1.0.
 * http://www.boost.org/LICENSE_1_0.txt
 * https://github.com/dlang/dmd/blob/master/src/dmd/globaltypes.h
 */

#pragma once

#include "root/dsystem.h"

typedef unsigned char TargetOS;
enum
{
    /* These are mutually exclusive; one and only one is set.
     * Match spelling and casing of corresponding version identifiers
     */
    TargetOS_linux        = 1,
    TargetOS_Windows      = 2,
    TargetOS_OSX          = 4,
    TargetOS_OpenBSD      = 8,
    TargetOS_FreeBSD      = 0x10,
    TargetOS_Solaris      = 0x20,
    TargetOS_DragonFlyBSD = 0x40,

    // Combination masks
    all = TargetOS_linux | TargetOS_Windows | TargetOS_OSX | TargetOS_OpenBSD | TargetOS_FreeBSD | TargetOS_Solaris | TargetOS_DragonFlyBSD,
    Posix = TargetOS_linux | TargetOS_OSX | TargetOS_OpenBSD | TargetOS_FreeBSD | TargetOS_Solaris | TargetOS_DragonFlyBSD,
};


typedef unsigned char Diagnostic;
enum
{
    DIAGNOSTICerror,  // generate an error
    DIAGNOSTICinform, // generate a warning
    DIAGNOSTICoff     // disable diagnostic
};

typedef unsigned char MessageStyle;
enum
{
    MESSAGESTYLEdigitalmars, // file(line,column): message
    MESSAGESTYLEgnu          // file:line:column: message
};

// The state of array bounds checking
typedef unsigned char CHECKENABLE;
enum
{
    CHECKENABLEdefault, // initial value
    CHECKENABLEoff,     // never do bounds checking
    CHECKENABLEon,      // always do bounds checking
    CHECKENABLEsafeonly // do bounds checking only in @safe functions
};

typedef unsigned char CHECKACTION;
enum
{
    CHECKACTION_D,        // call D assert on failure
    CHECKACTION_C,        // call C assert on failure
    CHECKACTION_halt,     // cause program halt on failure
    CHECKACTION_context   // call D assert with the error context on failure
};

enum JsonFieldFlags
{
    none         = 0,
    compilerInfo = (1 << 0),
    buildInfo    = (1 << 1),
    modules      = (1 << 2),
    semantics    = (1 << 3)
};

enum CppStdRevision
{
    CppStdRevisionCpp98 = 199711,
    CppStdRevisionCpp11 = 201103,
    CppStdRevisionCpp14 = 201402,
    CppStdRevisionCpp17 = 201703,
    CppStdRevisionCpp20 = 202002
};

/// Configuration for the C++ header generator
enum class CxxHeaderMode
{
    none,   /// Don't generate headers
    silent, /// Generate headers
    verbose /// Generate headers and add comments for hidden declarations
};

/// Trivalent boolean to represent the state of a `revert`able change
enum class FeatureState : signed char
{
    default_ = -1, /// Not specified by the user
    disabled = 0,  /// Specified as `-revert=`
    enabled = 1    /// Specified as `-preview=`
};

typedef unsigned structalign_t;
// magic value means "match whatever the underlying C compiler does"
// other values are all powers of 2
#define STRUCTALIGN_DEFAULT ((structalign_t) ~0)

// Because int64_t and friends may be any integral type of the correct size,
// we have to explicitly ask for the correct integer type to get the correct
// mangling with dmd. The #if logic here should match the mangling of
// Tint64 and Tuns64 in cppmangle.d.
#if MARS && DMD_VERSION >= 2079 && DMD_VERSION <= 2081 && \
    __APPLE__ && __SIZEOF_LONG__ == 8
// DMD versions between 2.079 and 2.081 mapped D long to int64_t on OS X.
typedef uint64_t dinteger_t;
typedef int64_t sinteger_t;
typedef uint64_t uinteger_t;
#elif __SIZEOF_LONG__ == 8
// Be careful not to care about sign when using dinteger_t
// use this instead of integer_t to
// avoid conflicts with system #include's
typedef unsigned long dinteger_t;
// Signed and unsigned variants
typedef long sinteger_t;
typedef unsigned long uinteger_t;
#else
typedef unsigned long long dinteger_t;
typedef long long sinteger_t;
typedef unsigned long long uinteger_t;
#endif

typedef int8_t                  d_int8;
typedef uint8_t                 d_uns8;
typedef int16_t                 d_int16;
typedef uint16_t                d_uns16;
typedef int32_t                 d_int32;
typedef uint32_t                d_uns32;
typedef int64_t                 d_int64;
typedef uint64_t                d_uns64;

enum class LINK : uint8_t
{
    default_,
    d,
    c,
    cpp,
    windows,
    objc,
    system
};

enum class CPPMANGLE : uint8_t
{
    def,
    asStruct,
    asClass
};

enum MATCH
{
    MATCHnomatch,       // no match
    MATCHconvert,       // match with conversions
    MATCHconst,         // match with conversion to const
    MATCHexact          // exact match
};

enum class PINLINE : uint8_t
{
    default_,     // as specified on the command line
    never,        // never inline
    always        // always inline
};

typedef uinteger_t StorageClass;

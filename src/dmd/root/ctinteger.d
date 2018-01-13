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

// Types used by the front-end for compile-time integers

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

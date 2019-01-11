// EXTRA_SOURCES: imports/argufile.d
/*
TEST_OUTPUT:
---
runnable/imports/argufile.d(76): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(76): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(81): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(81): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(100): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(105): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(109): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(112): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(117): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(141): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
runnable/imports/argufile.d(142): Deprecation: enum `std.format.Mangle` is deprecated - Use std.demangle
---
*/

// NOTE: The bug only works when main.d and argufile.d are put in
//                      separate files and compiled like 'dmd main.d argufile.d'
//                      Also, I'm sure writefln is causing the crash cause when I
//                      use printf(), it doesn't crash.

// main.d -------------------------------------------------------

import argufile;

int main(string[] args)
{
        string message = arguments("bob is ", 7, " years old");

        writefln(message);

        argufile.useargs(); // will crash here

        return 0;
}


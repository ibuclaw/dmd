// https://issues.dlang.org/show_bug.cgi?id=12527
/*
TEST_OUTPUT:
---
void function() @safe
void delegate() @safe
---
*/

@system:
    alias Fun = void function() @safe;
    static assert(Fun.stringof == "void function() @safe");
    alias Del = void delegate() @safe;
    static assert(Del.stringof == "void delegate() @safe");


/**
 * Stores command line options and contains other miscellaneous declarations.
 *
 * Copyright:   Copyright (C) 1999-2021 by The D Language Foundation, All Rights Reserved
 * Authors:     $(LINK2 http://www.digitalmars.com, Walter Bright)
 * License:     $(LINK2 http://www.boost.org/LICENSE_1_0.txt, Boost License 1.0)
 * Source:      $(LINK2 https://github.com/dlang/dmd/blob/master/src/dmd/globals.d, _globals.d)
 * Documentation:  https://dlang.org/phobos/dmd_globals.html
 * Coverage:    https://codecov.io/gh/dlang/dmd/src/master/src/dmd/globals.d
 */

module dmd.globals;

import core.stdc.stdint;
import dmd.root.array;
import dmd.root.filename;
import dmd.root.outbuffer;
import dmd.identifier;

public import dmd.globaltypes;

// Put command line switches in here
extern (C++) struct Param
{
    bool obj = true;        // write object file
    bool link = true;       // perform link
    bool dll;               // generate shared dynamic library
    bool lib;               // write library file instead of object file(s)
    bool multiobj;          // break one object file into multiple ones
    bool oneobj;            // write one object file instead of multiple ones
    bool trace;             // insert profiling hooks
    bool tracegc;           // instrument calls to 'new'
    bool verbose;           // verbose compile
    bool vcg_ast;           // write-out codegen-ast
    bool showColumns;       // print character (column) numbers in diagnostics
    bool vtls;              // identify thread local variables
    bool vtemplates;        // collect and list statistics on template instantiations
    bool vtemplatesListInstances; // collect and list statistics on template instantiations origins. TODO: make this an enum when we want to list other kinds of instances
    bool vgc;               // identify gc usage
    bool vfield;            // identify non-mutable field variables
    bool vcomplex;          // identify complex/imaginary type usage
    ubyte symdebug;         // insert debug symbolic information
    bool symdebugref;       // insert debug information for all referenced types, too
    bool optimize;          // run optimizer
    bool is64bit = (size_t.sizeof == 8);  // generate 64 bit code; true by default for 64 bit dmd
    bool isLP64;            // generate code for LP64
    TargetOS targetOS;      // operating system to generate code for
    bool hasObjectiveC;     // target supports Objective-C
    bool mscoff = false;    // for Win32: write MsCoff object files instead of OMF
    DiagnosticReporting useDeprecated = DiagnosticReporting.inform;  // how use of deprecated features are handled
    bool stackstomp;            // add stack stomping code
    bool useUnitTests;          // generate unittest code
    bool useInline = false;     // inline expand functions
    FeatureState useDIP25;  // implement http://wiki.dlang.org/DIP25
    bool useDIP1021;        // implement https://github.com/dlang/DIPs/blob/master/DIPs/DIP1021.md
    bool release;           // build release version
    bool preservePaths;     // true means don't strip path from source file
    DiagnosticReporting warnings = DiagnosticReporting.off;  // how compiler warnings are handled
    PIC pic = PIC.fixed;    // generate fixed, pic or pie code
    bool color;             // use ANSI colors in console output
    bool cov;               // generate code coverage data
    ubyte covPercent;       // 0..100 code coverage percentage required
    bool ctfe_cov = false;  // generate coverage data for ctfe
    bool nofloat;           // code should not pull in floating point support
    bool ignoreUnsupportedPragmas;  // rather than error on them
    bool useModuleInfo = true;   // generate runtime module information
    bool useTypeInfo = true;     // generate runtime type information
    bool useExceptions = true;   // support exception handling
    bool noSharedAccess;         // read/write access to shared memory objects
    bool previewIn;         // `in` means `[ref] scope const`, accepts rvalues
    bool shortenedMethods; // allow => in normal function declarations
    bool betterC;           // be a "better C" compiler; no dependency on D runtime
    bool addMain;           // add a default main() function
    bool allInst;           // generate code for all template instantiations
    bool fix16997;          // fix integral promotions for unary + - ~ operators
                            // https://issues.dlang.org/show_bug.cgi?id=16997
    bool fixAliasThis;      // if the current scope has an alias this, check it before searching upper scopes
    bool inclusiveInContracts;   // 'in' contracts of overridden methods must be a superset of parent contract
    /** The --transition=safe switch should only be used to show code with
     * silent semantics changes related to @safe improvements.  It should not be
     * used to hide a feature that will have to go through deprecate-then-error
     * before becoming default.
     */
    bool vsafe;             // use enhanced @safe checking
    bool ehnogc;            // use @nogc exception handling
    FeatureState dtorFields; // destruct fields of partially constructed objects
                            // https://issues.dlang.org/show_bug.cgi?id=14246
    bool fieldwise;         // do struct equality testing field-wise rather than by memcmp()
    bool rvalueRefParam;    // allow rvalues to be arguments to ref parameters
                            // http://dconf.org/2019/talks/alexandrescu.html
                            // https://gist.github.com/andralex/e5405a5d773f07f73196c05f8339435a
                            // https://digitalmars.com/d/archives/digitalmars/D/Binding_rvalues_to_ref_parameters_redux_325087.html
                            // Implementation: https://github.com/dlang/dmd/pull/9817

    CppStdRevision cplusplus = CppStdRevision.cpp11;    // version of C++ standard to support

    bool markdown = true;   // enable Markdown replacements in Ddoc
    bool vmarkdown;         // list instances of Markdown replacements in Ddoc

    bool showGaggedErrors;  // print gagged errors anyway
    bool printErrorContext;  // print errors with the error context (the error line in the source file)
    bool manual;            // open browser on compiler manual
    bool usage;             // print usage and exit
    bool mcpuUsage;         // print help on -mcpu switch
    bool transitionUsage;   // print help on -transition switch
    bool checkUsage;        // print help on -check switch
    bool checkActionUsage;  // print help on -checkaction switch
    bool revertUsage;       // print help on -revert switch
    bool previewUsage;      // print help on -preview switch
    bool externStdUsage;    // print help on -extern-std switch
    bool hcUsage;           // print help on -HC switch
    bool logo;              // print compiler logo

    CHECKENABLE useInvariants  = CHECKENABLE._default;  // generate class invariant checks
    CHECKENABLE useIn          = CHECKENABLE._default;  // generate precondition checks
    CHECKENABLE useOut         = CHECKENABLE._default;  // generate postcondition checks
    CHECKENABLE useArrayBounds = CHECKENABLE._default;  // when to generate code for array bounds checks
    CHECKENABLE useAssert      = CHECKENABLE._default;  // when to generate code for assert()'s
    CHECKENABLE useSwitchError = CHECKENABLE._default;  // check for switches without a default
    CHECKENABLE boundscheck    = CHECKENABLE._default;  // state of -boundscheck switch

    CHECKACTION checkAction = CHECKACTION.D; // action to take when bounds, asserts or switch defaults are violated

    uint errorLimit = 20;

    const(char)[] argv0;                // program name
    Array!(const(char)*) modFileAliasStrings; // array of char*'s of -I module filename alias strings
    Array!(const(char)*)* imppath;      // array of char*'s of where to look for import modules
    Array!(const(char)*)* fileImppath;  // array of char*'s of where to look for file import modules
    const(char)[] objdir;                // .obj/.lib file output directory
    const(char)[] objname;               // .obj file output name
    const(char)[] libname;               // .lib file output name

    bool doDocComments;                 // process embedded documentation comments
    const(char)[] docdir;               // write documentation file to docdir directory
    const(char)[] docname;              // write documentation file to docname
    Array!(const(char)*) ddocfiles;     // macro include files for Ddoc

    bool doHdrGeneration;               // process embedded documentation comments
    const(char)[] hdrdir;                // write 'header' file to docdir directory
    const(char)[] hdrname;               // write 'header' file to docname
    bool hdrStripPlainFunctions = true; // strip the bodies of plain (non-template) functions

    CxxHeaderMode doCxxHdrGeneration;      /// Generate 'Cxx header' file
    const(char)[] cxxhdrdir;            // write 'header' file to docdir directory
    const(char)[] cxxhdrname;           // write 'header' file to docname

    bool doJsonGeneration;              // write JSON file
    const(char)[] jsonfilename;          // write JSON file to jsonfilename
    JsonFieldFlags jsonFieldFlags;      // JSON field flags to include

    OutBuffer* mixinOut;                // write expanded mixins for debugging
    const(char)* mixinFile;             // .mixin file output name
    int mixinLines;                     // Number of lines in writeMixins

    uint debuglevel;                    // debug level
    Array!(const(char)*)* debugids;     // debug identifiers

    uint versionlevel;                  // version level
    Array!(const(char)*)* versionids;   // version identifiers

    const(char)[] defaultlibname;        // default library for non-debug builds
    const(char)[] debuglibname;          // default library for debug builds
    const(char)[] mscrtlib;              // MS C runtime library

    const(char)[] moduleDepsFile;        // filename for deps output
    OutBuffer* moduleDeps;              // contents to be written to deps file

    bool emitMakeDeps;                   // whether to emit makedeps
    const(char)[] makeDepsFile;          // filename for makedeps output
    Array!(const(char)*) makeDeps;      // dependencies for makedeps

    MessageStyle messageStyle = MessageStyle.digitalmars; // style of file/line annotations on messages

    bool run; // run resulting executable
    Strings runargs; // arguments for executable

    // Linker stuff
    Array!(const(char)*) objfiles;
    Array!(const(char)*) linkswitches;
    Array!bool linkswitchIsForCC;
    Array!(const(char)*) libfiles;
    Array!(const(char)*) dllfiles;
    const(char)[] deffile;
    const(char)[] resfile;
    const(char)[] exefile;
    const(char)[] mapfile;
}

extern (C++) struct Global
{
    const(char)[] inifilename;
    string mars_ext = "d";
    string doc_ext = "html";      // for Ddoc generated files
    string ddoc_ext = "ddoc";     // for Ddoc macro include files
    string hdr_ext = "di";        // for D 'header' import files
    string cxxhdr_ext = "h";      // for C/C++ 'header' files
    string json_ext = "json";     // for JSON files
    string map_ext = "map";       // for .map files

    string copyright = "Copyright (C) 1999-2021 by The D Language Foundation, All Rights Reserved";
    string written = "written by Walter Bright";

    Array!(const(char)*)* path;         // Array of char*'s which form the import lookup path
    Array!(const(char)*)* filePath;     // Array of char*'s which form the file import lookup path

    private enum string _version = import("VERSION");
    private enum uint _versionNumber = parseVersionNumber(_version);

    const(char)[] vendor;    // Compiler backend name

    Param params;
    uint errors;            // number of errors reported so far
    uint warnings;          // number of warnings reported so far
    uint gag;               // !=0 means gag reporting of errors & warnings
    uint gaggedErrors;      // number of errors reported while gagged
    uint gaggedWarnings;    // number of warnings reported while gagged

    void* console;         // opaque pointer to console for controlling text attributes

    Array!Identifier* versionids;    // command line versions and predefined versions
    Array!Identifier* debugids;      // command line debug versions and predefined versions

    enum recursionLimit = 500; // number of recursive template expansions before abort

  nothrow:

    /* Start gagging. Return the current number of gagged errors
     */
    extern (C++) uint startGagging()
    {
        ++gag;
        gaggedWarnings = 0;
        return gaggedErrors;
    }

    /* End gagging, restoring the old gagged state.
     * Return true if errors occurred while gagged.
     */
    extern (C++) bool endGagging(uint oldGagged)
    {
        bool anyErrs = (gaggedErrors != oldGagged);
        --gag;
        // Restore the original state of gagged errors; set total errors
        // to be original errors + new ungagged errors.
        errors -= (gaggedErrors - oldGagged);
        gaggedErrors = oldGagged;
        return anyErrs;
    }

    /*  Increment the error count to record that an error
     *  has occurred in the current context. An error message
     *  may or may not have been printed.
     */
    extern (C++) void increaseErrorCount()
    {
        if (gag)
            ++gaggedErrors;
        ++errors;
    }

    extern (C++) void _init()
    {
        version (MARS)
        {
            vendor = "Digital Mars D";
            static if (TARGET.Windows)
            {
                params.mscoff = params.is64bit;
            }

            // -color=auto is the default value
            import dmd.console : Console;
            params.color = Console.detectTerminal();
        }
        else version (IN_GCC)
        {
            vendor = "GNU D";
        }
    }

    /**
     * Deinitializes the global state of the compiler.
     *
     * This can be used to restore the state set by `_init` to its original
     * state.
     */
    extern (D) void deinitialize()
    {
        this = this.init;
    }

    /**
     * Computes the version number __VERSION__ from the compiler version string.
     */
    extern (D) private static uint parseVersionNumber(string version_)
    {
        //
        // parse _version
        //
        uint major = 0;
        uint minor = 0;
        bool point = false;
        // skip initial 'v'
        foreach (const c; version_[1..$])
        {
            if ('0' <= c && c <= '9') // isdigit
            {
                minor = minor * 10 + c - '0';
            }
            else if (c == '.')
            {
                if (point)
                    break; // ignore everything after second '.'
                point = true;
                major = minor;
                minor = 0;
            }
            else
                break;
        }
        return major * 1000 + minor;
    }

    /**
    Returns: the version as the number that would be returned for __VERSION__
    */
    extern(C++) uint versionNumber()
    {
        return _versionNumber;
    }

    /**
    Returns: compiler version string.
    */
    extern(D) string versionString()
    {
        return _version;
    }

    /**
    Returns: compiler version as char string.
    */
    extern(C++) const(char*) versionChars()
    {
        return _version.ptr;
    }

    /**
    Returns: the final defaultlibname based on the command-line parameters
    */
    extern (D) const(char)[] finalDefaultlibname() const
    {
        return params.betterC ? null :
            params.symdebug ? params.debuglibname : params.defaultlibname;
    }
}

version (DMDLIB)
{
    version = LocOffset;
}

// file location
struct Loc
{
    const(char)* filename; // either absolute or relative to cwd
    uint linnum;
    uint charnum;
    version (LocOffset)
        uint fileOffset;

    static immutable Loc initial;       /// use for default initialization of const ref Loc's

nothrow:
    extern (D) this(const(char)* filename, uint linnum, uint charnum) pure
    {
        this.linnum = linnum;
        this.charnum = charnum;
        this.filename = filename;
    }

    extern (C++) const(char)* toChars(
        bool showColumns = global.params.showColumns,
        ubyte messageStyle = global.params.messageStyle) const pure nothrow
    {
        OutBuffer buf;
        if (filename)
        {
            buf.writestring(filename);
        }
        if (linnum)
        {
            final switch (messageStyle)
            {
                case MessageStyle.digitalmars:
                    buf.writeByte('(');
                    buf.print(linnum);
                    if (showColumns && charnum)
                    {
                        buf.writeByte(',');
                        buf.print(charnum);
                    }
                    buf.writeByte(')');
                    break;
                case MessageStyle.gnu: // https://www.gnu.org/prep/standards/html_node/Errors.html
                    buf.writeByte(':');
                    buf.print(linnum);
                    if (showColumns && charnum)
                    {
                        buf.writeByte(':');
                        buf.print(charnum);
                    }
                    break;
            }
        }
        return buf.extractChars();
    }

    /* Checks for equivalence,
     * a) comparing the filename contents (not the pointer), case-
     *    insensitively on Windows, and
     * b) ignoring charnum if `global.params.showColumns` is false.
     */
    extern (C++) bool equals(ref const(Loc) loc) const
    {
        return (!global.params.showColumns || charnum == loc.charnum) &&
               linnum == loc.linnum &&
               FileName.equals(filename, loc.filename);
    }

    /* opEquals() / toHash() for AA key usage:
     *
     * Compare filename contents (case-sensitively on Windows too), not
     * the pointer - a static foreach loop repeatedly mixing in a mixin
     * may lead to multiple equivalent filenames (`foo.d-mixin-<line>`),
     * e.g., for test/runnable/test18880.d.
     */
    extern (D) bool opEquals(ref const(Loc) loc) const @trusted pure nothrow @nogc
    {
        import core.stdc.string : strcmp;

        return charnum == loc.charnum &&
               linnum == loc.linnum &&
               (filename == loc.filename ||
                (filename && loc.filename && strcmp(filename, loc.filename) == 0));
    }

    extern (D) size_t toHash() const @trusted pure nothrow
    {
        import dmd.root.string : toDString;

        auto hash = hashOf(linnum);
        hash = hashOf(charnum, hash);
        hash = hashOf(filename.toDString, hash);
        return hash;
    }

    /******************
     * Returns:
     *   true if Loc has been set to other than the default initialization
     */
    bool isValid() const pure
    {
        return filename !is null;
    }
}

extern (C++) __gshared Global global;

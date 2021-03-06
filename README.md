# Build

Xagasoft Build is a comprehensive build tool for nearly any multi-step
automation process, but is intended primarily for compiling source code.

Features:

 * Does _NOT_ rely on make of any sort.
 * Can auto-generate targets based on builtin and user provided rules.
 * Contains it's own Turing complete scripting language.
 * Provides a variety of output modes to make errors easier to see.
 * Supports plugins.
 * Write much, much less and do more.
 * Builtin understanding of dependency tracking.
 	* This feature is currently removed, as I am working on Windows compatibility. It'll be re-introduced later
 	* Well, it is still present, but not used in any compilation rules.

## Requirements

There are different requirements depending on how you got your source code and
what you intend to do with it.  If you just want to use build, then I recommend
getting one of the release tarballs.  They include most of the dependencies and
are the easiest to use.

### Tarball Releases

To compile a tarball release you need:

 * bash (or similar)
 * C++ compiler (gcc is the primary development target)
 * libc with pthreads and libdl support (glibc and most compatibles)

### SCM Checkouts

If you're getting the code from source control or want to do more there are
these additional requirements:

 * flex
 * bison

The flex and bison source code are pre-processed into C source code for the
tarballs, so you do not need these programs in order to compile.

Build also relies on libbu++, but the shell script 'build.sh' will check out
the files it needs from libbu++ SVN.  These files are also included in the
tarball release.

Build also builds the easiest when using build.  Once you have a working
version of build it's very easy to keep working on build.

### Building form source

The minimal requirement is Flex and Bison. As I have worked on the fork, I have not found a clever way to dynamicaly download the dependency files using Windows' shell. On linux, you may have CURL or Wget available. 
For now, my fork will further include the `minibu/` folder. Now, you just have to run `./speedbuild.sh` to build yourself a copy. But its recommended to use `build.sh`. The speedbuild is only good if you wish to understand how this tool is built on the simplest of all ways. After having used either of these scripts, you may want to properly build the tool using itself. To do so, follow these steps:


    cp build build_speed
    find . -name "*.o" -delete
    ./build_speed

Doing that, you will get a new build executable, but properly linked, configured and compiled. You can delete `build_speed` now.

# Example Build Scripts

Just to give you a taste, here are some real, simple build scripts.

    target "filescan"
    {
        rule "exe";
        input files("src/*.cpp");
        CXXFLAGS += "-ggdb";
        LDFLAGS += "-lbu++";
    }

Build has sensible defaults for many things, this creates an explicit target
named filescan that relies on the source code in the directory src.  It adds
debugging flags, and it links libbu++.

The build system uses a variation on a standard boolean transitive closure
algorithm to provide "path-finding" between the .cpp files and the inputs the
"exe" rule needs to create output.  In this case it will automatically create
targets needed to produce all of the object code.

    CXXFLAGS += "-ggdb -I. -Ilibgats";
    LDFLAGS += "-ggdb";

    target "libjove.a"
    {
        rule "lib";
        input files("src/libjove/*.cpp");
    }

    target "joved"
    {
        rule "exe";
        input files("src/joved/*.cpp");
        requires "libjove.a";

        LDFLAGS += "-L. -ljove -lbu++ -Llibgats -lgats -lcryptopp -lpthread";
    }

This example is slightly more complex.  It sets some flags that all targets will
use, then creates two explicit targets.  The second target, joved, also
requires that libjove.a is up to date, but it is not treated as an input.  This
is enough to determine order of building, all source files, targets, and even
provides full dependency tracking.


# Windows compatibility

As I am evolving this fork, I am also working on further improving the compatibility. In my current version, I am supporting a few of `cl.exe`'s command switches, as well as the rules. My goal is to be fully windows-compatible, to build projects of any scale on any platform.

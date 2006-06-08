// Reference Buildfile by Mike Buland
// Comments can be C++ style or bash style right now, no C style /* */

// Recognized file types will be built using the default programs on each
// system, each will get a target and be built with as much dependency tracking
// as possible.

// This is the default target, build with no params will always try to build
// the all target, unless your Buildfile specifies something a little more
// interesting.  Specifying phony as a parameter means that this target builds
// every call no matter what.
target( all, phony )
{
	build( build );
}

target( build )
{
	type = executable;
	linker = c++;
	output = build;
	
	if( isDebug() == true )
	{
		addParam( CXXFLAGS, "-ggdb")
		addParam( CFLAGS, "-ggdb")
	}
	
	usesDir( src );
}


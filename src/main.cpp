#include "builder.h"
#include "viewerplain.h"
#include "viewerpercent.h"

int main( int argc, char *argv[] )
{
	ViewerPercent p;
	Builder bld( p );

	bld.load("build.conf");

	if( argc >= 2 )
		bld.build( argv[1] );
	else
		bld.build();
/*
	printf("\n\n----------\nDebug dump\n----------\n");
	bld.debug();*/
}


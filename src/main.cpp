#include "builder.h"

int main()
{
	Builder bld;

	bld.load("build.conf");

	bld.build();
/*
	printf("\n\n----------\nDebug dump\n----------\n");
	bld.debug();*/
}


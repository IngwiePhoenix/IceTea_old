#include "builder.h"

int main()
{
	Builder bld;

	bld.load("congo");

	printf("\n\n----------\nDebug dump\n----------\n");

	bld.debug();
}


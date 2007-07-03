#include "viewermake.h"
#include "perform.h"
#include "bu/plugger.h"

PluginInterface2( make, ViewerMake, Viewer, "Mike Buland", 0, 1 );

ViewerMake::ViewerMake()
{
}

ViewerMake::~ViewerMake()
{
}

void ViewerMake::executeCmd( const std::string &sCmd )
{
	printf("%s\n", sCmd.c_str() );
}


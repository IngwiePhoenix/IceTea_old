#include "performcommand.h"
#include "bu/plugger.h"
#include "build.h"
#include "viewer.h"

#include <stdlib.h>

PluginInterface2(command, PerformCommand, Perform, "Mike Buland", 0, 1 );

PerformCommand::PerformCommand()
{
}

PerformCommand::~PerformCommand()
{
}

Perform *PerformCommand::duplicate( Build &bld, const StringList *cont, VarMap *mExtra )
{
	Perform *pRet = new PerformCommand();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}

void PerformCommand::execute( Build &bld )
{
	bld.getView()->executeCmd( lParam.front() );
	int n = system( lParam.front().c_str() );
	if( n != 0 )
		throw BuildException(
			"Command exited with error code %d.",
			WEXITSTATUS(n)
			);
}


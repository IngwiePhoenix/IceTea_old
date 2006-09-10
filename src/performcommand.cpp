#include "performcommand.h"
#include "plugger.h"

PluginInterface2(command, PerformCommand, Perform, "Mike Buland", 0, 1 );

PerformCommand::PerformCommand()
{
}

PerformCommand::~PerformCommand()
{
}

Perform *PerformCommand::duplicate( Build &bld, const std::string &cont )
{
	Perform *pRet = new PerformCommand();
	pRet->copyData( this, bld, cont, NULL );
	return pRet;
}


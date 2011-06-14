#include "functionclose.h"
#include "filemgr.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionClose, close, FunctionClose, Function,
		"Mike Buland", 0, 1 );

FunctionClose::FunctionClose()
{
}

FunctionClose::~FunctionClose()
{
}

Bu::String FunctionClose::getName() const
{
	return "close";
}

Variable FunctionClose::call( Variable &input, VarList )
{
	FileMgr::getInstance().close( (ptrdiff_t)input.getOpaque() );
	return Variable();
}


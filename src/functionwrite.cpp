#include "functionwrite.h"
#include "filemgr.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionWrite, write, FunctionWrite, Function,
		"Mike Buland", 0, 1 );

FunctionWrite::FunctionWrite()
{
}

FunctionWrite::~FunctionWrite()
{
}

Bu::String FunctionWrite::getName() const
{
	return "write";
}

Variable FunctionWrite::call( Variable &input, VarList lParams )
{
	if( lParams.getSize() != 1 )
	{
		throw Bu::ExceptionBase(
			"write takes one parameter, the string to write."
			);
	}
	FileMgr::getInstance().get( (int)input.getOpaque() ).write(
		lParams.first().toString()
		);
	return Variable();
}


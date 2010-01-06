#include "functionfilename.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionFileName, fileName, FunctionFileName, Function,
		"Mike Buland", 0, 1 );

FunctionFileName::FunctionFileName()
{
}

FunctionFileName::~FunctionFileName()
{
}

Bu::FString FunctionFileName::getName() const
{
	return "fileName";
}

Variable FunctionFileName::call( Variable &input, VarList /*lParams*/ )
{
	Bu::FString sFile;
	sFile = input.getString();

	Bu::FString::const_iterator i = sFile.begin();
	for(;;)
	{
		Bu::FString::const_iterator b = i.find('/');
		if( !b )
		{
			return Variable( Bu::FString( i ) );
		}
		i = b+1;
		if( !i )
		{
			return Variable( Bu::FString( i ) );
		}
	}
}


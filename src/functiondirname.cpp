#include "functiondirname.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionDirName, dirName, FunctionDirName, Function,
		"Mike Buland", 0, 1 );

FunctionDirName::FunctionDirName()
{
}

FunctionDirName::~FunctionDirName()
{
}

Bu::String FunctionDirName::getName() const
{
	return "dirName";
}

Variable FunctionDirName::call( Variable &input, VarList /*lParams*/ )
{
	Bu::String sFile;
	sFile = input.getString();

	Bu::String::const_iterator i = sFile.begin();
	Bu::String::const_iterator io;
	for(;;)
	{
		Bu::String::const_iterator b = i.find('/');
		if( !b )
		{
			return Variable( Bu::String( sFile.begin(), io ) );
		}
		io = b;
		i = b+1;
		if( !i )
		{
			return Variable( Bu::String( sFile.begin(), io ) );
		}
	}
}


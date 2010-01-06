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

Bu::FString FunctionDirName::getName() const
{
	return "dirName";
}

Variable FunctionDirName::call( Variable &input, VarList /*lParams*/ )
{
	Bu::FString sFile;
	sFile = input.getString();

	Bu::FString::const_iterator i = sFile.begin();
	Bu::FString::const_iterator io;
	for(;;)
	{
		Bu::FString::const_iterator b = i.find('/');
		if( !b )
		{
			return Variable( Bu::FString( sFile.begin(), io ) );
		}
		io = b;
		i = b+1;
		if( !i )
		{
			return Variable( Bu::FString( sFile.begin(), io ) );
		}
	}
}


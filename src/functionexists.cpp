#include "functionexists.h"

#include <unistd.h>

FunctionExists::FunctionExists()
{
}

FunctionExists::~FunctionExists()
{
}

Bu::FString FunctionExists::getName() const
{
	return "exists";
}

Variable FunctionExists::call( Variable &input, VarList lParams )
{
	Bu::FString sFile;
	if( input.getType() != Variable::typeNone )
	{
		sFile = input.toString();
	}
	else
	{
		sFile = lParams.first().toString();
	}
	if( access( sFile.getStr(), F_OK ) == 0 )
		return Variable( true );
	else
		return Variable( false );
}


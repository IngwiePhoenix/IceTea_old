#include "functiontostring.h"

#include <stdlib.h>
#include <bu/sio.h>
using namespace Bu;

#include <bu/plugger.h>
PluginInterface3( pluginFunctionToString, toString, FunctionToString, Function,
		"Mike Buland", 0, 1 );

FunctionToString::FunctionToString()
{
}

FunctionToString::~FunctionToString()
{
}

Bu::String FunctionToString::getName() const
{
	return "toString";
}

Variable FunctionToString::call( Variable &input, VarList lParams )
{
	Bu::String sStr;
	Bu::String sSep;
	if( lParams.getSize() == 0 )
	{
		sSep = " ";
	}
	else
	{
		sSep = lParams.first().getString();
	}
	switch( input.getType() )
	{
		case Variable::typeString:
			return input;

		case Variable::typeList:
			for( VarList::iterator i = input.begin(); i; i++ )
			{
				if( i != input.begin() )
					sStr += sSep;
				sStr += (*i).getString();
			}
			return Variable( sStr );

		default:
			return Variable( input.getString() );
	}
}


#include "functionunlink.h"

#include <unistd.h>
#include <stdlib.h>
#include <bu/sio.h>
using namespace Bu;

#include <bu/plugger.h>
PluginInterface3( pluginFunctionUnlink, unlink, FunctionUnlink, Function,
		"Mike Buland", 0, 1 );

FunctionUnlink::FunctionUnlink()
{
}

FunctionUnlink::~FunctionUnlink()
{
}

Bu::String FunctionUnlink::getName() const
{
	return "unlink";
}

Variable FunctionUnlink::call( Variable &/*input*/, VarList lParams )
{
	//sio << "Unlink called: " << lParams << sio.nl;
	for( VarList::iterator p = lParams.begin(); p; p++ )
	{
		switch( (*p).getType() )
		{
			case Variable::typeString:
				//sio << " xx> " << (*p).getString() << sio.nl;
				unlink( (*p).getString().getStr() );
				break;

			case Variable::typeList:
				//sio << " xx>";
				for( VarList::iterator i = (*p).begin(); i; i++ )
				{
					//sio << " " << (*i).getString();
					unlink( (*i).getString().getStr() );
				}
				//sio << sio.nl;
				break;

			default:
				throw Bu::ExceptionBase("Hey, wrong type passed.");
				break;
		}
	}

	return Variable();
}


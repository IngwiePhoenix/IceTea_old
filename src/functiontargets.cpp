#include "functiontargets.h"
#include "context.h"
#include "target.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionTargets, targets, FunctionTargets, Function,
		"Mike Buland", 0, 1 );

FunctionTargets::FunctionTargets()
{
}

FunctionTargets::~FunctionTargets()
{
}

Bu::String FunctionTargets::getName() const
{
	return "targets";
}

Variable FunctionTargets::call( Variable &/*input*/, VarList lParams )
{
	Variable vRet( Variable::typeList );
	TargetList lTrg;
	if( lParams.getSize() == 0 )
	{
		lTrg = pContext->getExplicitTargets();
	}
	else
	{
		lTrg = pContext->getTag( lParams.first().toString() );
	}
	for( TargetList::const_iterator i = lTrg.begin(); i; i++ )
	{
		for( StrList::const_iterator j = (*i)->getOutputList().begin(); j; j++ )
		{
			vRet.append( *j );
		}
	}
	return vRet;
}


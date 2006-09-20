#include "functiontargets.h"
#include "plugger.h"
#include "build.h"

PluginInterface2(targets, FunctionTargets, Function, "Mike Buland", 0, 1 );

FunctionTargets::FunctionTargets()
{
}

FunctionTargets::~FunctionTargets()
{
}

void FunctionTargets::execute( Build *bld, const StringList &lInput, StringList &lOutput )
{
	if( bld == NULL )
	{
		throw BuildException("You cannot call targets() from anywhere, see the manual.");
	}

	for( TargetMap::iterator i = bld->getTargetMap().begin();
		 i != bld->getTargetMap().end(); i++ )
	{
		lOutput.push_back( (*i).first );
	}
}

Function *FunctionTargets::duplicate( Build &bld, const StringList *cont, VarMap *mExtra )
{
	Function *pRet = new FunctionTargets();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}


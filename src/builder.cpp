#include <iostream>

#include "builder.h"
#include "action.h"
#include "command.h"
#include "build.tab.h"

subExceptionDef( BuildException )

Builder::Builder() :
	pDefaultAction( NULL ),
	pLastAddedAction( NULL )
{
}

Builder::~Builder()
{
}

void yyparse( Builder &bld );

void Builder::load( const char *sFN )
{
	file = sFN;

	scanBegin();
	yyparse( *this );
	scanEnd();
}

void Builder::add( Action *pAct )
{
	if( pAct->isDefault() )
	{
		if( pDefaultAction )
			throw BuildException("There's already a default exception");
		pDefaultAction = pAct;
	}
	else
	{
		mAction[pAct->getName()] = pAct;
	}
	pLastAddedAction = pAct;
}

void Builder::add( Command *pCmd )
{
	if( pLastAddedAction )
	{
		pLastAddedAction->add( pCmd );
	}
}

void Builder::debug()
{
	pDefaultAction->debug();
	for( std::map<const char *, Action *, ltstr>::iterator i = mAction.begin();
		 i != mAction.end(); i++ )
	{
		(*i).second->debug();
	}
}


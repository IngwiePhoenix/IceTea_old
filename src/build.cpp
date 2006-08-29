#include "build.h"

subExceptionDef( BuildException );


Build::Build()
{
}

Build::~Build()
{
}

void Build::addTarget( Target *pTarget )
{
	TargetMap::iterator i = mTarget.find( pTarget->getName() );
	if( i == mTarget.end() )
	{
		mTarget[pTarget->getName()] = pTarget;
	}
	else
	{
		throw BuildException("Merging targets isn't working yet.");
	}
}

void Build::addRequires( const std::string &who, const std::string &what )
{
	mRequires[who].push_back( what );
}

void Build::addRule( Rule *pRule )
{
	mRule[pRule->getName()] = pRule;
}

void Build::set( const std::string &cont, const std::string &var, const std::string &val )
{
	if( cont == "" )
	{
		mVars[var] = val;
	}
	else
	{
		mContVars[cont][var] = val;
	}
}

void Build::setAdd( const std::string &cont, const std::string &var, const std::string &val )
{
	if( cont == "" )
	{
		mVars[var] = getVar( cont, var ) + " " + val;
	}
	else
	{
		mContVars[cont][var] = getVar( cont, var ) + " " + val;
	}
}

std::string Build::getVar( const std::string &cont, const std::string &var )
{
	if( cont == "" )
	{
		if( mVars.find(var) == mVars.end() )
		{
			if( getenv( var.c_str() ) == NULL )
			{
				mVars[var] = "";
			}
			else
			{
				mVars[var] = getenv( var.c_str() );
			}
		}
		return mVars[var];
	}
	else
	{
		if( mContVars[cont].find(var) == mContVars[cont].end() )
		{
			mContVars[cont][var] = getVar( "", var );
		}
		return mContVars[cont][var];
	}
}

void Build::debugDump()
{
	printf("Requires:\n");
	for( ReqMap::iterator i = mRequires.begin(); i != mRequires.end(); i++ )
	{
		printf("  %s: ", (*i).first.c_str() );

		for( StringList::iterator j = (*i).second.begin();
			 j != (*i).second.end(); j++ )
		{
			if( j != (*i).second.begin() )
				printf(", ");
			printf("%s", (*j).c_str() );
		}
		printf("\n");
	}

	printf("Targets:\n");
	for( TargetMap::iterator i = mTarget.begin(); i != mTarget.end(); i++ )
	{
		printf("  %s:\n", (*i).first.c_str() );
		printf("    Rule:  %s\n", (*i).second->getRule().c_str() );
		printf("    Input: ");
		for( StringList::iterator j = (*i).second->getInput().begin();
			 j != (*i).second->getInput().end(); j++ )
		{
			if( j != (*i).second->getInput().begin() )
				printf(", ");
			printf("%s", (*j).c_str() );
		}
		printf("\n");
	}

	printf("Global Variables:\n");
	for( VarMap::iterator i = mVars.begin(); i != mVars.end(); i++ )
	{
		printf("  \"%s\" = \"%s\"\n", (*i).first.c_str(), (*i).second.c_str() );
	}

	printf("Context Variables:\n");
	for( ContextMap::iterator i = mContVars.begin(); i != mContVars.end(); i++ )
	{
		printf("  %s:\n", (*i).first.c_str() );
		
		for( VarMap::iterator j = (*i).second.begin();
			 j != (*i).second.end(); j++ )
		{
			printf("    \"%s\" = \"%s\"\n",
				(*j).first.c_str(),
				(*j).second.c_str()
				);
		}
	}

	printf("Rules:\n");
	for( RuleMap::iterator i = mRule.begin(); i != mRule.end(); i++ )
	{
		printf("  %s:\n", (*i).first.c_str() );
		printf("    Matches:  func\n");
		printf("    Filters:  %d\n", (*i).second->getFilterList().size() );
		printf("    Performs: %d\n", (*i).second->getPerformList().size() );
		printf("    Produces:\n");
		printf("    Requires:\n");
	}
}


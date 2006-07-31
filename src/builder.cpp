#include <iostream>

#include "builder.h"
#include "action.h"
#include "command.h"
#include "target.h"
#include "build.tab.h"
#include "rule.h"

subExceptionDef( BuildException )

Builder::Builder() :
	pDefaultAction( NULL ),
	pLastAddedAction( NULL ),
	sTmp(""),
	sContext("")
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

void Builder::add( Rule *pRule )
{
	pLastAddedRule = pRule;
	mRule[pRule->getName()] = pRule;
}

void Builder::add( Target *pTarg )
{
	pLastAddedTarget = pTarg;
	mTarget[pTarg->getName()] = pTarg;
}

void Builder::debug()
{
	printf("Actions:\n");
	pDefaultAction->debug();
	for( std::map<const char *, Action *, ltstr>::iterator i = mAction.begin();
		 i != mAction.end(); i++ )
	{
		(*i).second->debug();
	}

	printf("Targets:\n");
	for( std::map<const char *, Target *, ltstr>::iterator i = mTarget.begin();
		 i != mTarget.end(); i++ )
	{
		(*i).second->debug();
	}

	printf("Rules:\n");
	for( std::map<const char *, Rule *, ltstr>::iterator i = mRule.begin();
		 i != mRule.end(); i++ )
	{
		(*i).second->debug();
	}
	
	printf("Variables:\n");
	for( varmap::iterator i = mVar.begin(); i != mVar.end(); i++ )
	{
		printf("   %s = \"%s\"\n", (*i).first.c_str(), (*i).second.c_str() );
	}

	printf("Variables (by context):\n");
	for( std::map<std::string, varmap>::iterator j = mContVar.begin();
		 j != mContVar.end(); j++ )
	{
		printf("   %s:\n", (*j).first.c_str() );
		for( varmap::iterator i = (*j).second.begin();
			 i != (*j).second.end(); i++ )
		{
			printf("      %s = \"%s\"\n",
					(*i).first.c_str(), (*i).second.c_str() );
		}
	}

	printf("Additional dependancies:\n");
	for( std::map<std::string, std::list<std::string> *>::iterator i =
		 mRequires.begin(); i != mRequires.end(); i++ )
	{
		printf("   %s: ", (*i).first.c_str() );
		std::list<std::string> *pList = (*i).second;
		for( std::list<std::string>::iterator j = pList->begin();
			 j != pList->end(); j++ )
		{
			if( j != pList->begin() )
				printf(", ");
			printf("%s", (*j).c_str() );
		}
		printf("\n");
	}
}

void Builder::checkVar( const char *cont, const char *sName )
{
	if( cont[0] != '\0' )
	{
		varmap &mmVar = mContVar[cont];
		if( mmVar.find( sName ) == mmVar.end() )
		{
			checkVar( "", sName );
			mmVar[sName] = mVar[sName];
		}
	}
	else
	{
		if( mVar.find( sName ) == mVar.end() )
		{
			char *env = getenv( sName );
			if( env )
				mVar[sName] = env;
			else
				mVar[sName] = "";
		}
	}
}

void Builder::varSet( const char *sName, const char *sValue )
{
	checkVar( sContext, sName );

	if( sContext[0] == '\0' )
	{
		mVar[sName] = sValue;
	}
	else
	{
		mContVar[sContext.getString()][sName] = sValue;
	}
}

void Builder::varAddSet( const char *sName, const char *sValue )
{
	checkVar( sContext, sName );

	if( sContext[0] == '\0' )
	{
		std::string s = mVar[sName];
		s += " ";
		s += sValue;
		mVar[sName] = s;
	}
	else
	{
		std::string s = mContVar[sContext.getString()][sName];
		s += " ";
		s += sValue;
		mContVar[sContext.getString()][sName] = s;
	}
}

void Builder::requires( const char *sBase, const char *sReq )
{
	std::list<std::string> *pList = NULL;
	if( mRequires.find(sBase) == mRequires.end() )
	{
		pList = new std::list<std::string>;
		mRequires[sBase] = pList;
	}
	else
	{
		pList = mRequires[sBase];
	}

	pList->push_back( sReq );
}

void Builder::setContext( const char *sCont )
{
	sContext = sCont;
}

void Builder::setContext()
{
	setContext("");
}


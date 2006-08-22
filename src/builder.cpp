#include "builder.h"
#include "functionfactory.h"
#include "performfactory.h"
#include "targetfactory.h"
#include "action.h"

subExceptionDef( BuildException );

Builder::Builder() :
	fFunction( FunctionFactory::getInstance() ),
	fPerform( PerformFactory::getInstance() ),
	fTarget( TargetFactory::getInstance() )
{
}

Builder::~Builder()
{
}

void yyparse( Builder &bld );
extern int yydebug;

void Builder::load( const std::string &sFile )
{
	file = sFile;
	scanBegin();
	//yydebug = 1;
	yyparse( *this );
	scanEnd();
}

void Builder::error( YYLTYPE *locp, const char *msg )
{
	fflush( stdout );
	throw BuildException("%s: %d.%d-%d.%d: %s",
			file.c_str(),
			locp->first_line, locp->first_column,
			locp->last_line, locp->last_column,
			msg );
}

void Builder::error( const std::string &msg )
{
	fflush( stdout );
	throw BuildException("%s", msg.c_str() );
}

//
// Target functions
//
bool Builder::isTarget( const char *sType )
{
	return fTarget.hasPlugin( sType );
}

//
// Function functions
//
bool Builder::isFunction( const char *sFunc )
{
	return fFunction.hasPlugin( sFunc );
}

void Builder::newFunctionCall( const char *sName )
{
	pTmpFunc = fFunction.instantiate( sName );
}

void Builder::addFunctionParam( const char *sParam )
{
	pTmpFunc->addParam( sParam );
}

void Builder::newList()
{
	lTmp.clear();
}

void Builder::addListString( const char *str )
{
	lTmp.push_back( BuildListItem(str, NULL) );
}

void Builder::addListFunc()
{
	lTmp.push_back( BuildListItem("", pTmpFunc ) );
}

StringList Builder::buildToStringList( Builder::BuildList &lSrc, StringList &lIn )
{
	StringList lOut;

	for( BuildList::iterator i = lSrc.begin(); i != lSrc.end(); i++ )
	{
		if( (*i).second )
		{
			(*i).second->execute( lIn, lOut );
		}
		else
		{
			lOut.push_back( (*i).first );
		}
	}

	return lOut;
}

//
// Perform functions
//
bool Builder::isPerform( const char *sPerf )
{
	return fPerform.hasPlugin( sPerf );
}

void Builder::newPerform( const char *sName )
{
}

void Builder::addPerformParam( const char *sParam )
{
}

//
// Functions for dealing with actions
//
void Builder::addAction()
{
	lActions.push_back( ActionTmp("", ActionTmpCmdList()) );
}

void Builder::addAction( const char *sName )
{
	lActions.push_back( ActionTmp(sName, ActionTmpCmdList()) );
}

void Builder::addCommand( int nType )
{
	lActions.back().second.push_back( ActionTmpCmd( nType, lTmp ) );
}

//
// Debug
//
void Builder::debugDump()
{
	printf("Actions:\n");
	for( ActionTmpList::iterator i = lActions.begin();
		 i != lActions.end(); i++ )
	{
		if( (*i).first == "" )
		{
			printf("  default:\n");
		}
		else
		{
			printf("  \"%s\":\n", (*i).first.c_str() );
		}
		for( ActionTmpCmdList::iterator j = (*i).second.begin();
			 j != (*i).second.end(); j++ )
		{
			printf("    %d [", (*j).first );
			for( BuildList::iterator k = (*j).second.begin();
				 k != (*j).second.end(); k++ )
			{
				if( k != (*j).second.begin() )
				{
					printf(", ");
				}
				if( (*k).second )
				{
					printf("func()");
				}
				else
				{
					printf("\"%s\"", (*k).first.c_str() );
				}
			}
			printf("]\n");
		}
	}
}


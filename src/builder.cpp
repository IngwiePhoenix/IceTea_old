#include "builder.h"
#include "functionfactory.h"
#include "performfactory.h"
#include "targetfactory.h"
#include "action.h"
#include "build.h"
#include "rule.h"

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

Build *Builder::load( const std::string &sFile )
{
	file = sFile;
	scanBegin();
	//yydebug = 1;
	yyparse( *this );
	scanEnd();

	return genBuild();
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
	
void Builder::newTarget()
{
	lTargetTmp.push_back( TargetTmp(lTmp, TargetInfo()) );
}

void Builder::setTargetRule( const char *sRule )
{
	lTargetTmp.back().second.sRule = sRule;
}

void Builder::setTargetPrefix( const char *sPrefix )
{
	lTargetTmp.back().second.sPrefix = sPrefix;
}

void Builder::setTargetType( const char *sType )
{
	lTargetTmp.back().second.sType = sType;
}

void Builder::addTargetInput()
{
	lTargetTmp.back().second.lInput.insert(
		lTargetTmp.back().second.lInput.end(),
		lTmp.begin(), lTmp.end()
		);
}

void Builder::addTargetRequires()
{
	lTargetTmp.back().second.lRequires.insert(
		lTargetTmp.back().second.lRequires.end(),
		lTmp.begin(), lTmp.end()
		);
}

void Builder::addTargetSet( const char *sVar, const char *sVal, int nHow )
{
	lTargetTmp.back().second.lVar.push_back( SetVar( sVar, sVal, nHow ) );
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

//
// List functions
//
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

void Builder::filterList()
{
	StringList lTmp2;
	StringList lIn = buildToStringList( lTmp, StringList() );
	pTmpFunc->execute( lIn, lTmp2 );
	lTmp.clear();
	for( StringList::iterator i = lTmp2.begin(); i != lTmp2.end(); i++ )
	{
		lTmp.push_back( BuildListItem( *i, NULL ) );
	}
}

StringList Builder::buildToStringList( const BuildList &lSrc, const StringList &lIn )
{
	StringList lOut;

	for( BuildList::const_iterator i = lSrc.begin(); i != lSrc.end(); i++ )
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
// Rule functions
//
void Builder::addRule( const char *sName )
{
	lRuleTmp.push_back( RuleInfo() );
	lRuleTmp.back().sName = sName;
}

void Builder::addRuleMatches()
{
	lRuleTmp.back().pMatches = pTmpFunc;
}

void Builder::addRuleProduces()
{
	lRuleTmp.back().lProduces.insert(
		lRuleTmp.back().lProduces.end(),
		lTmp.begin(), lTmp.end()
		);
}

void Builder::addRuleRequires()
{
	lRuleTmp.back().lRequires.insert(
		lRuleTmp.back().lRequires.end(),
		lTmp.begin(), lTmp.end()
		);
}

void Builder::addRuleInputFilter()
{
	lRuleTmp.back().lFilter.push_back( pTmpFunc );
}

void Builder::addRulePerform()
{
	lRuleTmp.back().lPerform.push_back( pTmpPerform );
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
	pTmpPerform = fPerform.instantiate( sName );
}

void Builder::addPerformParam( const char *sParam )
{
	pTmpPerform->addParam( sParam );
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
// Global variable functions
//
void Builder::addGlobalSet( const char *sVar, const char *sValue, int nHow )
{
	lGlobalVars.push_back( SetVar( sVar, sValue, nHow ) );
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
			printf("    %d ", (*j).first );
			printBuildList( (*j).second );
			printf("\n");
		}
	}

	printf("\nTargets:\n");
	for( TargetTmpList::iterator i = lTargetTmp.begin();
		 i != lTargetTmp.end(); i++ )
	{
		printf("  ");
		printBuildList( (*i).first );
		printf(":\n");

		printf("    Rule:     %s\n", (*i).second.sRule.c_str() );
		printf("    Prefix:   %s\n", (*i).second.sPrefix.c_str() );
		printf("    Type:     %s\n", (*i).second.sType.c_str() );
		printf("    Input:    ");
		printBuildList( (*i).second.lInput );
		printf("\n    Requires: ");
		printBuildList( (*i).second.lRequires );
		printf("\n    Vars:\n");
		
		for( SetVarList::iterator j = (*i).second.lVar.begin();
			 j != (*i).second.lVar.end(); j++ )
		{
			char *op;
			switch( (*j).third )
			{
				case setSet: op = "="; break;
				case setAdd: op = "+="; break;
			}
			printf("      %s %s %s\n", (*j).first.c_str(), op, (*j).second.c_str() );
		}
	}

	printf("\nGlobals:\n");
	for( SetVarList::iterator j = lGlobalVars.begin();
		 j != lGlobalVars.end(); j++ )
	{
		char *op;
		switch( (*j).third )
		{
			case setSet: op = "="; break;
			case setAdd: op = "+="; break;
		}
		printf("  %s %s %s\n", (*j).first.c_str(), op, (*j).second.c_str() );
	}

	printf("\nRules:\n");
	for( RuleTmpList::iterator i = lRuleTmp.begin();
		 i != lRuleTmp.end(); i++ )
	{
		printf("  %s:\n", (*i).sName.c_str() );
		printf("    Matches:  func()\n");
		printf("    Produces: ");
		printBuildList( (*i).lProduces );
		printf("\n    Requires: ");
		printBuildList( (*i).lRequires );
		printf("\n    Filters:  %d\n", (*i).lFilter.size() );
		printf("    Performs: %d\n", (*i).lPerform.size() );
	}
}

void Builder::printBuildList( const BuildList &lst )
{
	printf("[");
	for( BuildList::const_iterator k = lst.begin();
		 k != lst.end(); k++ )
	{
		if( k != lst.begin() )
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
	printf("]");
}

//
// Actually make a build object
//
Build *Builder::genBuild()
{
	Build *bld = new Build;

	for( SetVarList::iterator i = lGlobalVars.begin();
		 i != lGlobalVars.end(); i++ )
	{
		switch( (*i).third )
		{
			case setSet:
				bld->set( "", (*i).first, (*i).second );
				break;

			case setAdd:
				bld->setAdd( "", (*i).first, (*i).second );
				break;
		}
	}

	for( TargetTmpList::iterator i = lTargetTmp.begin();
		 i != lTargetTmp.end(); i++ )
	{
		StringList lTargetNames = buildToStringList(
			(*i).first, StringList()
			);
		for( StringList::iterator j = lTargetNames.begin();
			 j != lTargetNames.end(); j++ )
		{
			if( (*i).second.sType != "" )
			{
				Target *pTarget = fTarget.instantiate(
					(*i).second.sType.c_str()
					);
				pTarget->setName( *j );
				pTarget->setRule( (*i).second.sRule );

				StringList lInputs = buildToStringList(
					(*i).second.lInput, StringList()
					);
				pTarget->getInput().insert(
					pTarget->getInput().end(),
					lInputs.begin(), lInputs.end()
					);
				
				bld->addTarget( pTarget );
			}
			StringList lReqs = buildToStringList(
				(*i).second.lRequires, StringList()
				);
			for( StringList::iterator k = lReqs.begin();
				 k != lReqs.end(); k++ )
			{
				bld->addRequires( (*j), (*k) );
			}
			for( SetVarList::iterator k = (*i).second.lVar.begin();
				 k != (*i).second.lVar.end(); k++ )
			{
				switch( (*k).third )
				{
					case setSet:
						bld->set( *j, (*k).first, (*k).second );
						break;
	
					case setAdd:
						bld->setAdd( *j, (*k).first, (*k).second );
						break;
				}
			}
		}
	}

	return bld;
}


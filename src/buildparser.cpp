#include "buildparser.h"
#include "functionfactory.h"
#include "performfactory.h"
#include "targetfactory.h"
#include "action.h"
#include "build.h"
#include "rule.h"
#include "stringprocbuild.h"

BuildParser::BuildParser() :
	fFunction( FunctionFactory::getInstance() ),
	fPerform( PerformFactory::getInstance() ),
	fTarget( TargetFactory::getInstance() )
{
}

BuildParser::~BuildParser()
{
}

void yyparse( BuildParser &bld );
extern int yydebug;

Build *BuildParser::load( const std::string &sFile )
{
	file = sFile;
	scanBegin();
	//yydebug = 1;
	yyparse( *this );
	scanEnd();

	return genBuild();
}

//
// Target functions
//
bool BuildParser::isTarget( const char *sType )
{
	return fTarget.hasPlugin( sType );
}
	
void BuildParser::newTarget()
{
	lTargetTmp.push_back( TargetTmp(lTmp, TargetInfo()) );
}

void BuildParser::setTargetRule( const char *sRule )
{
	lTargetTmp.back().second.sRule = sRule;
}

void BuildParser::setTargetPrefix( const char *sPrefix )
{
	lTargetTmp.back().second.sPrefix = sPrefix;
}

void BuildParser::setTargetType( const char *sType )
{
	lTargetTmp.back().second.sType = sType;
}

void BuildParser::addTargetInput()
{
	lTargetTmp.back().second.lInput.insert(
		lTargetTmp.back().second.lInput.end(),
		lTmp.begin(), lTmp.end()
		);
}

void BuildParser::addTargetRequires()
{
	lTargetTmp.back().second.lRequires.insert(
		lTargetTmp.back().second.lRequires.end(),
		lTmp.begin(), lTmp.end()
		);
}

void BuildParser::addTargetSet( const char *sVar, const char *sVal, int nHow )
{
	lTargetTmp.back().second.lVar.push_back( SetVar( sVar, sVal, nHow ) );
}

//
// Function functions
//
bool BuildParser::isFunction( const char *sFunc )
{
	return fFunction.hasPlugin( sFunc );
}

void BuildParser::newFunctionCall( const char *sName )
{
	pTmpFunc = fFunction.instantiate( sName );
}

void BuildParser::addFunctionParam( const char *sParam )
{
	pTmpFunc->addParam( sParam );
}

//
// List functions
//
void BuildParser::newList()
{
	lTmp.clear();
}

void BuildParser::addListString( const char *str )
{
	lTmp.push_back( BuildListItem(str, NULL) );
}

void BuildParser::addListFunc()
{
	lTmp.push_back( BuildListItem("", pTmpFunc ) );
}

void BuildParser::filterList()
{
	StringList lTmp2;
	StringList lIn = buildToStringList( lTmp, StringList() );
	pTmpFunc->execute( NULL, lIn, lTmp2 );
	lTmp.clear();
	for( StringList::iterator i = lTmp2.begin(); i != lTmp2.end(); i++ )
	{
		lTmp.push_back( BuildListItem( *i, NULL ) );
	}
}

StringList BuildParser::buildToStringList( const BuildList &lSrc, const StringList &lIn )
{
	StringList lOut;

	for( BuildList::const_iterator i = lSrc.begin(); i != lSrc.end(); i++ )
	{
		if( (*i).second )
		{
			(*i).second->execute( NULL, lIn, lOut );
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
void BuildParser::addRule( const char *sName )
{
	lRuleTmp.push_back( RuleInfo() );
	lRuleTmp.back().sName = sName;
	lRuleTmp.back().pAggregate = NULL;
}

void BuildParser::addRuleMatches()
{
	lRuleTmp.back().pMatches = pTmpFunc;
}

void BuildParser::addRuleProduces()
{
	lRuleTmp.back().lProduces.insert(
		lRuleTmp.back().lProduces.end(),
		lTmp.begin(), lTmp.end()
		);
}

void BuildParser::addRuleRequires()
{
	lRuleTmp.back().lRequires.insert(
		lRuleTmp.back().lRequires.end(),
		lTmp.begin(), lTmp.end()
		);
}

void BuildParser::addRuleInputFilter()
{
	lRuleTmp.back().lFilter.push_back( pTmpFunc );
}

void BuildParser::addRulePerform()
{
	lRuleTmp.back().lPerform.push_back( pTmpPerform );
}

void BuildParser::setRuleAggregate()
{
	lRuleTmp.back().pAggregate = pTmpFunc;
}

//
// Perform functions
//
bool BuildParser::isPerform( const char *sPerf )
{
	return fPerform.hasPlugin( sPerf );
}

void BuildParser::newPerform( const char *sName )
{
	pTmpPerform = fPerform.instantiate( sName );
}

void BuildParser::addPerformParam( const char *sParam )
{
	pTmpPerform->addParam( sParam );
}

//
// Functions for dealing with actions
//
void BuildParser::addAction()
{
	lActions.push_back( ActionTmp("", ActionTmpCmdList()) );
}

void BuildParser::addAction( const char *sName )
{
	lActions.push_back( ActionTmp(sName, ActionTmpCmdList()) );
}

void BuildParser::addCommand( int nType )
{
	lActions.back().second.push_back( ActionTmpCmd( nType, lTmp ) );
}

//
// Global variable functions
//
void BuildParser::addGlobalSet( const char *sVar, const char *sValue, int nHow )
{
	lGlobalVars.push_back( SetVar( sVar, sValue, nHow ) );
}

//
// Debug
//
void BuildParser::debugDump()
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

void BuildParser::printBuildList( const BuildList &lst )
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
Build *BuildParser::genBuild()
{
	Build *bld = new Build;
	bld->setStringProc( new StringProcBuild( bld ) );

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

	for( RuleTmpList::iterator i = lRuleTmp.begin(); i != lRuleTmp.end(); i++ )
	{
		Rule *pRule = new Rule;
		pRule->setName( (*i).sName );
		pRule->getMatchesList().push_back( (*i).pMatches );

		for( FunctionList::iterator j = (*i).lFilter.begin();
			 j != (*i).lFilter.end(); j++ )
		{
			pRule->getFilterList().push_back( *j );
		}

		for( PerformList::iterator j = (*i).lPerform.begin();
			 j != (*i).lPerform.end(); j++ )
		{
			pRule->getPerformList().push_back( *j );
		}

		for( BuildList::iterator j = (*i).lProduces.begin();
			 j != (*i).lProduces.end(); j++ )
		{
			if( (*j).second )
			{
				throw BuildException(
					"You cannot have functions in produces lists (rule %s).",
					(*i).sName.c_str() );
			}
			pRule->getProducesList().push_back( (*j).first );
		}

		if( (*i).pAggregate )
		{
			pRule->setAggregate( (*i).pAggregate );
		}

		for( BuildList::iterator j = (*i).lRequires.begin();
			 j != (*i).lRequires.end(); j++ )
		{
			if( (*j).second )
			{
				pRule->getReqFuncList().push_back( (*j).second );
			}
			else
			{
				pRule->getReqStrList().push_back( (*j).first );
			}
		}

		bld->addRule( pRule );
	}

	for( ActionTmpList::iterator i = lActions.begin();
		 i != lActions.end(); i++ )
	{
		Action *pAct = new Action;
		pAct->setName( (*i).first );

		for( ActionTmpCmdList::iterator j = (*i).second.begin();
			 j != (*i).second.end(); j++ )
		{
			StringList lWhat = buildToStringList(
				(*j).second, StringList()
				);

			for( StringList::iterator k = lWhat.begin();
				 k != lWhat.end(); k++ )
			{
				pAct->addCommand( (Action::eAction)((*j).first), *k );
			}
		}

		bld->addAction( pAct );
	}

	return bld;
}


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
	lTargetTmp.back().second.lInput.first.insert(
		lTargetTmp.back().second.lInput.first.end(),
		lTmp.first.begin(), lTmp.first.end()
		);
	lTargetTmp.back().second.lInput.second = lTmp.second;
}

void BuildParser::addTargetRequires()
{
	lTargetTmp.back().second.lRequires.first.insert(
		lTargetTmp.back().second.lRequires.first.end(),
		lTmp.first.begin(), lTmp.first.end()
		);
	lTargetTmp.back().second.lRequires.second = lTmp.second;
}

void BuildParser::addTargetSet( const char *sVar, const char *sVal, int nHow )
{
	lTargetTmp.back().second.lVar.push_back( SetVar( sVar, sVal, nHow ) );
}

void BuildParser::addTargetGroup( const char *sGroup )
{
	lTargetTmp.back().second.lGroups.push_back( sGroup );
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
	lTmp.first.clear();
	lTmp.second = NULL;
}

void BuildParser::addListString( const char *str )
{
	lTmp.first.push_back( BuildListItem(str, NULL) );
}

void BuildParser::addListFunc()
{
	lTmp.first.push_back( BuildListItem("", pTmpFunc ) );
}

void BuildParser::filterList()
{
	lTmp.second = pTmpFunc;
	//StringList lTmp2;
	//StringList lIn = buildToStringList( lTmp, StringList() );
	//pTmpFunc->execute( NULL, lIn, lTmp2 );
	//lTmp.clear();
	//for( StringList::iterator i = lTmp2.begin(); i != lTmp2.end(); i++ )
	//{
	//	lTmp.push_back( BuildListItem( *i, NULL ) );
	//}
}

void BuildParser::buildListFilter( BuildList &lSrc )
{
	if( lSrc.second == NULL )
		return;

	StringList lTmp2;
	StringList lIn = buildToStringList( lSrc, StringList() );
	pTmpFunc->execute( NULL, lIn, lTmp2 );
	lSrc.first.clear();
	delete lSrc.second;
	for( StringList::iterator i = lTmp2.begin(); i != lTmp2.end(); i++ )
	{
		lSrc.first.push_back( BuildListItem( *i, NULL ) );
	}
}

StringList BuildParser::buildToStringList( const BuildList &lSrc, const StringList &lIn, Build *pPass )
{
	StringList lOut;

	for( BuildListCore::const_iterator i = lSrc.first.begin();
		 i != lSrc.first.end(); i++ )
	{
		if( (*i).second )
		{
			(*i).second->execute( pPass, lIn, lOut );
		}
		else
		{
			lOut.push_back( (*i).first );
		}
	}

	if( lSrc.second )
	{
		StringList lTmp;
		lSrc.second->execute( pPass, lOut, lTmp );
		lOut.swap( lTmp );
	}

	return lOut;
}

StringList BuildParser::buildToStringListDup( const BuildList &lSrc, const StringList &lIn, Build &bld, const std::string &sCont, VarMap *mExtra, Build *pPass )
{
	StringList lOut;

	for( BuildListCore::const_iterator i = lSrc.first.begin();
		 i != lSrc.first.end(); i++ )
	{
		if( (*i).second )
		{
			StringList l;
			l.push_back( sCont );
			Function *pTmp = (*i).second->duplicate( bld, &l, mExtra );
			pTmp->execute( pPass, lIn, lOut );
			delete pTmp;
		}
		else
		{
			StringList l;
			l.push_back( sCont );
			lOut.push_back( bld.replVars( (*i).first, &l, mExtra ) );
		}
	}

	if( lSrc.second )
	{
		StringList lTmp;
		lSrc.second->execute( pPass, lOut, lTmp );
		lOut.swap( lTmp );
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
	lRuleTmp.back().lProduces.first.insert(
		lRuleTmp.back().lProduces.first.end(),
		lTmp.first.begin(), lTmp.first.end()
		);
}

void BuildParser::addRuleRequires()
{
	lRuleTmp.back().lRequires.first.insert(
		lRuleTmp.back().lRequires.first.end(),
		lTmp.first.begin(), lTmp.first.end()
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
	lActions.push_back( ActionTmp("", ActionTmpCmdList() ) );
}

void BuildParser::addAction( const char *sName )
{
	lActions.push_back( ActionTmp(sName, ActionTmpCmdList()) );
}

void BuildParser::addCommand( int nType )
{
	lActions.back().second.push_back( ActionTmpCmd( nType, lTmp ) );
}

void BuildParser::addGrpCommand( const char *sGroup, int nType )
{
	lActions.back().second.push_back( ActionTmpCmd( nType, sGroup ) );
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
			printf("    %d ", (*j).nAct );
			if( (*j).bGroup )
			{
				printf("!%s", (*j).sGroup.c_str() );
			}
			else
			{
				printBuildList( (*j).lCmds );
			}
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
	for( BuildListCore::const_iterator k = lst.first.begin();
		 k != lst.first.end(); k++ )
	{
		if( k != lst.first.begin() )
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
			(*i).first, StringList(), bld
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

				VarMap mExtra;
				mExtra["target"] = (*j);

				StringList lInputs = buildToStringListDup(
					(*i).second.lInput, StringList(),
					*bld, *j, &mExtra /*, bld <-- should this be here? */
					);
				pTarget->getInput().insert(
					pTarget->getInput().end(),
					lInputs.begin(), lInputs.end()
					);
				
				bld->addTarget( pTarget );

				for( StringList::iterator c = (*i).second.lGroups.begin();
					 c != (*i).second.lGroups.end(); c++ )
				{
					bld->addToGroup( *c, pTarget );
				}
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

		for( BuildListCore::iterator j = (*i).lProduces.first.begin();
			 j != (*i).lProduces.first.end(); j++ )
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

		for( BuildListCore::iterator j = (*i).lRequires.first.begin();
			 j != (*i).lRequires.first.end(); j++ )
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
			if( (*j).bGroup )
			{
				pAct->addCommand(
					(Action::eAction)((*j).nAct), (*j).sGroup, true
					);
			}
			else
			{
				StringList lWhat = buildToStringList(
					(*j).lCmds, StringList(), bld
					);

				for( StringList::iterator k = lWhat.begin();
					 k != lWhat.end(); k++ )
				{
					pAct->addCommand( (Action::eAction)((*j).nAct), *k, false );
				}
			}
		}

		bld->addAction( pAct );
	}

	// Now create an auto check-action for each group where there isn't already
	// an action.
	for( TargetTmpList::iterator i = lTargetTmp.begin();
		 i != lTargetTmp.end(); i++ )
	{
		for( StringList::iterator j = (*i).second.lGroups.begin();
			 j != (*i).second.lGroups.end(); j++ )
		{
			if( !bld->hasAction( (*j) ) )
			{
				Action *pAct = new Action;
				pAct->setName( *j );
				pAct->addCommand( Action::actCheck, *j, true );
				bld->addAction( pAct );
			}
		}
	}

	return bld;
}


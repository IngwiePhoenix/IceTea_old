#include <iostream>

#include "builder.h"
#include "action.h"
#include "command.h"
#include "target.h"
#include "build.tab.h"
#include "rule.h"
#include "viewer.h"

subExceptionDef( BuildException )

Builder::Builder( Viewer &rView ) :
	pDefaultAction( NULL ),
	pLastAddedAction( NULL ),
	sTmp(""),
	sContext(""),
	rView( rView )
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

void Builder::build( const char *sAct )
{
	Action *pAct;
	if( sAct == NULL )
		pAct = pDefaultAction;
	else
	{
		if( mAction.find( sAct ) == mAction.end() )
			throw BuildException("No action matches '%s'.", sAct );
		pAct = mAction[sAct];
	}

	rView.beginAction( sAct, pAct->getNumCommands() );

	pAct->execute( *this );

	rView.endAction();
}

void Builder::execute( Action *pAct )
{
	pAct->execute( *this );
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

	printf("Dependancies:\n");
	for( std::map<std::string, std::list<std::string> *>::iterator i =
		 mRequires.begin(); i != mRequires.end(); i++ )
	{
		printf("   %s: ", (*i).first.c_str() );
		std::list<std::string> *pList = (*i).second;
		int i = 0;
		for( std::list<std::string>::iterator j = pList->begin();
			 j != pList->end(); j++ )
		{
			if( j != pList->begin() )
				printf(", ");
			printf("%s", (*j).c_str() );
			if( i++ >= 3 )
			{
				printf("...");
				break;
			}
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

	std::string newVal = varRepl( sValue, sContext, NULL );

	if( sContext[0] == '\0' )
	{
		mVar[sName] = newVal;
	}
	else
	{
		mContVar[sContext.getString()][sName] = newVal;
	}
}

void Builder::varAddSet( const char *sName, const char *sValue )
{
	checkVar( sContext, sName );

	std::string newVal = varRepl( sValue, sContext, NULL );

	if( sContext[0] == '\0' )
	{
		std::string s = mVar[sName];
		s += " ";
		s += newVal;
		mVar[sName] = s;
	}
	else
	{
		std::string s = mContVar[sContext.getString()][sName];
		s += " ";
		s += newVal;
		mContVar[sContext.getString()][sName] = s;
	}
}

void Builder::processRequires( std::list<std::string> &lInput )
{
	// These are cheap and handy to have all the time
	for( regreqlist::iterator i = lRequiresRegexp.begin();
		 i != lRequiresRegexp.end(); i++ )
	{
		RegExp *re = (*i).first;
		for( std::list<std::string>::iterator j = lInput.begin();
			 j != lInput.end(); j++ )
		{
			if( re->execute( (*j).c_str() ) )
			{
				varmap *revars = regexVars( re );
				requiresNormal(
					(*j).c_str(),
					varRepl(
						(*i).second.c_str(),
						"",
						revars
						).c_str()
					);
				delete revars;
			}
		}
	}

	// These are only done on request now, they were too expensive
	/*
	for( regreqlist::iterator i = lRequiresRegexpCommand.begin();
		 i != lRequiresRegexpCommand.end(); i++ )
	{
		RegExp *re = (*i).first;
		for( std::list<std::string>::iterator j = lInput.begin();
			 j != lInput.end(); j++ )
		{
			if( re->execute( (*j).c_str() ) )
			{
				varmap *revars = regexVars( re );
				std::string s = varRepl( (*i).second.c_str(), "", revars );
				FILE *fcmd = popen( s.c_str(), "r" );
				std::string rhs;
				bool bHeader = true;
				for(;;)
				{
					if( feof( fcmd ) )
						break;
					int cc = fgetc( fcmd );
					if( cc == EOF )
						break;
					unsigned char c = cc;
					if( bHeader )
					{
						if( c == ':' )
							bHeader = false;
					}
					else
					{
						if( c == ' ' || c == '\t' )
						{
							if( rhs != "" )
							{
								requiresNormal(
									(*j).c_str(),
									rhs.c_str()
									);
								rhs = "";
							}
						}
						else
						{
							if( c == '\\' )
								c = fgetc( fcmd );
							if( c != '\n' )
								rhs += c;
						}
					}
				}
				if( rhs != "" )
				{
					requiresNormal(
						(*j).c_str(),
						rhs.c_str()
						);
					rhs = "";
				}
				pclose( fcmd );
				delete revars;
			}
		}
	}
	*/
}

void Builder::genRequiresFor( const char *sName )
{
	for( regreqlist::iterator i = lRequiresRegexpCommand.begin();
		 i != lRequiresRegexpCommand.end(); i++ )
	{
		RegExp *re = (*i).first;
		if( re->execute( sName ) )
		{
			varmap *revars = regexVars( re );
			std::string s = varRepl( (*i).second.c_str(), "", revars );
			rView.beginExtraRequiresCheck( s.c_str() );
			rView.executeCmd( s.c_str() );
			FILE *fcmd = popen( s.c_str(), "r" );
			std::string rhs;
			bool bHeader = true;
			for(;;)
			{
				if( feof( fcmd ) )
					break;
				int cc = fgetc( fcmd );
				if( cc == EOF )
					break;
				unsigned char c = cc;
				if( bHeader )
				{
					if( c == ':' )
						bHeader = false;
				}
				else
				{
					if( c == ' ' || c == '\t' )
					{
						if( rhs != "" )
						{
							requiresNormal(
								sName,
								rhs.c_str()
								);
							rhs = "";
						}
					}
					else
					{
						if( c == '\\' )
							c = fgetc( fcmd );
						if( c != '\n' )
							rhs += c;
					}
				}
			}
			if( rhs != "" )
			{
				requiresNormal(
					sName,
					rhs.c_str()
					);
				rhs = "";
			}
			pclose( fcmd );
			delete revars;
			rView.endExtraRequiresCheck();
		}
	}
}

std::map<std::string, std::string> *Builder::regexVars( RegExp *re )
{
	varmap *map = new varmap;

	int jmax = re->getNumSubStrings();
	for( int j = 0; j < jmax; j++ )
	{
		char buf[8];
		sprintf( buf, "re:%d", j );
		(*map)[buf] = re->getSubString( j );
	}

	return map;
}

void Builder::requires( const char *sBase, const char *sReq )
{
	if( bReqRegexp )
	{
		requiresRegexp( sBase, sReq );
	}
	else
	{
		requiresNormal( sBase, sReq );
	}
}

void Builder::requiresNormal( const char *sBase, const char *sReq )
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

void Builder::requiresRegexp( const char *sBase, const char *sReq )
{
	lRequiresRegexp.push_back(
		std::pair<RegExp *, std::string>(
			new RegExp( sBase ),
			sReq
			)
		);
}

void Builder::requiresFromCommand( const char *sBase, const char *sCmd )
{
	lRequiresRegexpCommand.push_back(
		std::pair<RegExp *, std::string>(
			new RegExp( sBase ),
			sCmd
			)
		);
}

void Builder::setContext( const char *sCont )
{
	sContext = sCont;
}

void Builder::setContext()
{
	setContext("");
}

bool Builder::hasVar( varmap *pMap, std::string &var )
{
	if( pMap == NULL ) 
		return false;
	if( pMap->find( var ) == pMap->end() )
		return false;
	return true;
}

std::string Builder::varRepl( const char *sSrc, const char *cont, varmap *mExtra )
{
	varmap *mCont = NULL;
	if( cont[0] != '\0' )
	{
		if( mContVar.find( cont ) != mContVar.end() )
			mCont = &mContVar[cont];
	}

	std::string out;
	std::string var;
	bool bVar = false;

	for( const char *s = sSrc; *s; s++ )
	{
		if( *s == '{' )
		{
			bVar = true;
			continue;
		}
		else if( *s == '}' && bVar )
		{
			if( hasVar( mExtra, var ) )
			{
				out += (*mExtra)[var];
			}
			else if( hasVar( mCont, var ) )
			{
				out += (*mCont)[var];
			}
			else if( hasVar( &mVar, var ) )
			{
				out += mVar[var];
			}
			var = "";
			bVar = false;
			continue;
		}

		if( bVar == true )
		{
			var += *s;
		}
		else
		{
			out += *s;
		}
	}

	return out;
}

Rule *Builder::getRule( const char *sName )
{
	if( mRule.find( sName ) != mRule.end() )
		return mRule[sName];

	return NULL;
}

std::list<Rule *> Builder::findRuleChain( Rule *pRule )
{
	std::list<Rule *> ret;

	for( std::map<const char *, Rule *, ltstr>::iterator i = mRule.begin();
		 i != mRule.end(); i++ )
	{
		if( pRule == (*i).second )
			continue;

		if( pRule->willChain( (*i).second ) )
		{
			ret.push_back( (*i).second );
		}
	}

	return ret;
}

void cleanList( std::list<std::string> &lst )
{
	std::map<std::string, bool> m;

	for( std::list<std::string>::iterator i = lst.begin(); i != lst.end(); i++ )
	{
		if( m.find( *i ) == m.end() )
			m[ *i ] = true;
		else
		{
			std::list<std::string>::iterator j = i;
			j--;
			lst.erase( i );
			i = j;
		}
	}
}

void Builder::error( const std::string &err )
{
	throw BuildException( err.c_str() );
}


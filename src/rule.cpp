#include "rule.h"
#include "perform.h"
#include "performcmd.h"
#include "builder.h" // for BuildException

Rule::Rule( const char *sName ) :
	sName( sName ),
	bNoProduces( true )
{
	lProduces.push_back("{target}");
}

Rule::~Rule()
{
}

void Rule::debug()
{
	printf("   Rule %s:\n",
		sName.getString()
		);
	printf("      Produces: ");
	if( lProduces.empty() )
		printf("{target}");
	else
	{
		for( std::list<std::string>::iterator i = lProduces.begin();
			 i != lProduces.end(); i++ )
		{
			if( i != lProduces.begin() )
				printf(", ");
			printf("%s", (*i).c_str() );
		}
	}
	printf("\n      Matches ");
	if( mHow == matchOne )
		printf("one ");
	else if( mHow == matchAll )
		printf("all ");
	printf("/%s/\n", rWhat.getSource() );

	printf("      Performs ");
	if( pHow == perfCommand )
		printf("command ");
	printf("\"%s\"\n", sPerfCmd.getString() );
}

void Rule::addProduces( const char *sP )
{
	if( bNoProduces )
	{
		lProduces.clear();
		bNoProduces = false;
	}
	lProduces.push_back( sP );
}

void Rule::setMatches( Matches how, const char *sW )
{
	rWhat.compile( sW );
	mHow = how;
}

void Rule::setPerforms( ePerform pwhat, const char *sperfcmd )
{
	pHow = pwhat;
	sPerfCmd = sperfcmd;
}

Perform *Rule::buildCommand( Builder &bld, const char *sCmd, Builder::varmap *vars )
{
	return new PerformCmd(
			bld.varRepl( sCmd, (*vars)["target"].c_str(), vars ).c_str(),
			(*vars)["target"].c_str()
			);
}

std::list<std::string> Rule::execute( Builder &bld, std::list<std::string> lInput, std::list<Perform *> &lPerf, const char *sTarget )
{
	std::list<Rule *> lRule = bld.findRuleChain( this );
	/*
	if( !lRule.empty() )
	{
		printf("Rule %s chains to: ", sName.getString() );
		for( std::list<Rule *>::iterator i = lRule.begin();
			 i != lRule.end(); i++ )
		{
			if( i != lRule.begin() )
				printf(", ");
			printf("%s", (*i)->sName.getString() );
		}
		printf("\n");
	}*/

	std::list<std::string> lOutput;
	std::string sMatches;

	for( std::list<Rule *>::iterator i = lRule.begin(); i != lRule.end(); i++ )
	{
		std::list<std::string> lTmp = (*i)->execute( bld, lInput, lPerf );
		lOutput.insert( lOutput.end(), lTmp.begin(), lTmp.end() );
	}

	std::list<std::string> lTest;
	lTest.insert( lTest.end(), lInput.begin(), lInput.end() );
	lTest.insert( lTest.end(), lOutput.begin(), lOutput.end() );
	
	cleanList( lTest );

	for( std::list<std::string>::iterator i = lTest.begin();
		 i != lTest.end(); i++ )
	{
		if( rWhat.execute( (*i).c_str() ) )
		{
			Builder::varmap *revars = bld.regexVars( &rWhat );
			for( std::list<std::string>::iterator j = lProduces.begin();
				 j != lProduces.end(); j++ )
			{
				if( mHow == matchOne )
				{
					lOutput.push_back(
						bld.varRepl(
							(*j).c_str(),
							"",
							revars
							)
						);
					(*revars)["target"] = (sTarget==NULL)?
						(lOutput.back().c_str()):(sTarget);
					(*revars)["match"] = (*i).c_str();
					Perform *p = buildCommand(
						bld,
						sPerfCmd,
						revars
						);
					lPerf.push_back( p );
				}
				else if( mHow == matchAll )
				{
					sMatches += " ";
					sMatches += (*i);
				}
			}
			delete revars;
		}
	}
	//std::list<std::string> lTmp = findTargets( bld, lTest, sMatches, sTarget );
	//lOutput.insert( lOutput.end(), lTmp.begin(), lTmp.end() );

	if( mHow == matchAll )
	{
		lOutput.push_back(
			bld.varRepl(
				sTarget,
				sTarget,
				NULL
				)
			);
		Builder::varmap vars;
		vars["target"] = sTarget;
		vars["match"] = sMatches;
		Perform *p = buildCommand(
			bld,
			sPerfCmd,
			&vars
			);
		lPerf.push_back( p );
	}

	return lOutput;
}

bool Rule::willChain( Rule *pRule )
{
	for( std::list<std::string>::iterator i = pRule->lProduces.begin();
		 i != pRule->lProduces.end(); i++ )
	{
		if( rWhat.execute( (*i).c_str() ) )
			return true;
	}

	return false;
}


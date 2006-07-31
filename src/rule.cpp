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

Perform *Rule::buildCommand( Builder &bld, const char *sCmd, const char *sTarget, const char *sMatches )
{
	Builder::varmap vars;
	vars["target"] = sTarget;
	vars["match"] = sMatches;
	return new PerformCmd( bld.varRepl( sCmd, "", &vars ).c_str(), sTarget );
}

std::list<std::string> Rule::findTargets( Builder &bld, std::list<std::string> &lIn, std::string &sMatches, const char *sTarget )
{
	std::list<std::string> lTmp;

	for( std::list<std::string>::iterator i = lIn.begin();
		 i != lIn.end(); i++ )
	{
		if( rWhat.execute( (*i).c_str() ) )
		{
			Builder::varmap *revars = bld.regexVars( &rWhat );
			for( std::list<std::string>::iterator j = lProduces.begin();
				 j != lProduces.end(); j++ )
			{
				if( mHow == matchOne )
				{
					lTmp.push_back(
						bld.varRepl(
							(*j).c_str(),
							"",
							revars
							)
						);
					Perform *p = buildCommand(
						bld,
						sPerfCmd,
						(sTarget==NULL)?(lTmp.back().c_str()):(sTarget),
						(*i).c_str()
						);
					p->execute( bld );
					delete p;
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

	return lTmp;
}

std::list<std::string> Rule::execute( Builder &bld, std::list<std::string> lInput, const char *sTarget )
{
	std::list<Rule *> lRule = bld.findRuleChain( this );

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
	}

	std::list<std::string> lOutput;
	std::string sMatches;

	for( std::list<Rule *>::iterator i = lRule.begin(); i != lRule.end(); i++ )
	{
		std::list<std::string> lTmp = (*i)->execute( bld, lInput );
		lOutput.insert( lOutput.end(), lTmp.begin(), lTmp.end() );
	}

	std::list<std::string> lTmp = findTargets( bld, lInput, sMatches, sTarget );
	lOutput.insert( lOutput.end(), lTmp.begin(), lTmp.end() );
	lTmp = findTargets( bld, lOutput, sMatches, sTarget );
	lOutput.insert( lOutput.end(), lTmp.begin(), lTmp.end() );

	if( mHow == matchAll )
	{
		lOutput.push_back(
			bld.varRepl(
				sTarget,
				"",
				NULL
				)
			);
		Perform *p = buildCommand(
			bld,
			sPerfCmd,
			sTarget,
			sMatches.c_str()
			);
		p->execute( bld );
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


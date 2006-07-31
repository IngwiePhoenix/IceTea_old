#ifndef RULE_H
#define RULE_H

#include <list>
#include <string>
#include <stdint.h>
#include "regexp.h"
#include "staticstring.h"

class Perform;

class Rule
{
public:
	enum Matches
	{
		matchOne,
		matchAll
	};

	enum ePerform
	{
		perfCommand
	};

public:
	Rule( const char *sName );
	virtual ~Rule();

	const char *getName()
	{
		return sName;
	}

	void debug();

	void addProduces( const char *sProduces );
	void setMatches( Matches how, const char *sWhat );
	void setPerforms( ePerform pwhat, const char *sPerfCmd );

	bool willChain( Rule *pRule );

	std::list<std::string> execute( class Builder &bld, std::list<std::string> lInput, const char *sTarget=NULL );

private:
	class Perform *buildCommand( class Builder &bld, const char *sCmd, const char *sTarget, const char *sMatches );
	std::list<std::string> findTargets( class Builder &bld, std::list<std::string> &lIn, std::string &sMatches, const char *sTarget );
	StaticString sName;
	std::list<std::string> lProduces;

	Matches mHow;
	RegExp rWhat;
	//StaticString sWhat;
	//regex_t rWhat;

	ePerform pHow;
	StaticString sPerfCmd;

	bool bNoProduces;
};

#endif

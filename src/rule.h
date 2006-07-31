#ifndef RULE_H
#define RULE_H

#include <list>
#include <string>
#include <stdint.h>
#include <regex.h>
#include "staticstring.h"

class Rule
{
public:
	enum Matches
	{
		matchOne,
		matchAll
	};

	enum Perform
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
	void setPerforms( Perform pwhat, const char *sPerfCmd );

	std::list<std::string> execute( class Builder &bld, std::list<std::string> lInput );

private:
	StaticString sName;
	std::list<std::string> lProduces;

	Matches mHow;
	StaticString sWhat;
	regex_t rWhat;

	Perform pHow;
	StaticString sPerfCmd;
};

#endif

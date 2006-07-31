#ifndef RULE_H
#define RULE_H

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

	void setProduces( const char *sProduces );
	void setMatches( Matches how, const char *sWhat );
	void setPerforms( Perform pwhat, const char *sPerfCmd );

private:
	StaticString sName;
	StaticString sProduces;

	Matches mHow;
	StaticString sWhat;
	regex_t rWhat;

	Perform pHow;
	StaticString sPerfCmd;
};

#endif

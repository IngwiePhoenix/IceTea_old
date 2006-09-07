#ifndef RULE_H
#define RULE_H

#include <stdint.h>
#include <string>
#include <list>

class Function;
class Perform;

typedef std::list<Function *> FunctionList;
typedef std::list<Perform *> PerformList;

class Rule
{
public:
	Rule();
	virtual ~Rule();

	void execute();

	std::string getName()
	{
		return sName;
	}

	void setName( const std::string &sName )
	{
		this->sName = sName;
	}

	FunctionList &getFilterList()
	{
		return lFilter;
	}

	PerformList &getPerformList()
	{
		return lPerform;
	}

	FunctionList &getMatchesList()
	{
		return lMatches;
	}

private:
	std::string sName;
	FunctionList lMatches;
	FunctionList lFilter;
	PerformList lPerform;
};

#endif

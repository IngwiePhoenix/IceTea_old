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

	std::string getName()
	{
		return sName;
	}

	void setName( const std::string &sName )
	{
		this->sName = sName;
	}

	FunctionList &getFunctionList()
	{
		return lFilter;
	}

	PerformList &getPerformList()
	{
		return lPerform;
	}

private:
	std::string sName;
	FunctionList lFilter;
	PerformList lPerform;
};

#endif

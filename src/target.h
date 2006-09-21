#ifndef TARGET_H
#define TARGET_H

#include <stdint.h>

#include <string>
#include <list>

#include "action.h"

typedef std::list<std::string> StringList;

class Build;

class Target
{
public:
	Target();
	virtual ~Target();

	void run( Action::eAction nAct, Build &bld );
	virtual void check( Build &bld ) = 0;
	virtual void clean( Build &bld ) = 0;

	void setName( const std::string &sName )
	{
		this->sName = sName;
	}

	std::string getName()
	{
		return sName;
	}

	void setRule( const std::string &sRule )
	{
		this->sRule = sRule;
	}

	std::string getRule()
	{
		return sRule;
	}

	StringList &getInput()
	{
		return lInput;
	}

	bool wasRun()
	{
		return bRun;
	}


private:
	std::string sName;
	std::string sRule;
	StringList lInput;
	bool bRun;
};

#endif

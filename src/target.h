#ifndef TARGET_H
#define TARGET_H

#include <stdint.h>

#include <string>
#include <list>

typedef std::list<std::string> StringList;

class Target
{
public:
	Target();
	virtual ~Target();

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

private:
	std::string sName;
	std::string sRule;
	StringList lInput;
};

#endif

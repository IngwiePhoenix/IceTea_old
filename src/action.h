#ifndef ACTION_H
#define ACTION_H

#include <list>
#include "staticstring.h"

class Command;
class Builder;

class Action
{
public:
	Action();
	Action( const char *sName );
	virtual ~Action();

	void add( Command *pCmd );
	void add( int nType, const char *sCmd );

	const char *getName()
	{
		return sName;
	}
	bool isDefault()
	{
		return bDefault;
	}

	int getNumCommands()
	{
		return lCommand.size();
	}

	void debug();

	void execute( class Builder &bld );

private:
	bool bDefault;
	StaticString sName;
	std::list<Command *> lCommand;
	std::list<std::pair<int, std::string> > lRegExCommand;
};

#endif

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

	const char *getName()
	{
		return sName;
	}
	bool isDefault()
	{
		return bDefault;
	}

	void debug();

	void execute( class Builder &bld );

private:
	bool bDefault;
	StaticString sName;
	std::list<Command *> lCommand;
};

#endif

#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include "staticstring.h"

class Builder;

class Command
{
public:
	enum CmdType
	{
		cmdCheck		= 0,
		cmdClean
	};

public:
	Command( CmdType cmd, const char *sTarget );
	virtual ~Command();

	CmdType getType()
	{
		return nType;
	}

	const char *getTarget()
	{
		return sTarget;
	}

	void debug();

	void execute( class Builder &bld );

private:
	CmdType nType;
	StaticString sTarget;

};

#endif

#ifndef PERFORM_CMD_H
#define PERFORM_CMD_H

#include <stdint.h>

#include "perform.h"
#include "staticstring.h"

class PerformCmd : public Perform
{
public:
	PerformCmd( const char *sCmd, const char *sTarget );
	virtual ~PerformCmd();

	virtual void execute( class Builder &bld );

private:
	StaticString sCommand;
	StaticString sTarget;
};

#endif

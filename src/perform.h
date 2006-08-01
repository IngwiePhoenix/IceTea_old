#ifndef PERFORM_H
#define PERFORM_H

#include <stdint.h>
#include "staticstring.h"

class Perform
{
public:
	Perform( const char *sTarget );
	virtual ~Perform();

	virtual void execute( class Builder &bld ) = 0;

	const char *getTarget()
	{
		return sTarget;
	}

private:
	StaticString sTarget;

};

#endif

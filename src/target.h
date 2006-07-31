#ifndef TARGET_H
#define TARGET_H

#include <stdint.h>
#include "staticstring.h"

class Target
{
public:
	Target( const char *sName );
	virtual ~Target();

	const char *getName()
	{
		return sName;
	}

	void setRule( const char *sRule );

	virtual void debug();

private:
	StaticString sName;
	StaticString sRule;

};

#endif

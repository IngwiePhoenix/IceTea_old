#ifndef TARGET_H
#define TARGET_H

#include <list>
#include <string>
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

	void addInput( const char *sInput );
	void addOutput( const char *sOutput );

	virtual void check( class Builder &bld ) = 0;
	virtual void clean( class Builder &bld ) = 0;

protected:
	StaticString sName;
	StaticString sRule;

	std::list<std::string> lInput;
	std::list<std::string> lOutput;

};

#endif

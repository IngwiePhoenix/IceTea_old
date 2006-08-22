#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>
#include "builder.h"

class Function
{
public:
	Function();
	virtual ~Function();

	void addParam( const char *str );
	virtual void execute( StringList &lInput, StringList &lOutput )=0;

private:
	StringList lParams;

};

#endif

#ifndef FUNCTION_REGEXP_H
#define FUNCTION_REGEXP_H

#include <stdint.h>

#include "function.h"

class FunctionRegexp : public Function
{
public:
	FunctionRegexp();
	virtual ~FunctionRegexp();
	
	virtual void execute( StringList &lInput, StringList &lOutput );

private:

};

#endif

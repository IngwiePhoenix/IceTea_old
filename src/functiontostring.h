#ifndef FUNCTION_TO_STRING_H
#define FUNCTION_TO_STRING_H

#include <stdint.h>

#include "function.h"

class FunctionToString : public Function
{
public:
	FunctionToString();
	virtual ~FunctionToString();
	
	virtual void execute( StringList &lInput, StringList &lOutput );

private:

};

#endif

#ifndef FUNCTION_FILES_IN_H
#define FUNCTION_FILES_IN_H

#include <stdint.h>

#include "function.h"

class FunctionFilesIn : public Function
{
public:
	FunctionFilesIn();
	virtual ~FunctionFilesIn();
	
	virtual void execute( StringList &lInput, StringList &lOutput );

private:

};

#endif

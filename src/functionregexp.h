#ifndef FUNCTION_REGEXP_H
#define FUNCTION_REGEXP_H

#include <stdint.h>

#include "function.h"

class FunctionRegexp : public Function
{
public:
	FunctionRegexp();
	virtual ~FunctionRegexp();
	
	virtual void execute( Build *bld, const StringList &lInput, StringList &lOutput );
	virtual Function *duplicate( Build &bld, const StringList *cont, VarMap *mExtra );

private:

};

#endif

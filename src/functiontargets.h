#ifndef FUNCTION_TARGETS_H
#define FUNCTION_TARGETS_H

#include <stdint.h>

#include "function.h"

class FunctionTargets : public Function
{
public:
	FunctionTargets();
	virtual ~FunctionTargets();
	
	virtual void execute( Build *bld, const StringList &lInput, StringList &lOutput );
	virtual Function *duplicate( Build &bld, const StringList *cont, VarMap *mExtra );

private:

};

#endif

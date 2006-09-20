#ifndef FUNCTION_COMMAND_TO_LIST_H
#define FUNCTION__H

#include <stdint.h>

#include "function.h"

class FunctionCommandToList : public Function
{
public:
	FunctionCommandToList();
	virtual ~FunctionCommandToList();
	
	virtual void execute( Build *bld, const StringList &lInput, StringList &lOutput );
	virtual Function *duplicate( Build &bld, const StringList *cont, VarMap *mExtra );

private:

};

#endif

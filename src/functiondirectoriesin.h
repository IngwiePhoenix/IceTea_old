#ifndef FUNCTION_DIRECTORIES_IN_H
#define FUNCTION_DIRECTORIES_IN_H

#include <stdint.h>

#include "function.h"

class FunctionDirectoriesIn : public Function
{
public:
	FunctionDirectoriesIn();
	virtual ~FunctionDirectoriesIn();
	
	virtual void execute( Build *bld, const StringList &lInput, StringList &lOutput );
	virtual Function *duplicate( Build &bld, const StringList *cont, VarMap *mExtra );

private:

};

#endif

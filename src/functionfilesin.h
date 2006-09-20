#ifndef FUNCTION_FILES_IN_H
#define FUNCTION_FILES_IN_H

#include <stdint.h>

#include "function.h"

class FunctionFilesIn : public Function
{
public:
	FunctionFilesIn();
	virtual ~FunctionFilesIn();
	
	virtual void execute( Build *bld, const StringList &lInput, StringList &lOutput );
	virtual Function *duplicate( Build &bld, const StringList *cont, VarMap *mExtra );

private:

};

#endif

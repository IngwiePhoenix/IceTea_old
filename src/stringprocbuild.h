#ifndef STRING_PROC_BUILD_H
#define STRING_PROC_BUILD_H

#include <stdint.h>

#include "stringproc.h"

class StringProcBuild : public StringProc
{
public:
	StringProcBuild( Build *pBld );
	virtual ~StringProcBuild();

	virtual std::string replVars( const std::string &sSrc, const StringList *pCont, VarMap *mExtra );

private:

};

#endif

#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>
#include "build.h"

class Function
{
public:
	Function();
	virtual ~Function();

	void addParam( const char *str );
	virtual void execute( Build *bld, const StringList &lInput, StringList &lOutput )=0;
	virtual Function *duplicate( Build &bld, const std::string &cont, VarMap *mExtra ) = 0;
	void copyData( Function *pSrc, Build &bld, const std::string &cont, VarMap *mExtra );

protected:
	StringList lParams;

};

#endif

#include "function.h"

Function::Function()
{
}

Function::~Function()
{
}

void Function::addParam( const char *str )
{
	lParams.push_back( str );
}

void Function::copyData( Function *pSrc, Build &bld, const StringList *cont, VarMap *mExtra )
{
	lParams.clear();
	for( std::list<std::string>::iterator i = pSrc->lParams.begin();
		 i != pSrc->lParams.end(); i++ )
	{
		lParams.push_back( bld.replVars( *i, cont, mExtra ) );
	}
}


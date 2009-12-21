#include "function.h"

Function::Function() :
	pContext( NULL )
{
}

Function::~Function()
{
}

void Function::setContext( class Context *p )
{
	pContext = p;
}


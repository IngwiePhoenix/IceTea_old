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


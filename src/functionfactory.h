#ifndef FUNCTION_FACTORY_H
#define FUNCTION_FACTORY_H

#include <stdint.h>

#include "bu/plugger.h"
#include "bu/singleton.h"
#include "function.h"

class FunctionFactory : public Bu::Plugger<Function>, public Bu::Singleton<FunctionFactory>
{
public:
	FunctionFactory();
	virtual ~FunctionFactory();

private:

};

#endif

#ifndef PERFORM_FACTORY_H
#define PERFORM_FACTORY_H

#include <stdint.h>

#include "perform.h"
#include "plugger.h"
#include "singleton.h"

class PerformFactory : public Plugger<Perform>, public Singleton<PerformFactory>
{
public:
	PerformFactory();
	virtual ~PerformFactory();

private:

};

#endif

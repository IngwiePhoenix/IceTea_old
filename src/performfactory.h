#ifndef PERFORM_FACTORY_H
#define PERFORM_FACTORY_H

#include <stdint.h>

#include "perform.h"
#include "bu/plugger.h"
#include "bu/singleton.h"

class PerformFactory : public Bu::Plugger<Perform>, public Bu::Singleton<PerformFactory>
{
public:
	PerformFactory();
	virtual ~PerformFactory();

private:

};

#endif

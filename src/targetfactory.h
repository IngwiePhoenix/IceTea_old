#ifndef TARGET_FACTORY_H
#define TARGET_FACTORY_H

#include <stdint.h>

#include "plugger.h"
#include "singleton.h"
#include "target.h"

class TargetFactory : public Plugger<Target>, public Singleton<TargetFactory>
{
public:
	TargetFactory();
	virtual ~TargetFactory();

private:

};

#endif

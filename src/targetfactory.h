#ifndef TARGET_FACTORY_H
#define TARGET_FACTORY_H

#include <stdint.h>

#include "bu/plugger.h"
#include "bu/singleton.h"
#include "target.h"

class TargetFactory : public Bu::Plugger<Target>, public Bu::Singleton<TargetFactory>
{
public:
	TargetFactory();
	virtual ~TargetFactory();

private:

};

#endif

#ifndef VIEWER_FACTORY_H
#define VIEWER_FACTORY_H

#include <stdint.h>

#include "viewer.h"
#include "bu/plugger.h"
#include "bu/singleton.h"

class ViewerFactory : public Bu::Plugger<Viewer>, public Bu::Singleton<ViewerFactory>
{
public:
	ViewerFactory();
	virtual ~ViewerFactory();

private:

};

#endif

#ifndef VIEWER_FACTORY_H
#define VIEWER_FACTORY_H

#include <stdint.h>

#include "viewer.h"
#include "plugger.h"
#include "singleton.h"

class ViewerFactory : public Plugger<Viewer>, public Singleton<ViewerFactory>
{
public:
	ViewerFactory();
	virtual ~ViewerFactory();

private:

};

#endif

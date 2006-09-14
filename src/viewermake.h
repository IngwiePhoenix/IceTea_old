#ifndef VIEWER_MAKE_H
#define VIEWER_MAKE_H

#include <stdint.h>

#include "viewer.h"

class ViewerMake : public Viewer
{
public:
	ViewerMake();
	virtual ~ViewerMake();

	virtual void executeCmd( const std::string &sCmd );

private:

};

#endif

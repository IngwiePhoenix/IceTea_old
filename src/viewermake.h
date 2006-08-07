#ifndef VIEWER_MAKE_H
#define VIEWER_MAKE_H

#include <stdint.h>

#include "viewer.h"
#include "staticstring.h"

class ViewerMake : public Viewer
{
public:
	ViewerMake();
	virtual ~ViewerMake();

	virtual void beginTarget( const char *sName, const char *sType, const char *sOperation, int nPerforms );
	virtual void endTarget();

	virtual void beginPerform( Perform *pPerf );
	virtual void beginExtraRequiresCheck( const char *sCommand );
	void printHead();
	virtual void beginExecute();
	virtual void executeCmd( const char *sCmd );

private:

};

#endif

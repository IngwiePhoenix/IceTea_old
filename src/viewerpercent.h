#ifndef VIEWER_PERCENT_H
#define VIEWER_PERCENT_H

#include <stdint.h>

#include "viewer.h"
#include "staticstring.h"

class ViewerPercent : public Viewer
{
public:
	ViewerPercent();
	virtual ~ViewerPercent();

	virtual void beginTarget( const char *sName, const char *sType, const char *sOperation, int nPerforms );
	virtual void endTarget();

	virtual void beginPerform( Perform *pPerf );
	virtual void beginExecute();
	virtual void endPerform();

private:
	class StaticString sTarget;
	int nLastLen;
	int nWidth;
	int nMax;
	int nCount;
};

#endif

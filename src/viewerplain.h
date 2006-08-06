#ifndef VIEWER_PLAIN_H
#define VIEWER_PLAIN_H

#include <stdint.h>

#include "viewer.h"
#include "staticstring.h"

class ViewerPlain : public Viewer
{
public:
	ViewerPlain();
	virtual ~ViewerPlain();

	virtual void beginTarget( const char *sName, const char *sType, const char *sOperation, int nPerforms );
	virtual void endTarget();

	virtual void beginPerform( Perform *pPerf );
	virtual void beginExtraRequiresCheck( const char *sCommand );
	void printHead();
	virtual void beginExecute();
	virtual void executeCmd( const char *sCmd );

private:
	class StaticString sAction;
	bool bPrinted;
	class StaticString sTarget;

};

#endif

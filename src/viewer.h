#ifndef VIEWER_H
#define VIEWER_H

#include <stdint.h>

class Perform;

class Viewer
{
public:
	Viewer();
	virtual ~Viewer();

	virtual void beginAction( const char *sName, int nCommands );
	virtual void endAction();
	virtual void beginTarget( const char *sName, const char *sType, const char *sOperation, int nPerforms );
	virtual void endTarget();

	virtual void beginPerform( Perform *pPerf );
	virtual void beginRequiresCheck( bool bCached, const char *sName );
	virtual void endRequiresCheck();
	virtual void beginExtraRequiresCheck( const char *sCommand );
	virtual void endExtraRequiresCheck();
	virtual void beginExecute();
	virtual void executeCmd( const char *sCmd );
	virtual void endExecute();	
	virtual void endPerform();

private:

};

#endif

#ifndef VIEWER_H
#define VIEWER_H

#include <stdint.h>
#include <string>
#include "action.h"

class Perform;

class Viewer
{
public:
	Viewer();
	virtual ~Viewer();

	virtual void beginAction( const std::string &sName, int nCommands );
	virtual void endAction();

	virtual void beginCommand( Action::eAction nAct, const std::string &sTarget, int nPerforms );
	virtual void endCommand();

	virtual void beginRequiresCheck( bool bCached, const std::string &sName );
	virtual void endRequiresCheck();
	virtual void beginPerform( Perform *pPerform );
	virtual void endPerform();
	virtual void beginExecute();
	virtual void endExecute();
	virtual void executeCmd( const std::string &sCmd );

private:

};

#endif

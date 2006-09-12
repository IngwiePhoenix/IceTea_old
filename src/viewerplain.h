#ifndef VIEWER_PLAIN_H
#define VIEWER_PLAIN_H

#include <stdint.h>

#include "viewer.h"

class ViewerPlain : public Viewer
{
public:
	ViewerPlain();
	virtual ~ViewerPlain();

	virtual void beginCommand( Action::eAction nAct, const std::string &sTarget );
	virtual void endCommand();

	virtual void beginRequiresCheck( bool bCached, const std::string &sName );
	virtual void endRequiresCheck();
	virtual void beginPerform( Perform *pPerform );
	virtual void endPerform();

private:

};

#endif

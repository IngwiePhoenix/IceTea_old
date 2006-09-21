#ifndef VIEWER_PERCENT_H
#define VIEWER_PERCENT_H

#include <stdint.h>

#include "viewer.h"

class ViewerPercent : public Viewer
{
public:
	ViewerPercent();
	virtual ~ViewerPercent();

	virtual void beginCommand( Action::eAction nAct, const std::string &sTarget );
	virtual void endCommand();

	virtual void beginRequiresCheck( bool bCached, const std::string &sName );
	virtual void endRequiresCheck();
	virtual void beginPerform( Perform *pPerform );
	virtual void endPerform();
	virtual void beginPerforms( int nCount );
	virtual void endPerforms();
	virtual void beginAction( const std::string &sName, int nCommands );
	virtual void endAction();

	void printPercent( const std::string &sCur );

private:
	int nTotalCommands;
	int nCurCommand;
	int nTotalPerforms;
	int nCurPerform;
	std::string sTarget;
	int nLastLen;
	int nWidth;
	int nTermWidth;
};

#endif

#ifndef VIEWER_PLAIN_H
#define VIEWER_PLAIN_H

#include <stdint.h>
#include <list>
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

	virtual void endAction();

	void printHead();
	void indent();

private:
	bool bRunClean;
	typedef struct
	{
		int nLevel;
		bool bCmdClean;
		std::string sTarget;
	} Cmd;
	typedef std::list<Cmd> CmdStack;
	CmdStack sCmd;
};

#endif

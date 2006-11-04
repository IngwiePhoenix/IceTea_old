#ifndef VIEWER_PLAIN_H
#define VIEWER_PLAIN_H

#include <stdint.h>
#include <list>
#include "viewer.h"

class ViewerPlainPct : public Viewer
{
public:
	ViewerPlainPct();
	virtual ~ViewerPlainPct();

	virtual void beginCommand( Action::eAction nAct, const std::string &sTarget );
	virtual void endCommand();

	virtual void beginRequiresCheck( bool bCached, const std::string &sName );
	virtual void endRequiresCheck();
	virtual void beginPerform( Perform *pPerform );
	virtual void endPerform();
	virtual void beginPerforms( int nCount );

	virtual void skipRequiresCheck( bool bCached, const std::string &sName );
	virtual void skipPerform( Perform *pPerform );

	virtual void beginAction( const std::string &sName, int nCommands );

	virtual void endAction();

	void printHead();
	void indent();

private:
	//bool bDidReq;
	int iTP, iCP, iTC, iCC;
	
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

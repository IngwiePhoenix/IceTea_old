#ifndef VIEWER_PLAIN_PCT_H
#define VIEWER_PLAIN_PCT_H

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

	void printPerform(const char *sRule, const char *sTarget);
	void printHead();
	void indent();

private:
	int iTC, iCC;
	
	bool bRunClean;
	typedef struct
	{
		int nLevel;
		bool bCmdClean;
		std::string sTarget;
		int iTP, iCP;
	} Cmd;
	typedef std::list<Cmd> CmdStack;
	CmdStack sCmd;
};

#endif

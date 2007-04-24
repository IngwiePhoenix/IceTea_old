#ifndef ACTION_H
#define ACTION_H

#include <stdint.h>

#include <string>
#include <list>

class Action
{
public:
	enum eAction
	{
		actCheck,
		actClean
	};
public:
	Action();
	virtual ~Action();

	typedef struct Cmd
	{
		Cmd( eAction act, const std::string &sWhat, bool bIsGroup ) :
			act( act ), sWhat( sWhat ), bIsGroup( bIsGroup )
		{}
		eAction act;
		std::string sWhat;
		bool bIsGroup;
	} Cmd;

	void addCommand( eAction act, const std::string &sWhat, bool bIsGroup );

	void begin();
	bool isEnded();
	void next();

	int size()
	{
		return lCmds.size();
	}

	eAction getAct();
	std::string getWhat();
	bool isGroup();

	void setName( const std::string &sName )
	{
		this->sName = sName;
	}

	std::string getName()
	{
		return sName;
	}
	
	void setMode( eAction nAct );

private:
	typedef std::list<Cmd> CmdList;
	CmdList lCmds;
	CmdList::iterator i;
	std::string sName;

};

#endif

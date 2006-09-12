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
		Cmd( eAction act, const std::string &sWhat ) :
			act( act ), sWhat( sWhat )
		{}
		eAction act;
		std::string sWhat;
	} Cmd;

	void addCommand( eAction act, const std::string &sWhat );

	void begin();
	bool isEnded();
	void next();

	int size()
	{
		return lCmds.size();
	}

	eAction getAct();
	std::string getWhat();

	void setName( const std::string &sName )
	{
		this->sName = sName;
	}

	std::string getName()
	{
		return sName;
	}

private:
	typedef std::list<Cmd> CmdList;
	CmdList lCmds;
	CmdList::iterator i;
	std::string sName;

};

#endif

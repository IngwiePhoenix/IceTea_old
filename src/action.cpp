#include "action.h"

Action::Action()
{
}

Action::~Action()
{
}

void Action::addCommand( eAction act, const std::string &sWhat, bool bIsGroup )
{
	lCmds.push_back( Cmd( act, sWhat, bIsGroup ) );
}

void Action::begin()
{
	i = lCmds.begin();
}

bool Action::isEnded()
{
	return i == lCmds.end();
}

void Action::next()
{
	i++;
}

Action::eAction Action::getAct()
{
	return (*i).act;
}

std::string Action::getWhat()
{
	return (*i).sWhat;
}

bool Action::isGroup()
{
	return (*i).bIsGroup;
}

void Action::setMode( eAction nAct )
{
	for( CmdList::iterator j = lCmds.begin(); j != lCmds.end(); j++ )
	{
		(*j).act = nAct;
	}
}


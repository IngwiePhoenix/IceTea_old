#include "action.h"

Action::Action()
{
}

Action::~Action()
{
}

void Action::addCommand( eAction act, const std::string &sWhat )
{
	lCmds.push_back( Cmd( act, sWhat ) );
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


#include "action.h"
#include "command.h"
#include "builder.h"

Action::Action() :
	bDefault( true ),
	sName("")
{
}

Action::Action( const char *sName ) :
	bDefault( false ),
	sName( sName )
{
}

Action::~Action()
{
}

void Action::add( Command *pCmd )
{
	lCommand.push_back( pCmd );
}

void Action::add( int nType, const char *sCmd )
{
	lRegExCommand.push_back( std::pair<int,std::string>( nType, sCmd ) );
}

void Action::debug()
{
	if( bDefault )
		printf("   action default:\n");
	else
		printf("   action \"%s\":\n", sName.getString() );

	for( std::list<Command *>::iterator i = lCommand.begin();
		 i != lCommand.end(); i++ )
	{
		(*i)->debug();
	}
}

void Action::execute( Builder &bld )
{
	for( std::list<std::pair<int,std::string> >::iterator i =
		 lRegExCommand.begin(); i != lRegExCommand.end(); i++ )
	{
		std::list<std::string> lTmp = bld.findTargets( (*i).second.c_str() );
		for( std::list<std::string>::iterator j = lTmp.begin();
			 j != lTmp.end(); j++ )
		{
			add( new Command( (Command::CmdType)(*i).first, (*j).c_str() ) );
		}
	}
	for( std::list<Command *>::iterator i = lCommand.begin();
		 i != lCommand.end(); i++ )
	{
		(*i)->execute( bld );
	}
}


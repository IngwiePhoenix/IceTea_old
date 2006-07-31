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
	for( std::list<Command *>::iterator i = lCommand.begin();
		 i != lCommand.end(); i++ )
	{
		(*i)->execute( bld );
	}
}


#include "command.h"
#include "builder.h"
#include "target.h"

Command::Command( CmdType cmd, const char *sTarget ) :
	nType( cmd ),
	sTarget( sTarget )
{
}

Command::~Command()
{
}

void Command::debug()
{
	static const char *cmdt[]={"Check", "Clean"};
	printf("      command: %s %s\n", cmdt[ nType ], sTarget.getString() );
}

void Command::execute( Builder &bld )
{
	switch( nType )
	{
		case cmdCheck:
			bld.getTarget( sTarget )->check( bld );
			break;

		case cmdClean:
			bld.getTarget( sTarget )->clean( bld );
			break;
	}
}


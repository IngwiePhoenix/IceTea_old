#include "command.h"

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


#include "target.h"

Target::Target() :
	bRun( false )
{
}

Target::~Target()
{
}

void Target::run( Action::eAction nAct, Build &bld )
{
	bRun = true;

	switch( nAct )
	{
		case Action::actCheck:
			check( bld );
			break;

		case Action::actClean:
			clean( bld );
			break;
	}
}


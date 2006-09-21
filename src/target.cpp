#include "target.h"
#include "build.h"
#include "viewer.h"

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

	bld.getView()->beginCommand( nAct, sName );
	switch( nAct )
	{
		case Action::actCheck:
			check( bld );
			break;

		case Action::actClean:
			clean( bld );
			break;
	}
	bld.getView()->endCommand();
}


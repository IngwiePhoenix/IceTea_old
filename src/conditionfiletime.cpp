#include "conditionfiletime.h"
#include "target.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "statcache.h"

#include <bu/sio.h>
using namespace Bu;

ConditionFileTime::ConditionFileTime()
{
}

ConditionFileTime::~ConditionFileTime()
{
}

bool ConditionFileTime::shouldExec( class Runner &r, Target &rTarget )
{
	StatCache &Stat = StatCache::getInstance();
	for( StrList::const_iterator j = rTarget.getOutputList().begin(); j; j++ )
	{
		if( access( (*j).getStr(), F_OK ) )
		{
			//sio << "-- Target processed because '" << *j << "' doesn't exist."
			//	<< sio.nl;
			// Output doesn't exist
			rTarget.buildRequires( r );
			return true;
		}
	}

	time_t tOut = 0, tmp;
	for( StrList::const_iterator j = rTarget.getOutputList().begin();
		 j; j++ )
	{
		tmp = Stat.mtime( *j );
		if( tOut == 0 || tOut > tmp )
		{
			tOut = tmp;
		}
	}
	for( StrList::const_iterator j = rTarget.getInputList().begin();
		 j; j++ )
	{
		if( tOut < Stat.mtime( *j ) )
		{
			//sio << "-- Target processed because '" << *j
			//	<< "' is newer than output." << sio.nl;
			rTarget.buildRequires( r );
			return true;
		}
	}
	rTarget.gatherRequires( r );
	for( StrList::const_iterator j = rTarget.getRequiresList().begin();
		 j; j++ )
	{
		if( tOut < Stat.mtime( *j ) )
		{
			//sio << "-- Target processed because '" << *j
			//	<< "' is newer than output." << sio.nl;
			rTarget.buildRequires( r );
			return true;
		}
	}
	return false; 
}

Condition *ConditionFileTime::clone()
{
	return new ConditionFileTime();
}


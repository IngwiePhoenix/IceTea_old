#include "conditionfiletime.h"
#include "target.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
	for( StrList::const_iterator j = rTarget.getOutputList().begin(); j; j++ )
	{
		if( access( (*j).getStr(), F_OK ) )
		{
			//sio << "-- Target processed because '" << *j << "' doesn't exist."
			//	<< sio.nl;
			// Output doesn't exist
			return true;
		}
	}

	time_t tOut = 0;
	struct stat s;
	for( StrList::const_iterator j = rTarget.getOutputList().begin();
		 j; j++ )
	{
		stat( (*j).getStr(), &s );
		if( tOut == 0 || tOut > s.st_mtime )
		{
			tOut = s.st_mtime;
		}
	}
	for( StrList::const_iterator j = rTarget.getInputList().begin();
		 j; j++ )
	{
		stat( (*j).getStr(), &s );
		if( tOut < s.st_mtime )
		{
			//sio << "-- Target processed because '" << *j
			//	<< "' is newer than output." << sio.nl;
			return true;
		}
	}
	rTarget.buildRequires( r );
	for( StrList::const_iterator j = rTarget.getRequiresList().begin();
		 j; j++ )
	{
		stat( (*j).getStr(), &s );
		if( tOut < s.st_mtime )
		{
			//sio << "-- Target processed because '" << *j
			//	<< "' is newer than output." << sio.nl;
			return true;
		}
	}
	return false; 
}

Condition *ConditionFileTime::clone()
{
	return new ConditionFileTime();
}

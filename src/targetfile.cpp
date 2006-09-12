#include "targetfile.h"
#include "plugger.h"
#include "rule.h"
#include "build.h"
#include "perform.h"
#include "function.h"
#include "viewer.h"

PluginInterface2(file, TargetFile, Target, "Mike Buland", 0, 1 );

TargetFile::TargetFile()
{
}

TargetFile::~TargetFile()
{
}

void TargetFile::check( Build &bld )
{
	Rule *pRule = bld.getRule( getRule() );
	PerformList lPerf;
	pRule->setTarget( getName() );
	StringList lFinal = pRule->execute( bld, getInput(), lPerf );

	bld.getView()->beginPerforms( lPerf.size() );

	for( PerformList::iterator i = lPerf.begin(); i != lPerf.end(); i++ )
	{
		time_t tTarget = getTime( bld, (*i)->getTarget() );
		StringList &reqs = bld.getRequires( (*i)->getTarget() );
		bool bExtras = false;
		for( StringList::iterator j = reqs.begin(); j != reqs.end(); j++ )
		{
			if( getTime( bld, *j ) > tTarget )
			{
				bld.getView()->beginPerform( *i );
				(*i)->execute( bld );
				bld.getView()->endPerform();
				updateTime( (*i)->getTarget() );
				break;
			}
			if( bExtras == false )
			{
				StringList::iterator jj = j;
				jj++;
				if( jj == reqs.end() )
				{
					if( !bld.getCached( (*i)->getTarget(), tTarget, reqs ) )
					{
						bld.updateCache( (*i)->getTarget(), (*i)->getReqFuncs(), reqs );
					}
					bExtras = true;
				}
			}
		}
	}
	
	bld.getView()->endPerforms();
}

void TargetFile::clean( Build &bld )
{
	printf("Target file cleaning:  %s\n", getName().c_str() );
}

time_t TargetFile::getTime( Build &bld, std::string str )
{
	std::map<std::string, time_t>::iterator i = mTimes.find( str );
	if( i != mTimes.end() )
	{
		//nCache++;
		//bld.view().beginRequiresCheck( true, str.c_str() );
		//bld.view().endRequiresCheck();
		return (*i).second;
	}

	//bld.view().beginRequiresCheck( false, str.c_str() );
	struct stat st;
	stat( str.c_str(), &st );

	mTimes[str] = st.st_mtime;

	//nNew++;

	//bld.view().endRequiresCheck();

	return st.st_mtime;
}

void TargetFile::updateTime( std::string str )
{
	struct stat st;
	stat( str.c_str(), &st );

	mTimes[str] = st.st_mtime;
}


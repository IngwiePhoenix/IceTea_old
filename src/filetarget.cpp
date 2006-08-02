#include <errno.h>
#include <dirent.h>

#include "perform.h"
#include "rule.h"
#include "filetarget.h"
#include "builder.h" // for BuildException

FileTarget::FileTarget( const char *sName ) :
	Target( sName )
{
}

FileTarget::~FileTarget()
{
}

void FileTarget::debug()
{
	Target::debug();
	printf("      type: FileTarget\n");
}

char *gnu_getcwd()
{
	size_t size = 1024;
     
	while (1)
	{
		char *buffer = new char[size];
		if (getcwd (buffer, size) == buffer)
			return buffer;
		delete[] buffer;
		if (errno != ERANGE)
			return 0;
		size *= 2;
	}
}

void FileTarget::addInputDir( const char *sDir )
{
	DIR *dir = opendir( sDir );
	if( dir == NULL )
	{
		throw BuildException( strerror( errno ) );
	}

	char *cwd = gnu_getcwd();
	std::string base;//( cwd );
	//base += "/";
	base += sDir;
	base += "/";
	delete[] cwd;

	struct dirent *de;

	while( (de = readdir( dir )) )
	{
		if( de->d_name[0] == '.' || de->d_name[0] == '\0' )
			continue;

		std::string s( base );
		s += de->d_name;
		addInput( s.c_str() );
	}

	closedir( dir );
}

int nNew, nCache;

void FileTarget::check( Builder &bld )
{
	printf("--- %s ---\n", getName() );
	nNew = nCache = 0;
	Rule *pRule = bld.getRule( sRule );

	std::list<Perform *> perf;
	std::list<std::string> tmp = pRule->execute( bld, lInput, perf, getName() );
	lOutput.insert( lOutput.end(), tmp.begin(), tmp.end() );

	bld.processRequires( lOutput );

	for( std::list<Perform *>::iterator i = perf.begin();
		 i != perf.end(); i++ )
	{
		bool bExtraReqs = false;
		time_t target = getTime( std::string((*i)->getTarget()) );
		std::list<std::string> *lReqs = bld.getRequires( (*i)->getTarget() );
		if( lReqs == NULL )
		{
			printf("No dependancies: %s\n", (*i)->getTarget() );
			continue;
		}
		for( std::list<std::string>::iterator j = lReqs->begin();
			 j != lReqs->end(); j++ )
		{
			if( getTime( *j ) > target )
			{
				(*i)->execute( bld );
				updateTime( (*i)->getTarget() );
				break;
			}
			if( bExtraReqs == false )
			{
				std::list<std::string>::iterator k = j;
				k++;
				if( k == lReqs->end() )
				{
					bExtraReqs = true;
					bld.genRequiresFor( (*i)->getTarget() );
				}
			}
		}
	}

	printf("Cache hits %d, %d new (%f%%)\n", nCache, nNew, nCache/ (double)(nNew+nCache)*100.0 );
}

void FileTarget::clean( Builder &bld )
{
	Rule *pRule = bld.getRule( sRule );

	std::list<Perform *> perf;
	std::list<std::string> tmp = pRule->execute( bld, lInput, perf, getName() );
	lOutput.insert( lOutput.end(), tmp.begin(), tmp.end() );

	for( std::list<std::string>::iterator i = lOutput.begin();
		 i != lOutput.end(); i++ )
	{
		unlink( (*i).c_str() );
	}
}

time_t FileTarget::getTime( std::string str )
{
	std::map<std::string, time_t>::iterator i = mTimes.find( str );
	if( i != mTimes.end() )
	{
		nCache++;
		return (*i).second;
	}

	struct stat st;
	stat( str.c_str(), &st );

	mTimes[str] = st.st_mtime;

	nNew++;

	return st.st_mtime;
}

void FileTarget::updateTime( std::string str )
{
	struct stat st;
	stat( str.c_str(), &st );

	mTimes[str] = st.st_mtime;
}


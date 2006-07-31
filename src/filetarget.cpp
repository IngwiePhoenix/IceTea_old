#include <errno.h>
#include <dirent.h>

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
	std::string base( cwd );
	base += "/";
	base += sDir;
	delete[] cwd;

	struct dirent *de;

	while( (de = readdir( dir )) )
	{
		if( de->d_name[0] == '.' || de->d_name[0] == '\0' )
			continue;

		std::string s( base );
		s += "/";
		s += de->d_name;
		addInput( s.c_str() );
	}

	closedir( dir );
}

void FileTarget::check( Builder &bld )
{
	Rule *pRule = bld.getRule( sRule );

	std::list<std::string> tmp = pRule->execute( bld, lInput );
	lOutput.insert( lOutput.end(), tmp.begin(), tmp.end() );

	bld.processRequires( lInput );
}

void FileTarget::clean( Builder &bld )
{
}


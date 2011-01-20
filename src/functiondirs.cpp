#include "functiondirs.h"

#include <sys/types.h> 
#include <sys/stat.h> 
#include <glob.h>
#include <unistd.h>

#include <bu/plugger.h>
PluginInterface3( pluginFunctionDirs, dirs, FunctionDirs, Function,
		"Mike Buland", 0, 1 );

FunctionDirs::FunctionDirs()
{
}

FunctionDirs::~FunctionDirs()
{
}

Bu::String FunctionDirs::getName() const
{
	return "dirs";
}

Variable FunctionDirs::call( Variable &/*input*/, VarList lParams )
{
	glob_t globbuf;

	int flags = 0;

	for( VarList::const_iterator i = lParams.begin(); i; i++ )
	{
		switch( (*i).getType() )
		{
			case Variable::typeString:
				glob( (*i).getString().getStr(), flags, NULL, &globbuf );
				flags |= GLOB_APPEND;
				break;

			case Variable::typeList:
				throw Bu::ExceptionBase("Lists not supported in glob yet.");
				break;

			default:
				throw Bu::ExceptionBase(
					"Cannot use a non-string or non-list as a parameter to glob"
					);
				break;
		}
	}

	Variable vRet( Variable::typeList );
	struct stat s;
	for( size_t j = 0; j < globbuf.gl_pathc; j++ )
	{
		stat( globbuf.gl_pathv[j], &s );
		if( S_ISDIR( s.st_mode ) )
			vRet.append( globbuf.gl_pathv[j] );
	}

	globfree( &globbuf );

	return vRet;
}


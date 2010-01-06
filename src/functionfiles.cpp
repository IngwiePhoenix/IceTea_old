#include "functionfiles.h"

#include <sys/types.h> 
#include <sys/stat.h> 
#include <glob.h>
#include <unistd.h>

#include <bu/plugger.h>
PluginInterface3( pluginFunctionFiles, files, FunctionFiles, Function,
		"Mike Buland", 0, 1 );

FunctionFiles::FunctionFiles()
{
}

FunctionFiles::~FunctionFiles()
{
}

Bu::FString FunctionFiles::getName() const
{
	return "files";
}

Variable FunctionFiles::call( Variable &/*input*/, VarList lParams )
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
		if( S_ISREG( s.st_mode ) )
			vRet.append( globbuf.gl_pathv[j] );
	}

	globfree( &globbuf );

	return vRet;
}


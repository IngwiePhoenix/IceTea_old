#include <dirent.h>

#include "functiondirectoriesin.h"
#include "plugger.h"

PluginInterface2(directoriesIn, FunctionDirectoriesIn, Function, "Mike Buland", 0, 1 );

FunctionDirectoriesIn::FunctionDirectoriesIn()
{
}

FunctionDirectoriesIn::~FunctionDirectoriesIn()
{
}

void FunctionDirectoriesIn::execute( Build *bld, const StringList &lInput, StringList &lOutput )
{
	DIR *d = opendir( lParams.front().c_str() );
	if( d == NULL )
		throw BuildException(
			"Can't open directory %s.",
			lParams.front().c_str()
			);

	struct dirent *e;

	//std::string prefix = lParams.front() + "/";

	while( (e = readdir( d )) )
	{
		if( e->d_type == DT_DIR )
		{
			lOutput.push_back( e->d_name );
		}
	}

	closedir( d );
}

Function *FunctionDirectoriesIn::duplicate( Build &bld, const std::string &cont, VarMap *mExtra )
{
	Function *pRet = new FunctionDirectoriesIn();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}


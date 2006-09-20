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

	std::string prefix;
	if( lParams.size() >= 2 )
	{
		StringList::iterator i = lParams.begin();
		i++;
		prefix = *i;
	}
	else
	{
		prefix = lParams.front() + "/";
	}

	while( (e = readdir( d )) )
	{
		if( e->d_type == DT_DIR )
		{
			if( e->d_name[0] == '.' || e->d_name[0] == '\0' )
				continue;
			std::string sOut = prefix + e->d_name;
			lOutput.push_back( sOut );
			if( bld )
			{
				std::string sV = lParams.front() + "/";
				sV += e->d_name;
				bld->set( sOut, "fulldir", sV );
			}
			else
			{
				printf("no build access.\n");
			}
		}
	}

	closedir( d );
}

Function *FunctionDirectoriesIn::duplicate( Build &bld, const StringList *cont, VarMap *mExtra )
{
	Function *pRet = new FunctionDirectoriesIn();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}


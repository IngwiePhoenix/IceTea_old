#include <dirent.h>

#include "functionfilesin.h"
#include "plugger.h"
#include "build.h"

PluginInterface2(filesIn, FunctionFilesIn, Function, "Mike Buland", 0, 1 );

FunctionFilesIn::FunctionFilesIn()
{
}

FunctionFilesIn::~FunctionFilesIn()
{
}

void FunctionFilesIn::execute( Build *bld, const StringList &lInput, StringList &lOutput )
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
		if( e->d_type != DT_DIR )
		{
			if( e->d_name[0] != '.')
				lOutput.push_back( prefix + e->d_name );
		}
	}

	closedir( d );
}

Function *FunctionFilesIn::duplicate( Build &bld, const StringList *cont, VarMap *mExtra )
{
	Function *pRet = new FunctionFilesIn();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}


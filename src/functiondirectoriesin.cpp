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
}

Function *FunctionDirectoriesIn::duplicate( Build &bld, const std::string &cont, VarMap *mExtra )
{
	Function *pRet = new FunctionDirectoriesIn();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}


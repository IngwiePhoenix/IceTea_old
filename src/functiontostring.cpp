#include "functiontostring.h"
#include "bu/plugger.h"

PluginInterface2(toString, FunctionToString, Function, "Mike Buland", 0, 1 );

FunctionToString::FunctionToString()
{
}

FunctionToString::~FunctionToString()
{
}

void FunctionToString::execute( Build *bld, const StringList &lInput, StringList &lOutput )
{
	std::string sOut;

	for( StringList::const_iterator i = lInput.begin(); i != lInput.end(); i++ )
	{
		if( i != lInput.begin() ) sOut += " ";
		sOut += *i;
	}

	lOutput.push_back( sOut );
}

Function *FunctionToString::duplicate( Build &bld, const StringList *cont, VarMap *mExtra )
{
	Function *pRet = new FunctionToString();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}


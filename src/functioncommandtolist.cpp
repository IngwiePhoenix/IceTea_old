#include "functioncommandtolist.h"
#include "bu/plugger.h"

PluginInterface2(commandToList, FunctionCommandToList, Function, "Mike Buland", 0, 1 );

FunctionCommandToList::FunctionCommandToList()
{
}

FunctionCommandToList::~FunctionCommandToList()
{
}

void FunctionCommandToList::execute( Build *bld, const StringList &lInput, StringList &lOutput )
{
	//rView.beginExtraRequiresCheck( s.c_str() );
	//rView.executeCmd( s.c_str() );
	FILE *fcmd = popen( lParams.front().c_str(), "r" );
	std::string rhs;
	bool bHeader = true;
	for(;;)
	{
		if( feof( fcmd ) )
			break;
		int cc = fgetc( fcmd );
		if( cc == EOF )
			break;
		unsigned char c = cc;
		if( bHeader )
		{
			if( c == ':' )
				bHeader = false;
		}
		else
		{
			if( c == ' ' || c == '\t' )
			{
				if( rhs != "" )
				{
					lOutput.push_back( rhs );
					rhs = "";
				}
			}
			else
			{
				if( c == '\\' )
					c = fgetc( fcmd );
				if( c != '\n' )
					rhs += c;
			}
		}
	}
	if( rhs != "" )
	{
		lOutput.push_back( rhs );
		rhs = "";
	}
	pclose( fcmd );
	//rView.endExtraRequiresCheck();
}

Function *FunctionCommandToList::duplicate( Build &bld, const StringList *cont, VarMap *mExtra )
{
	Function *pRet = new FunctionCommandToList();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}


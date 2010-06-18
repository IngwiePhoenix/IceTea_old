#include "functiongetmakedeps.h"
#include "context.h"
#include "view.h"

#include <bu/process.h>
#include <bu/sio.h>
using namespace Bu;

#include <bu/plugger.h>
PluginInterface3( pluginFunctionGetMakeDeps, getMakeDeps, FunctionGetMakeDeps,
		Function, "Mike Buland", 0, 1 );

FunctionGetMakeDeps::FunctionGetMakeDeps()
{
}

FunctionGetMakeDeps::~FunctionGetMakeDeps()
{
}

Bu::FString FunctionGetMakeDeps::getName() const
{
	return "getMakeDeps";
}

Variable FunctionGetMakeDeps::call( Variable &/*input*/, VarList lParams )
{
	pContext->getView()->cmdStarted( lParams.first().getString().getStr() );
	Process p( Process::StdOut, "/bin/bash", "/bin/bash", "-c",
		lParams.first().getString().getStr(), NULL );

	// Gather all data from the command.
	Bu::FString sBuf;
	do
	{
		char buf[4096];
		int iRead = p.read( buf, 4096 );
		sBuf.append( buf, iRead );
	}
	while( !p.isEos() );
	
	pContext->getView()->cmdFinished( "", "", p.childExitStatus() );
	
	Variable vRet( Variable::typeList );

	Bu::FString::iterator i, j;
	i = sBuf.find(':')+2;
	while( i )
	{
		// Find whitespace at the end of the word, this one is easy, there's
		// always a space after a word
		for( j = i; j && *j != ' ' && *j != '\n' && *j != '\r'; j++ ) { }
		
		Bu::FString sTmp( i, j );
		vRet.append( sTmp );
	
		// Find the begining of the next word, trickier, we don't want to go
		// off the end, and we need to skip \ chars at the ends of lines, right
		// now this is too stupid to do that, so it may not work on windows.
		// TODO: perhaps make this only skip \ chars at the ends of lines,
		// we'll see if it matters.
		for( i = j+1;
			 i && (*i == ' ' || *i == '\\' || *i == '\n' || *i == '\r'); i++ )
		{ }
	}

	return vRet;
}


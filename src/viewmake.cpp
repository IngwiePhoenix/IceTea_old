#include "viewmake.h"
#include "target.h"

#include <bu/plugger.h>

#include <bu/sio.h>
using namespace Bu;

PluginInterface3( pluginViewMake, make, ViewMake, View,
		"Mike Buland", 0, 1 );

ViewMake::ViewMake()
{
}

ViewMake::~ViewMake()
{
}

void ViewMake::beginAction( const Bu::FString &/*sAction*/ )
{
}

void ViewMake::endAction()
{
}

void ViewMake::skipTarget( const Bu::FString &/*sProfile*/,
		const Target &/*rTarget*/ )
{
}

void ViewMake::beginTarget( const Bu::FString &/*sProfile*/,
		const Target &/*rTarget*/ )
{
}

void ViewMake::processTarget( const Bu::FString &/*sProfile*/,
		const Target &/*rTarget*/ )
{
}

void ViewMake::endTarget()
{
}

void ViewMake::buildRequires( const Target &/*rTarget*/ )
{
}

void ViewMake::cmdStarted( const Bu::FString &sCmd )
{
	sio << sCmd << sio.nl;
}

void ViewMake::cmdFinished( const Bu::FString &sStdOut,
		const Bu::FString &sStdErr, long /*iExit*/ )
{
	sio << sStdOut << sStdErr;
}

void ViewMake::userError( const Bu::FString &sMsg )
{
	sio << "Error: " << sMsg << sio.nl;
}

void ViewMake::userWarning( const Bu::FString &sMsg )
{
	sio << "Warning: " << sMsg << sio.nl;
}

void ViewMake::userNotice( const Bu::FString &sMsg )
{
	sio << "Notice: " << sMsg << sio.nl;
}

void ViewMake::sysError( const Bu::FString &sMsg )
{
	sio << sMsg << sio.nl;
}

void ViewMake::sysWarning( const Bu::FString &sMsg )
{
	sio << sMsg << sio.nl;
}


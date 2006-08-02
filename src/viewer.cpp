#include "viewer.h"

Viewer::Viewer()
{
}

Viewer::~Viewer()
{
}

void Viewer::beginAction( const char *sName, int nCommands )
{
}

void Viewer::endAction()
{
}

void Viewer::beginTarget( const char *sName, const char *sType, const char *sOperation, int nPerforms )
{
}

void Viewer::endTarget()
{
}

void Viewer::beginPerform( Perform *pPerf )
{
}

void Viewer::beginRequiresCheck( bool bCached, const char *sName )
{
}

void Viewer::endRequiresCheck()
{
}

void Viewer::beginExtraRequiresCheck( const char *sCommand )
{
}

void Viewer::endExtraRequiresCheck()
{
}

void Viewer::beginExecute()
{
}

void Viewer::executeCmd( const char *sCmd )
{
}

void Viewer::endExecute()
{
}

void Viewer::endPerform()
{
}


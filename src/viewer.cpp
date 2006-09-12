#include "viewer.h"

Viewer::Viewer()
{
}

Viewer::~Viewer()
{
}

void Viewer::beginAction( const std::string &sName, int nCommands )
{
}

void Viewer::endAction()
{
}

void Viewer::beginCommand( Action::eAction nAct, const std::string &sTarget, int nPerforms )
{
}

void Viewer::endCommand()
{
}

void Viewer::beginRequiresCheck( bool bCached, const std::string &sName )
{
}

void Viewer::endRequiresCheck()
{
}

void Viewer::beginPerform( Perform *pPerform )
{
}

void Viewer::endPerform()
{
}

void Viewer::beginExecute()
{
}

void Viewer::endExecute()
{
}

void Viewer::executeCmd( const std::string &sCmd )
{
}


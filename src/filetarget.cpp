#include "filetarget.h"

FileTarget::FileTarget( const char *sName ) :
	Target( sName )
{
}

FileTarget::~FileTarget()
{
}

void FileTarget::debug()
{
	Target::debug();
	printf("      type: FileTarget\n");
}


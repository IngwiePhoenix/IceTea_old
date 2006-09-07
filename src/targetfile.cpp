#include "targetfile.h"
#include "plugger.h"
#include "rule.h"
#include "build.h"

PluginInterface2(file, TargetFile, Target, "Mike Buland", 0, 1 );

TargetFile::TargetFile()
{
}

TargetFile::~TargetFile()
{
}

void TargetFile::check( Build &bld )
{
	printf("Target file checking:  %s\n", getName().c_str() );

	Rule *pRule = bld.getRule( getRule() );
	pRule->execute();
}

void TargetFile::clean( Build &bld )
{
	printf("Target file cleaning:  %s\n", getName().c_str() );
}


#include "functionplugger.h"

#include <sys/types.h>
#include <dirent.h>

extern Bu::PluginInfo pluginFunctionDirName;
extern Bu::PluginInfo pluginFunctionDirs;
extern Bu::PluginInfo pluginFunctionExecute;
extern Bu::PluginInfo pluginFunctionExists;
extern Bu::PluginInfo pluginFunctionFileName;
extern Bu::PluginInfo pluginFunctionFiles;
extern Bu::PluginInfo pluginFunctionGetMakeDeps;
extern Bu::PluginInfo pluginFunctionMatches;
extern Bu::PluginInfo pluginFunctionReplace;
extern Bu::PluginInfo pluginFunctionTargets;
extern Bu::PluginInfo pluginFunctionToString;
extern Bu::PluginInfo pluginFunctionUnlink;
extern Bu::PluginInfo pluginFunctionRegEx;
extern Bu::PluginInfo pluginFunctionRange;
extern Bu::PluginInfo pluginFunctionOpen;
extern Bu::PluginInfo pluginFunctionClose;
extern Bu::PluginInfo pluginFunctionRead;
extern Bu::PluginInfo pluginFunctionWrite;
extern Bu::PluginInfo pluginFunctionUnique;

FunctionPlugger::FunctionPlugger()
{
	registerBuiltinPlugin( &pluginFunctionDirName );
	registerBuiltinPlugin( &pluginFunctionDirs );
	registerBuiltinPlugin( &pluginFunctionExecute );
	registerBuiltinPlugin( &pluginFunctionExists );
	registerBuiltinPlugin( &pluginFunctionFileName );
	registerBuiltinPlugin( &pluginFunctionFiles );
	registerBuiltinPlugin( &pluginFunctionGetMakeDeps );
	registerBuiltinPlugin( &pluginFunctionMatches );
	registerBuiltinPlugin( &pluginFunctionReplace );
	registerBuiltinPlugin( &pluginFunctionTargets );
	registerBuiltinPlugin( &pluginFunctionToString );
	registerBuiltinPlugin( &pluginFunctionUnlink );
	registerBuiltinPlugin( &pluginFunctionRegEx );
	registerBuiltinPlugin( &pluginFunctionRange );
	registerBuiltinPlugin( &pluginFunctionOpen );
	registerBuiltinPlugin( &pluginFunctionClose );
	registerBuiltinPlugin( &pluginFunctionRead );
	registerBuiltinPlugin( &pluginFunctionWrite );
	registerBuiltinPlugin( &pluginFunctionUnique );

	DIR *dir = opendir("/usr/lib/build");
	if( !dir )
		return;
	struct dirent *de;
	while( (de = readdir( dir )) )
	{
		if( strncmp("pluginFunction", de->d_name, 15 ) )
			continue;

		Bu::String sFile("/usr/lib/build/");
		sFile += de->d_name;
		char *s = de->d_name;
		for(; *s && *s != '.'; s++ ) { }
		registerExternalPlugin(
			sFile,
			Bu::String( de->d_name, (ptrdiff_t)s-(ptrdiff_t)de->d_name )
			);
	}

	closedir( dir );
}

FunctionPlugger::~FunctionPlugger()
{
}


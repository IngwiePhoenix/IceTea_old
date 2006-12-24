#include "buildparser.h"
//#include "viewerplain.h"
//#include "viewerpercent.h"
//#include "viewermake.h"
#include "paramproc.h"
#include "staticstring.h"
#include "build.h"
#include "action.h"

class Param : public ParamProc
{
public:
	Param() :
		sFile("build.conf"),
		sCache(".build.cache"),
		bDebug( false ),
		bPostDebug( false ),
		sView("plainpct"),
		bInfo( false ),
		bCleanMode( false ),
		sDir("")
	{
		addHelpBanner("Build r?\n\n");
		addParam("file", 'f', &sFile, 
				"Set the input script, default: build.conf");
		addParam("info", 'i', &bInfo,
				"Display useful info about the loaded config file.", NULL, "true" );
		addParam("chdir", 'C', &sDir,
				"Change to the specified dir before doing anything else." );
		addParam("clean", 'c', &bCleanMode,
				"Clean instead of checking the given action.", NULL, "true" );
		addParam('p', mkproc(Param::procViewPercent),
				"Switch to percent view.");
		addParam('P', mkproc(Param::procViewPlain),
				"Switch to plain view.");
		addParam("color", mkproc(Param::procColorPercent),
				"Switch to colored percent view.");
		addParam('m', mkproc(Param::procViewMake),
				"Switch to 'make' style view.");
		addParam("cache", &sCache,
				"Set an alternative cache file, default: .build.cache" );
		addParam('d', &bDebug,
				"Display debug info instead of building", NULL, "true" );
		addParam('D', &bPostDebug,
				"Display debug info after building", NULL, "true" );
		addParam("help", mkproc(ParamProc::help),
				"This help");
		//pViewer = new ViewerPlain;
	}

	virtual ~Param()
	{
		//delete pViewer;
	}

	virtual int cmdParam( int argc, char *argv[] )
	{
		if( sAction > 0 )
		{
			printf("You can only specify one action per command line.\n\n");
			exit( 1 );
		}
		sAction = argv[0];
		return 1;
	}

	int procViewPercent( int argc, char *argv[] )
	{
		sView = "percent";
	}
	
	int procViewPlain( int argc, char *argv[] )
	{
		sView = "plain";
	}

	int procViewMake( int argc, char *argv[] )
	{
		sView = "make";
	}

	int procColorPercent( int argc, char *argv[] )
	{
		sView = "colorpct";
	}

	std::string sCache;
	std::string sFile;
	std::string sView;
	std::string sDir;
	StaticString sAction;
	//Viewer *pViewer;
	bool bDebug;
	bool bPostDebug;
	bool bInfo;
	bool bCleanMode;

private:
};

int main( int argc, char *argv[] )
{
	Param prm;
	prm.process( argc, argv );

	BuildParser bld;
	Build *pBuild = NULL;

	char *olddir = NULL;

	try
	{
		if( prm.sDir != "" )
		{
			olddir = new char[4096];
			getcwd( olddir, 4096 );
			chdir( prm.sDir.c_str() );
		}

		pBuild = bld.load( prm.sFile.c_str() );
		pBuild->setCache( prm.sCache );
		pBuild->setView( prm.sView );
		if( prm.bCleanMode )
			pBuild->setMode( Action::actClean );

		if( prm.bInfo )
		{
			pBuild->printInfo();
			delete pBuild;
			if( olddir ) { chdir( olddir ); delete[] olddir; }
			return 0;
		}
		if( prm.bDebug )
		{
			printf("\n\n----------\nDebug dump\n----------\n");
			pBuild->debugDump();
		}
		else
		{
			if( prm.sAction > 0 )
				pBuild->execAction( prm.sAction.getString() );
			else
				pBuild->execAction("");
		}
		if( prm.bPostDebug )
		{
			printf("\n\n----------\nDebug dump\n----------\n");
			pBuild->debugDump();
		}
	}
	catch( BuildException &e )
	{
		fputs( e.what(), stderr );
		fputs( "\n", stderr );
		if( prm.bPostDebug )
		{
			printf("\n\n----------\nDebug dump\n----------\n");
			pBuild->debugDump();
		}
		delete pBuild;
		if( olddir ) { chdir( olddir ); delete[] olddir; }
		return 1;
	}

	delete pBuild;
	if( olddir ) { chdir( olddir ); delete[] olddir; }

	return 0;
}


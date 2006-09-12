#include "buildparser.h"
//#include "viewerplain.h"
//#include "viewerpercent.h"
//#include "viewermake.h"
#include "paramproc.h"
#include "staticstring.h"
#include "build.h"

class Param : public ParamProc
{
public:
	Param() :
		sFile("build.conf"),
		sCache(".build.cache"),
		bDebug( false ),
		sView("plain")
	{
		addHelpBanner("Build r?\n\n");
		addParam("file", 'f', &sFile, 
				"Set the input script, default: build.conf");
		addParam('p', mkproc(Param::procViewPercent),
				"Switch to percent view.");
		addParam('m', mkproc(Param::procViewMake),
				"Switch to 'make' style view.");
		addParam("cache", &sCache,
				"Set an alternative cache file." );
		addParam('d', &bDebug,
				"Print out a debug dump of the build.conf", NULL, "true" );
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
		//delete pViewer;
		//pViewer = new ViewerPercent;
	}

	int procViewMake( int argc, char *argv[] )
	{
		//delete pViewer;
		//pViewer = new ViewerMake;
	}

	std::string sCache;
	std::string sFile;
	std::string sView;
	StaticString sAction;
	//Viewer *pViewer;
	bool bDebug;

private:
};

int main( int argc, char *argv[] )
{
	Param prm;
	prm.process( argc, argv );

	BuildParser bld;
	Build *pBuild;

	try
	{
		pBuild = bld.load( prm.sFile.c_str() );
		pBuild->setCache( prm.sCache );
		pBuild->setView( prm.sView );
		if( prm.bDebug )
		{
			printf("\n\n----------\nDebug dump\n----------\n");
			bld.debugDump();
		}
		else
		{
			if( prm.sAction > 0 )
				pBuild->execAction( prm.sAction.getString() );
			else
				pBuild->execAction("");
		}
	}
	catch( BuildException &e )
	{
		fputs( e.what(), stderr );
		fputs( "\n", stderr );
		return 1;
	}

	delete pBuild;
}


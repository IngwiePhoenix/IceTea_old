#include "builder.h"
#include "viewerplain.h"
#include "viewerpercent.h"
#include "paramproc.h"
#include "staticstring.h"

class Param : public ParamProc
{
public:
	Param() :
		sFile("build.conf")
	{
		addHelpBanner("Build r?\n\n");
		addParam("file", 'f', &sFile, 
				"Set the input script, default: build.conf");
		addParam('p', mkproc(Param::procViewPercent),
				"Switch to percent view.");
		addParam("help", mkproc(ParamProc::help),
				"This help");
		pViewer = new ViewerPlain;
	}

	virtual ~Param()
	{
		delete pViewer;
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
		delete pViewer;
		pViewer = new ViewerPercent;
	}

	std::string sFile;
	StaticString sAction;
	Viewer *pViewer;

private:
};

int main( int argc, char *argv[] )
{
	Param prm;
	prm.process( argc, argv );

	Builder bld( *prm.pViewer );

	bld.load( prm.sFile.c_str() );

	if( prm.sAction > 0 )
		bld.build( prm.sAction );
	else
		bld.build();
/*
	printf("\n\n----------\nDebug dump\n----------\n");
	bld.debug();*/
}


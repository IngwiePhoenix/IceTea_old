#ifndef VIEW_H
#define VIEW_H

#include <bu/fstring.h>
#include "types.h"

/**
 * Base class for all views.  A view is the only way that build is allowed to
 * communicate with the user during the processing of a buildfile, the main
 * executable may output some things for command line arguments and whatnot on
 * it's own, or debugging info, but all reports of everything happening during
 * the process is sent through a view, so it can be made pretty and usable.
 */
class View
{
public:
	View();
	virtual ~View();

	virtual void beginAction( const Bu::FString &sAction )=0;
	virtual void endAction()=0;

	virtual void skipTarget( const Bu::FString &sProfile,
			const Target &rTarget )=0;
	virtual void beginTarget( const Bu::FString &sProfile,
			const Target &rTarget )=0;
	virtual void processTarget( const Bu::FString &sProfile,
			const Target &rTarget )=0;
	virtual void endTarget()=0;

	virtual void buildRequires( const Target &rTarget )=0;
	virtual void cmdStarted( const Bu::FString &sCmd )=0;
	virtual void cmdFinished( const Bu::FString &sStdOut,
			const Bu::FString &sStdErr, long iExit )=0;

	virtual void userError( const Bu::FString &sMsg )=0;
	virtual void userWarning( const Bu::FString &sMsg )=0;
	virtual void userNotice( const Bu::FString &sMsg )=0;

	virtual void sysError( const Bu::FString &sMsg )=0;
	virtual void sysWarning( const Bu::FString &sMsg )=0;

private:
};

#endif

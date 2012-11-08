#ifndef VIEW_H
#define VIEW_H

#include <bu/string.h>
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

    virtual void beginAction( const Bu::String &sAction )=0;
    virtual void endAction()=0;

    virtual void skipTarget( const Bu::String &sProfile,
            const Target &rTarget )=0;
    virtual void beginTarget( const Bu::String &sProfile,
            const Target &rTarget )=0;
    virtual void processTarget( const Bu::String &sProfile,
            const Target &rTarget )=0;
    virtual void endTarget()=0;

    virtual void buildRequires( const Target &rTarget )=0;
    virtual void cmdStarted( const Bu::String &sCmd )=0;
    virtual void cmdFinished( const Bu::String &sStdOut,
            const Bu::String &sStdErr, long iExit )=0;

    virtual void userError( const Bu::String &sMsg )=0;
    virtual void userWarning( const Bu::String &sMsg )=0;
    virtual void userNotice( const Bu::String &sMsg )=0;

    virtual void sysError( const Bu::String &sMsg )=0;
    virtual void sysWarning( const Bu::String &sMsg )=0;

private:
};

#endif

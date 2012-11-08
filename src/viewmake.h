/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef VIEW_MAKE_H
#define VIEW_MAKE_H

#include "view.h"

class ViewMake : public View
{
public:
    ViewMake();
    virtual ~ViewMake();

    virtual void beginAction( const Bu::String &sAction );
    virtual void endAction();

    virtual void skipTarget( const Bu::String &sProfile,
            const Target &rTarget );
    virtual void beginTarget( const Bu::String &sProfile,
            const Target &rTarget );
    virtual void processTarget( const Bu::String &sProfile,
            const Target &rTarget );    
    virtual void endTarget();

    virtual void buildRequires( const Target &rTarget );
    virtual void cmdStarted( const Bu::String &sCmd );
    virtual void cmdFinished( const Bu::String &sStdOut,
            const Bu::String &sStdErr, long iExit );

    virtual void userError( const Bu::String &sMsg );
    virtual void userWarning( const Bu::String &sMsg );
    virtual void userNotice( const Bu::String &sMsg );

    virtual void sysError( const Bu::String &sMsg );
    virtual void sysWarning( const Bu::String &sMsg );
};

#endif

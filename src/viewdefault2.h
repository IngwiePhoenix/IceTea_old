/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef VIEW_DEFAULT2_H
#define VIEW_DEFAULT2_H

#include "view.h"

class ViewDefault2 : public View
{
public:
    ViewDefault2();
    virtual ~ViewDefault2();

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

    void drawTargetHdr( const Bu::String &sProfile, const Target &rTarget );

private:
    bool bFirst;
    bool bDisped;
    bool bDispedTrg;
    int iDepth;
    int iTotal;
    int iCurrent;
    Bu::String sCurProfile;
    Bu::String sTopTarget;
};

#endif

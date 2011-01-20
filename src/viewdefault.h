#ifndef VIEW_DEFAULT_H
#define VIEW_DEFAULT_H

#include "view.h"

class ViewDefault : public View
{
public:
	ViewDefault();
	virtual ~ViewDefault();

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

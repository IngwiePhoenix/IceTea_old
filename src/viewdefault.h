#ifndef VIEW_DEFAULT_H
#define VIEW_DEFAULT_H

#include "view.h"

class ViewDefault : public View
{
public:
	ViewDefault();
	virtual ~ViewDefault();

	virtual void beginAction( const Bu::FString &sAction );
	virtual void endAction();

	virtual void skipTarget( const Bu::FString &sProfile,
			const Target &rTarget );
	virtual void beginTarget( const Bu::FString &sProfile,
			const Target &rTarget );
	virtual void processTarget( const Bu::FString &sProfile,
			const Target &rTarget );	
	virtual void endTarget();

	virtual void buildRequires( const Target &rTarget );
	virtual void cmdStarted( const Bu::FString &sCmd );
	virtual void cmdFinished( const Bu::FString &sStdOut,
			const Bu::FString &sStdErr, long iExit );

	virtual void userError( const Bu::FString &sMsg );
	virtual void userWarning( const Bu::FString &sMsg );
	virtual void userNotice( const Bu::FString &sMsg );

	virtual void sysError( const Bu::FString &sMsg );
	virtual void sysWarning( const Bu::FString &sMsg );

private:
	bool bFirst;
	int iDepth;
	int iTotal;
	int iCurrent;
};

#endif

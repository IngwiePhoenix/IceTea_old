#include "target.h"
#include "variable.h"
#include "condition.h"
#include "astleaf.h"
#include "astbranch.h"
#include "runner.h"
#include "context.h"
#include "profile.h"
#include "view.h"
#include "cache.h"

#include <bu/membuf.h>
#include <bu/formatter.h>
#include <bu/heap.h>

#include <bu/sio.h>
using namespace Bu;

Target::Target( bool bExplicit ) :
	bExplicit( bExplicit ),
	bRun( false ),
	iDepCount( 0 )
{
}

Target::Target( const Bu::FString &sOutput, bool bExplicit ) :
	bExplicit( bExplicit ),
	lsOutput( sOutput ),
	iDepCount( 0 )
{
}

Target::~Target()
{
}

void Target::addInput( const Bu::FString &sInput )
{
	lsInput.append( sInput );
}

const StrList &Target::getInputList() const
{
	return lsInput;
}

void Target::resetInputList( const StrList &lInputs )
{
	lsInput = lInputs;
	if( lsInput.getSize() == 1 )
	{
		hVars.insert("INPUT", lsInput.first() );
	}
	else
	{
		Variable vInput( Variable::typeList );
		for( StrList::iterator i = lsInput.begin(); i; i++ )
		{
			vInput.append( Variable( *i ) );
		}
		hVars.insert("INPUT", vInput );
	}
}

void Target::addRequires( const Bu::FString &sReq )
{
	lsRequires.append( sReq );
}

void Target::addRequires( const AstBranch *pBr )
{
	lbRequires.append( pBr );
}

const StrList &Target::getRequiresList() const
{
	return lsRequires;
}

void Target::gatherRequires( Runner &r )
{
	Cache &c = Cache::getInstance();
	try
	{
		lsRequires = c.getRequires( lsOutput.first() );
	}
	catch( Bu::HashException &e )
	{
		buildRequires( r );
	}
}

void Target::buildRequires( Runner &r )
{
	r.getContext().getView()->buildRequires( *this );
	r.getContext().pushScope( hVars );
	for( AstBranchList::iterator i = lbRequires.begin(); i; i++ )
	{
		Variable v = r.execExpr( (*(*i)->getBranchBegin()).begin() );
		if( v.getType() == Variable::typeList )
		{
			for( VarList::iterator j = v.begin(); j; j++ )
			{
				Bu::FString sReq = (*j).toString();
				addRequires( sReq );
			/*	try
				{
					addDep( r.getContext().getTarget( sReq ) );
				}
				catch(...) { }*/
			}
		}
		else
		{
			Bu::FString sReq = v.toString();
			addRequires( sReq );
		/*	try
			{
				addDep( r.getContext().getTarget( sReq ) );
			}
			catch(...) { }*/
		}
	}
	r.getContext().popScope();

	Cache &c = Cache::getInstance();
	c.setRequires( lsOutput.first(), lsRequires );
}

void Target::addOutput( const Bu::FString &sOutput )
{
	lsOutput.append( sOutput );
}

const StrList &Target::getOutputList() const
{
	return lsOutput;
}

void Target::setPrefix( const Bu::FString &sPrefix )
{
	this->sPrefix = sPrefix;
}

const Bu::FString &Target::getPrefix() const
{
	return sPrefix;
}

void Target::setRule( const Bu::FString &sRule )
{
	this->sRule = sRule;
}

const Bu::FString &Target::getRule() const
{
	return sRule;
}

bool Target::hasRule() const
{
	return !sRule.isEmpty();
}

bool Target::isExplicit() const
{
	return bExplicit;
}

void Target::addDep( Target *pDep )
{
	lDeps.append( pDep );
}

const TargetList &Target::getDepList() const
{
	return lDeps;
}

void Target::addProfile( const class AstBranch *pProfRoot )
{
	Profile *pProf = new Profile( pProfRoot );
	hProfiles.insert( pProf->getName(), pProf );
	/*
	hProfiles.insert(
		dynamic_cast<const AstLeaf *>(
			(*pProfRoot->getBranchBegin()).first()
			)->getStrValue(),
		pProfRoot
		);
		*/
}

void Target::addProfile( const class Profile *pSrc )
{
	hProfiles.insert( pSrc->getName(), new Profile( *pSrc ) );
}

bool Target::hasProfile( const Bu::FString &sName ) const
{
	return hProfiles.has( sName );
}

const Profile *Target::getProfile( const Bu::FString &sName ) const
{
	return hProfiles.get( sName );
}

void Target::setVars( const VarHash &hNewVars )
{

	hVars = hNewVars;
}

const VarHash &Target::getVars() const
{
	return hVars;
}

void Target::setDisplay( const Bu::FString &sNewDisplay )
{
	if( !sDisplay )
		sDisplay = sNewDisplay;
}

const Bu::FString &Target::getDisplay() const
{
	return sDisplay;
}

void Target::process( class Runner &r, const Bu::FString &sProfile )
{
	r.getContext().getView()->beginTarget( sProfile, *this );
	bRun = true;
	bool bShouldExec = false;

	for( TargetList::iterator i = lDeps.begin(); i; i++ )
	{
		if( (*i)->bRun )
			continue;

		// TODO:  This is important, in the future, it may be possible for a
		// target to be triggered by multiple dependant targets, to cover for
		// this the below mergeUnder should be *TEMPORARY* and the target
		// that was marged to be reset post processing.
		(*i)->mergeUnder( hVars );
		(*i)->process( r, sProfile );
	}
	try
	{
		bShouldExec = hProfiles.get( sProfile )->shouldExec( r, *this );
	}
	catch( Bu::HashException &e )
	{
	}
	
	if( !bShouldExec )
	{
		r.getContext().getView()->skipTarget( sProfile, *this );
	}
	else
	{
		r.getContext().getView()->processTarget( sProfile, *this );
		r.execProfile( this, sProfile );
	}
	
	r.getContext().getView()->endTarget();
}

void Target::mergeUnder( const VarHash &hNewVars )
{
	for( VarHash::const_iterator i = hNewVars.begin(); i; i++ )
	{
		if( !hVars.has( i.getKey() ) )
		{
			hVars.insert( i.getKey(), i.getValue() );
		}
	}
}

bool Target::hasRun()
{
	return bRun;
}

void Target::mergeUnder( const Target *pSrc )
{
	// If either are explicit, then it's explicit
	bExplicit = bExplicit || pSrc->bExplicit;

	merge( lsInput, pSrc->lsInput );
	merge( lsRequires, pSrc->lsRequires );
	merge( lsOutput, pSrc->lsOutput );

	if( !sPrefix )
		sPrefix = pSrc->sPrefix;

	sRule = pSrc->sRule;

	mergeUnder( pSrc->hVars );

	// Deps?  They should be computed much after merging anyway, peh!
	
	for( ProfileHash::const_iterator i = pSrc->hProfiles.begin(); i; i++ )
	{
		if( !hProfiles.has( i.getKey() ) )
		{
			hProfiles.insert( i.getKey(), i.getValue() );
		}
	}

	if( !sDisplay )
		sDisplay = pSrc->sDisplay;

	// Now we need to reset our vars.
	hVars.insert("INPUT", lsInput );
	hVars.insert("REQUIRES", lsRequires );
	hVars.insert("OUTPUT", lsOutput );
}

void Target::merge( StrList &lOut, const StrList &lIn )
{
	Bu::Heap<Bu::FString> hStr;
	for( StrList::const_iterator i = lOut.begin(); i; i++ )
	{
		hStr.enqueue( *i );
	}
	for( StrList::const_iterator i = lIn.begin(); i; i++ )
	{
		hStr.enqueue( *i );
	}

	lOut.clear();

	if( hStr.isEmpty() )
		return;

	lOut.append( hStr.dequeue() );
	while( !hStr.isEmpty() )
	{
		if( hStr.peek() == lOut.last() )
		{
			hStr.dequeue();
		}
		else
		{
			lOut.append( hStr.dequeue() );
		}
	}
}

void Target::resetRun( bool bHasRun )
{
	bRun = bHasRun;

	for( TargetList::iterator i = lDeps.begin(); i; i++ )
	{
		(*i)->resetRun( bHasRun );
	}
}

void Target::setDepCount()
{
	bRun = true;
	iDepCount = 1;
	for( TargetList::iterator i = lDeps.begin(); i; i++ )
	{
		if( (*i)->bRun )
		{
			continue;
		}
		(*i)->setDepCount();
		iDepCount += (*i)->getDepCount();
	}
}

int Target::getDepCount() const
{
	return iDepCount;
}

void Target::collapseDeps()
{
	if( lDeps.getSize() <= 1 )
		return;
	Bu::Hash<ptrdiff_t, bool> hDep;
	for( TargetList::iterator i = lDeps.begin(); i; i++ )
	{
		if( hDep.has( (ptrdiff_t)*i ) )
		{
			lDeps.erase( i );
			i--;
		}
		else
		{
			hDep.insert( (ptrdiff_t)*i, true );
		}
	}
}

Bu::Formatter &operator<<( Bu::Formatter &f, const Target &t )
{
	f.incIndent();
	f << f.nl << "Input = " << t.lsInput << "," << f.nl
		<< "Requires = " << t.lsRequires << "," << f.nl
		<< "Output = \"" << t.lsOutput << "\"," << f.nl
		<< "Prefix = \"" << t.sPrefix << "\"," << f.nl
		<< "Rule = \"" << t.sRule << "\"," << f.nl
		<< "Explicit = " << t.bExplicit << "," << f.nl
		<< "Vars = " << t.hVars
		<< f.nl;
	f.decIndent();
	return f;
}

template<> Bu::Formatter &Bu::operator<< <Target>( Bu::Formatter &f, const Target *t )
{
	return f << (*t);
}


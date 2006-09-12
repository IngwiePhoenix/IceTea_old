#include "rule.h"
#include "build.h"
#include "function.h"
#include "perform.h"

Rule::Rule() :
	pAggregate( NULL )
{
}

Rule::~Rule()
{
}

StringList Rule::execute( Build &bld, StringList &lInput, PerformList &lPerf )
{
	StringList lOutput;

	RuleList rl = bld.findChainRules( this );

	for( RuleList::iterator i = rl.begin(); i != rl.end(); i++ )
	{
		StringList tmp = (*i)->execute( bld, lInput, lPerf );
		lOutput.insert( lOutput.end(), tmp.begin(), tmp.end() );
	}

	StringList lMine;
	for( FunctionList::iterator i = lMatches.begin(); i != lMatches.end(); i++ )
	{
		(*i)->execute( &bld, lInput, lMine );
	}

	StringList lTmp;
	for( FunctionList::iterator i = lFilter.begin(); i != lFilter.end(); i++ )
	{
		(*i)->execute( &bld, lMine, lTmp );
		lMine.swap( lTmp );
		lTmp.clear();
	}

	bool bHasProduces = true;
	if( lProduces.empty() )
	{
		bHasProduces = false;
		lProduces.push_back( sTarget );
	}

	StringList lNewOut;
	if( pAggregate )
	{
		VarMap mTmp;
		std::string target = lProduces.front();//, lProduces.front(), NULL ).c_str();
		mTmp["target"] = target;
		lNewOut.push_back( target );

		for( StringList::iterator i = lMine.begin(); i != lMine.end(); i++ )
		{
			bld.addRequires( target, (*i) );
		}
		for( StringList::iterator i = lReqStrs.begin();
			 i != lReqStrs.end(); i++ )
		{
			bld.addRequires( target, (*i) );
		}

		StringList lTmp;
		pAggregate->execute( &bld, lMine, lTmp );
		mTmp["match"] = lTmp.front();

		for( PerformList::iterator k = lPerform.begin();
			 k != lPerform.end(); k++ )
		{
			Perform *p = (*k)->duplicate( bld, target, &mTmp );
			p->setTarget( target );
			p->setRule( sName );
			//p->setReqFuncs( &lReqFuncs );
			lPerf.push_back( p );
		}
	}
	else
	{
		for( StringList::iterator i = lMine.begin(); i != lMine.end(); i++ )
		{
			for( StringList::iterator j = lProduces.begin();
				 j != lProduces.end(); j++ )
			{
				VarMap mTmp;
				std::string target = bld.replVars( (*j), (*i), NULL );
				mTmp["target"] = target;
				lNewOut.push_back( target );
				mTmp["match"] = (*i);
				bld.addRequires( target, (*i) );
				for( StringList::iterator k = lReqStrs.begin();
					 k != lReqStrs.end(); k++ )
				{
					bld.addRequires( target, (*k) );
				}
				for( PerformList::iterator k = lPerform.begin();
					 k != lPerform.end(); k++ )
				{
					Perform *p = (*k)->duplicate( bld, (*i), &mTmp );
					p->setTarget( target );
					p->setRule( sName );
					for( FunctionList::iterator f = lReqFuncs.begin();
						 f != lReqFuncs.end(); f++ )
					{
						p->getReqFuncs().push_back(
							(*f)->duplicate( bld, (*i), &mTmp )
							);
					}
					lPerf.push_back( p );
				}
			}
		}
	}

	lInput.insert( lInput.end(), lNewOut.begin(), lNewOut.end() );
	lOutput.insert( lOutput.end(), lNewOut.begin(), lNewOut.end() );

	if( bHasProduces == false )
	{
		lProduces.clear();
	}

	return lOutput;
}


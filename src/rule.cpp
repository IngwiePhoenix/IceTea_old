#include "rule.h"
#include "build.h"
#include "function.h"
#include "perform.h"

#include <set>

Rule::Rule() :
	pAggregate( NULL )
{
}

Rule::~Rule()
{
}

StringList Rule::execute( Build &bld, StringList &lInput, PerformList &lPerf, bool bFirstOnly )
{
	StringList lOutput;

	RuleList rl = bld.findChainRules( this );

	for( RuleList::iterator i = rl.begin(); i != rl.end(); i++ )
	{
		(*i)->setTarget( sTarget );
		StringList tmp = (*i)->execute( bld, lInput, lPerf, bFirstOnly );
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

	{
		std::set<std::string> sUsed;
		for( StringList::iterator i = lInput.begin(); i != lInput.end(); i++ )
		{
isuck:		if( i == lInput.end() ) break;
			if( sUsed.find( *i ) != sUsed.end() )
			{
				StringList::iterator j = i;
				j++;
				lInput.erase( i );
				i = j;
				goto isuck;
			}
			sUsed.insert( *i );
		}
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
			StringList cont;
			cont.push_front( target );
			Perform *p = (*k)->duplicate( bld, &cont, &mTmp );
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
			//StringList::iterator j = lProduces.begin();
			{
				VarMap mTmp;
				StringList cont;
				cont.push_front( sTarget );
				cont.push_front( (*i) );
				std::string target = bld.replVars( (*j), &cont, NULL );
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
					StringList cont2;
					cont2.push_front( sTarget );
					cont2.push_front( (*i) );
					cont2.push_front( target );
					Perform *p = (*k)->duplicate( bld, &cont2, &mTmp );
					p->setTarget( target );
					p->setRule( sName );
					for( FunctionList::iterator f = lReqFuncs.begin();
						 f != lReqFuncs.end(); f++ )
					{
						p->getReqFuncs().push_back(
							(*f)->duplicate( bld, &cont2, &mTmp )
							);
					}
					lPerf.push_back( p );
				}
				if( bFirstOnly )
					break;
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


#include "astleaf.h"

AstLeaf::AstLeaf( const Location &loc, Type eType ) :
	AstNode( loc, eType ),
	sVal( NULL )
{
}

AstLeaf::AstLeaf( const Location &loc, Type eType, int iNew ) :
	AstNode( loc, eType ),
	sVal( NULL )
{
	setIntValue( iNew );
}

AstLeaf::AstLeaf( const Location &loc, Type eType, float fNew ) :
	AstNode( loc, eType ),
	sVal( NULL )
{
	setFloatValue( fNew );
}

AstLeaf::AstLeaf( const Location &loc, Type eType, bool bNew ) :
	AstNode( loc, eType ),
	sVal( NULL )
{
	setBoolValue( bNew );
}

AstLeaf::AstLeaf( const Location &loc, Type eType, const Bu::String &sNew ) :
	AstNode( loc, eType ),
	sVal( NULL )
{
	setStrValue( sNew );
}

AstLeaf::AstLeaf( const Location &loc, Type eType, const char *sNew ) :
	AstNode( loc, eType ),
	sVal( NULL )
{
	setStrValue( sNew );
}

AstLeaf::~AstLeaf()
{
	if( getDataType() == typeDataString )
		delete sVal;
}

void AstLeaf::setIntValue( int iNew )
{
	if( getDataType() != typeDataInt )
		throw Bu::ExceptionBase("Type is not int.");
	iVal = iNew;
}

void AstLeaf::setFloatValue( float fNew )
{
	if( getDataType() != typeDataFloat )
		throw Bu::ExceptionBase("Type is not float.");
	fVal = fNew;
}

void AstLeaf::setBoolValue( bool bNew )
{
	if( getDataType() != typeDataBool )
		throw Bu::ExceptionBase("Type is not bool.");
	bVal = bNew;
}

void AstLeaf::setStrValue( const Bu::String &sNew )
{
	if( getDataType() != typeDataString )
		throw Bu::ExceptionBase("Type is not string.");
	if( sVal == NULL )
		sVal = new Bu::String( sNew );
	else
		*sVal = sNew;
}

int AstLeaf::getIntValue() const
{
	if( getDataType() != typeDataInt )
		throw Bu::ExceptionBase("Type is not int.");
	return iVal;
}

float AstLeaf::getFloatValue() const
{
	if( getDataType() != typeDataFloat )
		throw Bu::ExceptionBase("Type is not float.");
	return fVal;
}

bool AstLeaf::getBoolValue() const
{
	if( getDataType() != typeDataBool )
		throw Bu::ExceptionBase("Type is not bool.");
	return bVal;
}

Bu::String &AstLeaf::getStrValue() const
{
	if( getDataType() != typeDataString )
		throw Bu::ExceptionBase("Type is not string.");
	return *sVal;
}

Bu::Formatter &operator<<( Bu::Formatter &f, const AstLeaf &l )
{
	switch( l.getDataType() )
	{
		case AstNode::typeDataInt:
			f << ": " << l.getIntValue();
			break;

		case AstNode::typeDataFloat:
			f << ": " << l.getFloatValue();
			break;

		case AstNode::typeDataBool:
			f << ": " << l.getBoolValue();
			break;

		case AstNode::typeDataString:
			f << ": '" << l.getStrValue() << "'";
			break;

		case AstNode::typeDataNone:
			break;

		default:
			f << ": " << "!! Invalid Type !!";
	}
	return f;
}


#ifndef AST_LEAF_H
#define AST_LEAF_H

#include "astnode.h"
#include "bu/string.h"
#include "bu/formatter.h"

class AstLeaf : public AstNode
{
public:
	AstLeaf( const Location &loc, Type eType );
	AstLeaf( const Location &loc, Type eType, int iNew );
	AstLeaf( const Location &loc, Type eType, float fNew );
	AstLeaf( const Location &loc, Type eType, bool bNew );
	AstLeaf( const Location &loc, Type eType, const Bu::String &sNew );
	AstLeaf( const Location &loc, Type eType, const char *sNew );
	virtual ~AstLeaf();

	void setIntValue( int iNew );
	void setFloatValue( float fNew );
	void setBoolValue( bool bNew );
	void setStrValue( const Bu::String &sNew );

	int getIntValue() const;
	float getFloatValue() const;
	bool getBoolValue() const;
	Bu::String &getStrValue() const;

private:
	union
	{
		int iVal;
		float fVal;
		bool bVal;
		Bu::String *sVal;
	};
};

Bu::Formatter &operator<<( Bu::Formatter &f, const AstLeaf &l );

#endif

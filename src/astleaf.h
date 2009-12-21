#ifndef AST_LEAF_H
#define AST_LEAF_H

#include "astnode.h"
#include "bu/fstring.h"
#include "bu/formatter.h"

class AstLeaf : public AstNode
{
public:
	AstLeaf( Type eType );
	AstLeaf( Type eType, int iNew );
	AstLeaf( Type eType, float fNew );
	AstLeaf( Type eType, bool bNew );
	AstLeaf( Type eType, const Bu::FString &sNew );
	AstLeaf( Type eType, const char *sNew );
	virtual ~AstLeaf();

	void setIntValue( int iNew );
	void setFloatValue( float fNew );
	void setBoolValue( bool bNew );
	void setStrValue( const Bu::FString &sNew );

	int getIntValue() const;
	float getFloatValue() const;
	bool getBoolValue() const;
	Bu::FString &getStrValue() const;

private:
	union
	{
		int iVal;
		float fVal;
		bool bVal;
		Bu::FString *sVal;
	};
};

Bu::Formatter &operator<<( Bu::Formatter &f, const AstLeaf &l );

#endif

#ifndef AST_NODE_H
#define AST_NODE_H

#include "bu/formatter.h"

class AstNode
{
public:
	enum Type
	{
		// Branching types
		typeFunction		= 0x100001,
		typeSet				= 0x100002,
		typeUnset			= 0x100003,
		typeIf				= 0x100004,
		typeInclude			= 0x100005,
		typeTarget			= 0x100006,
		typeRule			= 0x100007,
		typeConfig			= 0x100008,
		typeList			= 0x100009,
		typeInlineFunc		= 0x10000A,
		typeRequires		= 0x10000B,
		typeFor				= 0x10000C,
		typeFunctionDef		= 0x10000D,
		typeReturn			= 0x10000E,
		typeProfile			= 0x10000F,
		typeInput			= 0x100010,
		typeRuleDef			= 0x100011,
		typeOutput			= 0x100012,
		typeAutoConfig		= 0x100013,
		typeGlobalConfig	= 0x100014,
		typeType			= 0x100015,
		typeValue			= 0x100016,
		typeAllow			= 0x100017,
		typeDefault			= 0x100018,
		typeExport			= 0x100019,
		typeExpr			= 0x10001A, /***< Stack based compound expression.*/
		typeActionDef		= 0x10001B,
		typeProcessTarget	= 0x10001C,
		typeTag				= 0x10001D,

		// Leaf types
		typeVariable		= 0x210001,
		typeString			= 0x210002,
		typeInt				= 0x220003,
		typeFloat			= 0x230004,
		typeBool			= 0x240005,
		typeVersion			= 0x250006,
		typeOpEq			= 0x200007,
		typeOpPlusEq		= 0x200008,
		typeOpPlusEqRaw		= 0x200009,
		typeError			= 0x21000A,
		typeWarning			= 0x21000B,
		typeNotice			= 0x21000C,
		typeTypeString		= 0x20000D,
		typeTypeInt			= 0x20000E,
		typeTypeFloat		= 0x20000F,
		typeTypeBool		= 0x200010,
		typeTypeVersion		= 0x200011,
		typeCmpEq			= 0x200012,
		typeCmpLt			= 0x200013,
		typeCmpGt			= 0x200014,
		typeCmpNe			= 0x200015,
		typeCmpLtEq			= 0x200016,
		typeCmpGtEq			= 0x200017,
		typeCondition		= 0x210018,
		typeDisplay			= 0x210019,
		typeCache			= 0x24001A,
		typePushPrefix		= 0x21001B,
		typePopPrefix		= 0x20001C,
		typeNull			= 0x20001D,
		typeVariableRef		= 0x21001E,
		typeOpPlus			= 0x20001F,
		typeOpMinus			= 0x200020,
		typeOpMultiply		= 0x200021,
		typeOpDivide		= 0x200022,
		typeOpNegate		= 0x200023,
		typeOpNot			= 0x200024,

		typeBranch			= 0x100000,
		typeLeaf			= 0x200000,
		typeClassMask		= 0x300000,

		typeDataNone		= 0x000000,
		typeDataString		= 0x010000,
		typeDataInt			= 0x020000,
		typeDataFloat		= 0x030000,
		typeDataBool		= 0x040000,
		typeDataVersion		= 0x050000,

		typeDataMask		= 0x0F0000
	};
public:
	AstNode( Type eType );
	virtual ~AstNode();

	Type getType() const { return eType; }
	Type getClass() const { return (Type)(eType&typeClassMask); }
	Type getDataType() const { return (Type)(eType&typeDataMask); }

private:
	Type eType;
};

Bu::Formatter &operator<<( Bu::Formatter &f, const AstNode &n );
Bu::Formatter &operator<<( Bu::Formatter &f, const AstNode::Type &t );

#endif

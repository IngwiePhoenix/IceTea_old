/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */
#include "astnode.h"
#include "astleaf.h"
#include "astbranch.h"

AstNode::AstNode( const Location &loc, Type eType ) :
    eType( eType ),
    loc( loc )
{
}

AstNode::~AstNode()
{
}

Bu::Formatter &operator<<( Bu::Formatter &f, const AstNode &n )
{
    f << n.getType();
    if( n.getClass() == AstNode::typeBranch )
    {
        f << *dynamic_cast<const AstBranch *>(&n);
    }
    else
    {
        f << *dynamic_cast<const AstLeaf *>(&n);
    }
    return f;
}

Bu::Formatter &operator<<( Bu::Formatter &f, const AstNode::Type &t )
{
    switch( t )
    {
        case AstNode::typeFunction:     f << "Function";        break;
        case AstNode::typeSet:          f << "Set";             break;
        case AstNode::typeUnset:        f << "Unset";           break;
        case AstNode::typeIf:           f << "If";              break;
        case AstNode::typeInclude:      f << "Include";         break;
        case AstNode::typeTarget:       f << "Target";          break;
        case AstNode::typeRule:         f << "Rule";            break;
        case AstNode::typeConfig:       f << "Config";          break;
        case AstNode::typeList:         f << "List";            break;
        case AstNode::typeInlineFunc:   f << "InlineFunc";      break;
        case AstNode::typeRequires:     f << "Requires";        break;
        case AstNode::typeFor:          f << "For";             break;
        case AstNode::typeFunctionDef:  f << "FunctionDef";     break;
        case AstNode::typeReturn:       f << "Return";          break;
        case AstNode::typeProfile:      f << "Profile";         break;
        case AstNode::typeInput:        f << "Input";           break;
        case AstNode::typeRuleDef:      f << "RuleDef";         break;
        case AstNode::typeOutput:       f << "Output";          break;
        case AstNode::typeAutoConfig:   f << "AutoConfig";      break;
        case AstNode::typeGlobalConfig: f << "GlobalConfig";    break;
        case AstNode::typeType:         f << "Type";            break;
        case AstNode::typeValue:        f << "Value";           break;
        case AstNode::typeAllow:        f << "Allow";           break;  
        case AstNode::typeDefault:      f << "Default";         break;
        case AstNode::typeExport:       f << "Export";          break;
        case AstNode::typeExpr:         f << "Expr";            break;
        case AstNode::typeActionDef:    f << "ActionDef";       break;
        case AstNode::typeProcessTarget:f << "ProcessTarget";   break;
        case AstNode::typeTag:          f << "Tag";             break;
        
        case AstNode::typeVariable:     f << "Variable";        break;
        case AstNode::typeString:       f << "String";          break;
        case AstNode::typeInt:          f << "Int";             break;
        case AstNode::typeFloat:        f << "Float";           break;
        case AstNode::typeBool:         f << "Bool";            break;
        case AstNode::typeVersion:      f << "Version";         break;
        case AstNode::typeOpEq:         f << "Operator =";      break;
        case AstNode::typeOpPlusEq:     f << "Operator +=";     break;
        case AstNode::typeOpPlusEqRaw:  f << "Operator <<";     break;
        case AstNode::typeError:        f << "Error";           break;
        case AstNode::typeWarning:      f << "Warning";         break;
        case AstNode::typeNotice:       f << "Notice";          break;
        case AstNode::typeTypeString:   f << "Type String";     break;
        case AstNode::typeTypeInt:      f << "Type Int";        break;
        case AstNode::typeTypeFloat:    f << "Type Float";      break;
        case AstNode::typeTypeBool:     f << "Type Bool";       break;
        case AstNode::typeTypeVersion:  f << "Type Version";    break;
        case AstNode::typeCmpEq:        f << "Compare =";       break;
        case AstNode::typeCmpLt:        f << "Compare <";       break;
        case AstNode::typeCmpGt:        f << "Compare >";       break;
        case AstNode::typeCmpNe:        f << "Compare !=";      break;
        case AstNode::typeCmpLtEq:      f << "Compare <=";      break;
        case AstNode::typeCmpGtEq:      f << "Compare >=";      break;
        case AstNode::typeCondition:    f << "Condition";       break;
        case AstNode::typeDisplay:      f << "Display";         break;
        case AstNode::typeCache:        f << "Cache";           break;
        case AstNode::typePushPrefix:   f << "Push Prefix";     break;
        case AstNode::typePopPrefix:    f << "Pop Prefix";      break;
        case AstNode::typeNull:         f << "Null";            break;
        case AstNode::typeVariableRef:  f << "VariableRef";     break;
        case AstNode::typeOpPlus:       f << "Operator +";      break;
        case AstNode::typeOpMinus:      f << "Operator -";      break;
        case AstNode::typeOpMultiply:   f << "Operator *";      break;
        case AstNode::typeOpDivide:     f << "Operator /";      break;
        case AstNode::typeOpNegate:     f << "Operator negate"; break;
        case AstNode::typeOpNot:        f << "Operator not";    break;

        case AstNode::typeBranch:       f << "Branch";          break;
        case AstNode::typeLeaf:         f << "Leaf";            break;
        case AstNode::typeClassMask:    f << "ClassMask";       break;

        case AstNode::typeDataNone:     f << "<Data None>";     break;
        case AstNode::typeDataString:   f << "<Data String>";   break;
        case AstNode::typeDataInt:      f << "<Data Int>";      break;
        case AstNode::typeDataFloat:    f << "<Data Float>";    break;
        case AstNode::typeDataBool:     f << "<Data Bool>";     break;
        case AstNode::typeDataVersion:  f << "<Data Version>";  break;
        case AstNode::typeDataMask:     f << "<Data Mask>";     break;
    }
    return f;
}


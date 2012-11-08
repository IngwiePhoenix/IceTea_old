/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

%defines
%{  /* Prologue -- decls and stuff */
  #include "buildparser.h"
  #include "ast.h"
void yyerror( YYLTYPE *locp, yyscan_t yyscanner, BuildParser &bld, const char *msg );
%}
/* Bison declerations */

%parse-param { yyscan_t yyscanner }
%parse-param { BuildParser &bld }
%lex-param { yyscan_t yyscanner }
%lex-param { BuildParser &bld }
%pure-parser

%locations

%debug
%error-verbose
%name-prefix="build_"

%union {
	int iVal;
	char *sVal;
	float fVal;
	bool bVal;
}

%token <sVal>	LTR_STRING		"string literal"
%token <sVal>	LTR_KEYWORD		"keyword"
%token <sVal>	LTR_CONDITION	"condition term"
%token <sVal>	LTR_VARIABLE	"variable name"
%token <sVal>	LTR_FUNCTION	"function name"
%token <sVal>	LTR_UNDEF		"undefined identifier"
%token <sVal>	LTR_PROFILE		"profile execute"
%token <iVal>	LTR_INT			"integer literal"
%token <fVal>	LTR_FLOAT		"floating point literal"
%token <bVal>	LTR_BOOL		"boolean literal"

%token TOK_TARGET			"target"
%token TOK_INPUT			"input"
%token TOK_OUTPUT			"output"
%token TOK_UNSET			"unset"
%token TOK_SET				"set"
%token TOK_CONDITION		"condition"
%token TOK_REQUIRES			"requires"
%token TOK_AUTO				"auto"
%token TOK_CONFIG			"config"
%token TOK_DISPLAY			"display"
%token TOK_TYPE				"type"
%token TOK_INT				"int"
%token TOK_FLOAT			"float"
%token TOK_BOOL				"boolean"
%token TOK_VERSION			"version"
%token TOK_STRING			"string"
%token TOK_DEFAULT			"default"
%token TOK_ALLOW			"allow"
%token TOK_RULE				"rule"
%token TOK_ACTION			"action"
%token TOK_PROFILE			"profile"
%token TOK_IF				"if"
%token TOK_THEN				"then"
%token TOK_ELSE				"else"
%token TOK_INCLUDE			"include"
%token TOK_ERROR			"error"
%token TOK_WARNING			"warning"
%token TOK_NOTICE			"notice"
%token TOK_CACHE			"cache"
%token TOK_ALWAYS			"always"
%token TOK_NEVER			"never"
%token TOK_GLOBAL			"global"
%token TOK_LOCAL			"local"
%token TOK_FOR				"for"
%token TOK_IN				"in"
%token TOK_DO				"do"
%token TOK_RETURN			"return"
%token TOK_FUNCTION			"function"
%token TOK_CONTINUE			"continue"
%token TOK_BREAK			"break"
%token TOK_VALUE			"value"
%token TOK_ALL				"all"
%token TOK_EXPORT			"export"
%token TOK_TAG				"tag"
%token TOK_NULL				"null"

%token OP_ADDSETP			"+="
%token OP_ADDSETR			"<<"
%token OP_CMPEQUAL			"=="
%token OP_INEQUAL			"!="
%token OP_LTEQUAL			"<="
%token OP_GTEQUAL			">="

%token '(' ')' '{' '}' '[' ']' ',' ';' '=' '.' '<' '>' '+' '-' '*' '/'

%right '=' OP_ADDSETP OP_ADDSETR OPADDSETR
%left OP_CMPEQUAL '<' '>' OP_INEQUAL OP_LTEQUAL OP_GTEQUAL '+' '-' '*' '/'
%left '(' ')' '{' '}' '[' ']'
%left IINEG IINOT

%destructor { delete[] $$; } LTR_STRING
%destructor { delete[] $$; } LTR_KEYWORD
%destructor { delete[] $$; } LTR_CONDITION
%destructor { delete[] $$; } LTR_VARIABLE
%destructor { delete[] $$; } LTR_FUNCTION
%destructor { delete[] $$; } LTR_UNDEF
%destructor { delete[] $$; } LTR_PROFILE

%%  /* Grammar rules */

/*
 *  root stuff
 */

root:
//	| root set
	| root line_expr
	| root unset
	| root target
	| root rule
	| root config
	| root root_if
	| root root_for
	| root include
	| root notify
	| root export
	| root function_def
	| root action_def
	;

root_sub_exprs:
//		  | root set
		  | root line_expr
		  | root unset
		  | root target
		  | root rule
		  | root config
		  | root root_if
		  | root root_for
		  | root include
		  | root notify
		  | root export
		  ;

include: TOK_INCLUDE LTR_STRING ';' { bld.include( $2, yyscanner, &yylloc ); }
	   ;

/*
 *  data related
 */

string: LTR_STRING { bld.xAst.addNode( @1, AstNode::typeString, $1 ); }
	  ;

int: LTR_INT { bld.xAst.addNode( @1, AstNode::typeInt, $1 ); }
   ;

float: LTR_FLOAT { bld.xAst.addNode( @1, AstNode::typeFloat, $1 ); }
	 ;

bool: LTR_BOOL { bld.xAst.addNode( @1, AstNode::typeBool, (bool)$1 ); }
	;

null: TOK_NULL { bld.xAst.addNode( @1, AstNode::typeNull ); }

literal: string
	   | int
	   | float
	   | bool
	   | null
	   ;

variable: LTR_UNDEF { bld.xAst.addNode( @1, AstNode::typeVariable, $1 ); }

list_core:
		 | { bld.xAst.openBranch(); } expr
		 | list_core ',' { bld.xAst.openBranch(); } expr
		 ;

list: '[' {
	  bld.xAst.addNode( @1, AstNode::typeList );
	} list_core ']' {
	  bld.xAst.closeNode();
	}
	;

value_mods:
		  | value_mods '.' function
		  ;

value_core: variable
		  | literal
		  | function_no_input
		  | list
		  ;

value: value_core value_mods
	 ;

/*
 *  misc global things
 */

notify: TOK_ERROR LTR_STRING ';' { bld.xAst.addNode( @$,AstNode::typeError, $2 ); }
	  | TOK_WARNING LTR_STRING ';' { bld.xAst.addNode( @$, AstNode::typeWarning, $2 ); }
	  | TOK_NOTICE LTR_STRING ';' { bld.xAst.addNode( @$, AstNode::typeNotice, $2 ); }
	  ;
/*
set_rhs: '=' { bld.xAst.addNode( AstNode::typeOpEq ); } value
	  | OP_ADDSETP { bld.xAst.addNode( AstNode::typeOpPlusEq ); } value
	  | OP_ADDSETR { bld.xAst.addNode( AstNode::typeOpPlusEqRaw ); } string
	  ;

set: TOK_SET {
   	bld.xAst.addNode( AstNode::typeSet );
   	bld.xAst.openBranch();
   } variable set_rhs ';' {
	bld.xAst.closeNode();
   }
   ;*/

unset: TOK_UNSET {
	 	bld.xAst.addNode( @1, AstNode::typeUnset );
		bld.xAst.openBranch();
	 } variable ';' {
		bld.xAst.closeNode();
	 }
	 ;

export_rhs: '=' value
		  |
		  ;

export: TOK_EXPORT {
	    bld.xAst.addNode( @1, AstNode::typeExport );
		bld.xAst.openBranch();
	  } variable export_rhs ';' {
	    bld.xAst.closeNode();
	  }
	  ;

func_params:
		   | func_param_list
		   ;

func_param_list: { bld.xAst.openBranch(); } expr
			   | func_param_list ',' { bld.xAst.openBranch(); } expr
			   ;

function: LTR_UNDEF '(' {
			bld.xAst.addNode( @$, AstNode::typeFunction );
			bld.xAst.openBranch();
			bld.xAst.addNode( @$, AstNode::typeString, $1 );
		} func_params ')' {
			bld.xAst.closeNode();
		}
		;

function_no_input: LTR_UNDEF '(' {
			bld.xAst.addNode( @$, AstNode::typeNull );
			bld.xAst.addNode( @$, AstNode::typeFunction );
			bld.xAst.openBranch();
			bld.xAst.addNode( @$, AstNode::typeString, $1 );
		} func_params ')' {
			bld.xAst.closeNode();
		}
		;

requires: TOK_REQUIRES {
			bld.xAst.addNode( @$, AstNode::typeRequires );
			bld.xAst.openBranch();
		} value ';' {
			bld.xAst.closeNode();
		}
		;

type: TOK_STRING { bld.xAst.addNode( @1, AstNode::typeTypeString ); }
	| TOK_INT { bld.xAst.addNode( @1, AstNode::typeTypeInt ); }
	| TOK_FLOAT { bld.xAst.addNode( @1, AstNode::typeTypeFloat ); }
	| TOK_BOOL { bld.xAst.addNode( @1, AstNode::typeTypeBool ); }
	| TOK_VERSION { bld.xAst.addNode( @1, AstNode::typeTypeVersion ); }
	;

/*
 *  comparisons
 */

expr: value
	| '(' expr ')'
	| LTR_UNDEF '=' {
		bld.xAst.addNode( @$, AstNode::typeVariableRef, $1 );
	} expr {
		bld.xAst.addNode( @3, AstNode::typeOpEq );
	}
	| LTR_UNDEF OP_ADDSETP {
		bld.xAst.addNode( @$, AstNode::typeVariableRef, $1 );
	} expr {
		bld.xAst.addNode( @3, AstNode::typeOpPlusEq );
	}
	| LTR_UNDEF OP_ADDSETR {
		bld.xAst.addNode( @$, AstNode::typeVariableRef, $1 );
	} expr {
		bld.xAst.addNode( @3, AstNode::typeOpPlusEqRaw );
	}
	| expr OP_CMPEQUAL expr
	{
		bld.xAst.addNode( @$, AstNode::typeCmpEq );
	}
	| expr '<' expr
	{
		bld.xAst.addNode( @$, AstNode::typeCmpLt );
	}
	| expr '>' expr
	{
		bld.xAst.addNode( @$, AstNode::typeCmpGt );
	}
	| expr OP_INEQUAL expr
	{
		bld.xAst.addNode( @$, AstNode::typeCmpNe );
	}
	| expr OP_LTEQUAL expr
	{
		bld.xAst.addNode( @$, AstNode::typeCmpLtEq );
	}
	| expr OP_GTEQUAL expr
	{
		bld.xAst.addNode( @$, AstNode::typeCmpGtEq );
	}
	| expr '+' expr
	{
		bld.xAst.addNode( @$, AstNode::typeOpPlus );
	}
	| expr '-' expr
	{
		bld.xAst.addNode( @$, AstNode::typeOpMinus );
	}
	| expr '*' expr
	{
		bld.xAst.addNode( @$, AstNode::typeOpMultiply );
	}
	| expr '/' expr
	{
		bld.xAst.addNode( @$, AstNode::typeOpDivide );
	}
	| '-' expr %prec IINEG
	{
		bld.xAst.addNode( @$, AstNode::typeOpNegate );
	}
	| '!' expr %prec IINOT
	{
		bld.xAst.addNode( @$, AstNode::typeOpNot );
	}
	;

line_expr: {
		 	bld.xAst.addNode( @$, AstNode::typeExpr );
			bld.xAst.openBranch();
		 } expr ';'
		 {
		 	bld.xAst.closeNode();
		 }
		 ;

if_core: TOK_IF {
	   	bld.xAst.addNode( @$, AstNode::typeIf );
		bld.xAst.openBranch();
//		bld.xAst.addNode( @$, AstNode::typeExpr );
//		bld.xAst.openBranch();
	   } expr TOK_THEN {
//	    bld.xAst.closeNode();
	   	bld.xAst.openBranch();
	   }
	   ;

else: TOK_ELSE { bld.xAst.openBranch(); }
	;

root_if: if_core '{' root_sub_exprs '}' root_else { bld.xAst.closeNode(); }
	  ;

root_else:
		 | else '{' root_sub_exprs '}'
		 | else root_if
		 ;

target_if: if_core '{' target_exprs '}' target_else { bld.xAst.closeNode(); }
		 ;

target_else:
		   | else '{' target_exprs '}'
		   | else target_if
		   ;

rule_if: if_core '{' rule_exprs '}' rule_else { bld.xAst.closeNode(); }
	   ;

rule_else:
		 | else '{' rule_exprs '}'
		 | else rule_if
		 ;

function_if: if_core '{' function_exprs '}' function_else
		   { bld.xAst.closeNode(); }
		   ;

function_else:
			 | else '{' function_exprs '}'
			 | else function_if
			 ;

/*
 *  loops
 */

for_base: TOK_FOR {
			bld.xAst.addNode( @$, AstNode::typeFor );
			bld.xAst.openBranch();
		} variable TOK_IN {
			bld.xAst.openBranch();
		} value TOK_DO {
			bld.xAst.openBranch();
		}
		;

root_for: for_base '{' root_sub_exprs '}' { bld.xAst.closeNode(); }
		;

target_for: for_base '{' target_exprs '}' { bld.xAst.closeNode(); }
		  ;

rule_for: for_base '{' rule_exprs '}' { bld.xAst.closeNode(); }
		;

function_for: for_base '{' function_exprs '}' { bld.xAst.closeNode(); }
			;

/*
 *  functions
 */

function_def: TOK_FUNCTION LTR_UNDEF {
				bld.xAst.addNode( @1, AstNode::typeFunctionDef );
				bld.xAst.openBranch();
				bld.xAst.addNode( @2, AstNode::typeString, $2 );
				bld.xAst.openBranch();
			} '(' param_defs ')' {
				bld.xAst.openBranch();
			} '{' function_exprs '}' {
				bld.xAst.closeNode();
			}
			;

param_defs:
		  | param_def_list
		  ;

param_def_list: variable
			  | param_def_list ',' variable
			  ;

function_exprs:
//			  | function_exprs function ';'
//			  | function_exprs set
			  | function_exprs unset
			  | function_exprs line_expr
			  | function_exprs export
			  | function_exprs notify
			  | function_exprs function_if
			  | function_exprs function_for
			  | function_exprs return
			  | function_exprs process_target
			  ;

return: TOK_RETURN {
	  	bld.xAst.addNode( @$, AstNode::typeReturn );
		bld.xAst.openBranch();
	  } expr {
	  	bld.xAst.closeNode();
	  } ';'
	  ;

/*
 * Actions, they're basically functions, no parameters
 */

action_def: TOK_ACTION LTR_STRING {
		    bld.xAst.addNode( @$, AstNode::typeActionDef );
			bld.xAst.openBranch();
			bld.xAst.addNode( @1, AstNode::typeString, $2 );
			bld.xAst.openBranch();
		  } '{' function_exprs '}' {
		    bld.xAst.closeNode();
		  }
		  ;

/*
 *  profiles
 */

profile: TOK_PROFILE {
	   	bld.xAst.addNode( @$, AstNode::typeProfile );
		bld.xAst.openBranch();
	   } string {
	   	bld.xAst.openBranch();
	   } '{' profile_exprs '}' {
	   	bld.xAst.closeNode();
	   } /* in-line function */
	   ;

profile_exprs:
//			 | profile_exprs function ';'
//			 | profile_exprs set
			 | profile_exprs unset
			 | profile_exprs line_expr
			 | profile_exprs export
			 | profile_exprs notify
			 | profile_exprs function_if
			 | profile_exprs function_for
			 | profile_exprs return
			 | profile_exprs process_target
			 | profile_exprs condition
			 ;
/*
 *  targets
 */

target: TOK_TARGET {
	  	bld.xAst.addNode( @$, AstNode::typeTarget );
		bld.xAst.openBranch();
	  } expr {
	  	bld.xAst.openBranch();
	  } '{' target_exprs '}' {
	  	bld.xAst.closeNode();
	  }
	  ;

target_exprs:
//			| target_exprs set
			| target_exprs unset
			| target_exprs line_expr
			| target_exprs export
			| target_exprs target_input
			| target_exprs requires
			| target_exprs profile
			| target_exprs target_if
			| target_exprs target_for
			| target_exprs notify
			| target_exprs target_rule
			| target_exprs tag
			| target_exprs display
			;

target_input: TOK_INPUT {
				bld.xAst.addNode( @$, AstNode::typeInput );
				bld.xAst.openBranch();
			} expr ';' {
				bld.xAst.closeNode();
			}
			;

target_rule: TOK_RULE {
		   	bld.xAst.addNode( @$, AstNode::typeRule );
			bld.xAst.openBranch();
		   } string ';' {
		   	bld.xAst.closeNode();
		   }
		   ;

condition: TOK_CONDITION LTR_CONDITION ';' {
		 	bld.xAst.addNode( @$, AstNode::typeCondition, $2 );
		 }
		 | TOK_CONDITION TOK_ALWAYS ';'{
		 	bld.xAst.addNode( @$, AstNode::typeCondition, "always" );
		 }
		 | TOK_CONDITION TOK_NEVER ';'{
		 	bld.xAst.addNode( @$, AstNode::typeCondition, "never" );
		 }
		 ;

/*
 *  rules
 */

rule: TOK_RULE {
		bld.xAst.addNode( @$, AstNode::typeRuleDef );
		bld.xAst.openBranch();
	} string {
		bld.xAst.openBranch();
	} '{' rule_exprs '}' {
		bld.xAst.closeNode();
	}
	;

rule_exprs:
		  | rule_exprs rule_input
		  | rule_exprs output
		  | rule_exprs requires
		  | rule_exprs profile
		  | rule_exprs rule_if
		  | rule_exprs rule_for
		  | rule_exprs notify
		  | rule_exprs display
		  | rule_exprs tag
//		  | rule_exprs set
		  ;

rule_input_func: function
			   | LTR_STRING {
			     /* In this case, when the input is just a string,
				    lets actually turn it into a call to the matches function.
					*/
			     bld.xAst.addNode( @1, AstNode::typeFunction );
			     bld.xAst.openBranch();
			     bld.xAst.addNode( @1, AstNode::typeString, "matches" );
			     bld.xAst.openBranch();
			     bld.xAst.addNode( @1, AstNode::typeString, $1 );
			     bld.xAst.closeNode();
			   }
/*			   | string */
			   ;

rule_input: TOK_INPUT {
		  	bld.xAst.addNode( @$, AstNode::typeInput );
			bld.xAst.openBranch();
		  } rule_input_func ';' {
		  	bld.xAst.closeNode();
		  }
		  ;

output: TOK_OUTPUT {
	  	bld.xAst.addNode( @$, AstNode::typeOutput );
		bld.xAst.openBranch();
	  } value ';' {
	  	bld.xAst.closeNode();
	  }
	  ;

/*
 *  config
 */
config: TOK_CONFIG {
	  	bld.xAst.addNode( @$, AstNode::typeConfig );
		bld.xAst.openBranch();
	  } string {
	  	bld.xAst.openBranch();
	  } '{' config_exprs '}' {
	  	bld.xAst.closeNode();
	  }
	  | TOK_AUTO TOK_CONFIG {
	  	bld.xAst.addNode( @$, AstNode::typeAutoConfig );
		bld.xAst.openBranch();
	  } string {
	  	bld.xAst.openBranch();
	  } '{' config_exprs '}' {
	  	bld.xAst.closeNode();
	  }
	  | TOK_GLOBAL TOK_CONFIG {
	  	bld.xAst.addNode( @$, AstNode::typeGlobalConfig );
		bld.xAst.openBranch();
	  } string {
	  	bld.xAst.openBranch();
	  } '{' config_exprs '}' {
	  	bld.xAst.closeNode();
	  }
	  ;

config_exprs:
			| config_exprs display
			| config_exprs config_type
			| config_exprs default
			| config_exprs value_key
			| config_exprs allow
			| config_exprs cache
			;

display: TOK_DISPLAY LTR_STRING ';' {
	   	bld.xAst.addNode( @$, AstNode::typeDisplay, $2 );
	   }
	   ;

config_type: TOK_TYPE {
		   	bld.xAst.addNode( @$, AstNode::typeType );
			bld.xAst.openBranch();
		   } type ';' {
		   	bld.xAst.closeNode();
		   }
		   ;

default: TOK_DEFAULT {
	   	bld.xAst.addNode( @$, AstNode::typeDefault );
		bld.xAst.openBranch();
	   } literal ';' {
	   	bld.xAst.closeNode();
	   }
	   ;

value_key_val: value ';'
			 | '{' function_exprs '}' /* inline function */

value_key: TOK_VALUE {
		 	bld.xAst.addNode( @$, AstNode::typeValue );
			bld.xAst.openBranch();
		 } value_key_val {
		 	bld.xAst.closeNode();
		 }
		 ;

allow: TOK_ALLOW {
	 	bld.xAst.addNode( @$, AstNode::typeAllow );
		bld.xAst.openBranch();
	 } value ';' {
	 	bld.xAst.closeNode();
	 }
	 ;

cache: TOK_CACHE TOK_ALWAYS ';'
	 { bld.xAst.addNode( @$, AstNode::typeCache, true ); }
	 | TOK_CACHE TOK_NEVER ';'
	 { bld.xAst.addNode( @$, AstNode::typeCache, false ); }
	 ;

/*
 * target/profile execute
 */
process_target: LTR_PROFILE
			  {
			  	bld.xAst.addNode( @$, AstNode::typeProcessTarget );
				bld.xAst.openBranch();
				bld.xAst.addNode( @$, AstNode::typeString, $1 );
				bld.xAst.openBranch();
			  } value ';' {
				bld.xAst.closeNode();
			  }
			  ;

tag: TOK_TAG
   {
   	bld.xAst.addNode( @$, AstNode::typeTag );
	bld.xAst.openBranch();
   } value ';' {
   	bld.xAst.closeNode();
   }
   ;
	

%%

/* Epilogue -- whatever you want, functions mainly */

void build_error( YYLTYPE *locp, yyscan_t, BuildParser &bld, const char *msg )
{
	bld.error(
		locp->first_line, locp->last_line,
		locp->first_column, locp->last_column,
		msg
		);
}


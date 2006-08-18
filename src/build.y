%defines
%{
# include <string>
# include "builder.h"
# include "action.h"
# include "command.h"
# include "rule.h"
# include "filetarget.h"
# include "build.tab.h"
void yyerror( YYLTYPE *locp, Builder &bld, char const *msg );
%}

%parse-param { Builder &bld }
%lex-param { Builder &bld }
%pure-parser

%locations

%debug
%error-verbose
%union {
	char *strval;
	int tval;
}

%token <strval>		STRING		"string literal"
%token <strval>		REGEXP		"regular expression"
%token <tval>		TARGETTYPE	"target type"
%token <strval>		FUNCTION	"function name"

%token TOK_ADDSET				"+="
%token TOK_DEFAULT				"default"
%token TOK_ACTION				"action"
%token TOK_CHECK				"check"
%token TOK_CLEAN				"clean"
%token TOK_RULE					"rule"
%token TOK_TARGET				"target"
%token TOK_SET					"set"
%token TOK_INPUT				"input"
%token TOK_FILTER				"filter"
%token TOK_PREFIX				"prefix"
%token TOK_REQUIRES				"requires"
%token TOK_MATCHES				"matches"
%token TOK_PERFORM				"perform"
%token TOK_PRODUCES				"produces"

%token ',' ':' '='

%destructor { delete[] $$; } STRING

%%

// General input format
input:
	 | input rule
	 | input action
	 | input target
	 ;

// Rule interpretation
rule: TOK_RULE STRING ':' rulecmds
	;

rulecmds: rulecmd
		| rulecmds ',' rulecmd
		;

rulecmd: TOK_MATCHES REGEXP
	   | TOK_PRODUCES STRING
	   | TOK_REQUIRES list
	   | TOK_INPUT TOK_FILTER REGEXP
	   | TOK_INPUT TOK_FILTER func
	   | TOK_PERFORM func
	   ;

// Action interpretation
action: TOK_DEFAULT TOK_ACTION ':' actioncmds
	  | STRING TOK_ACTION ':' actioncmds
	  ;

actioncmds: actioncmd
		  | actioncmds ',' actioncmd
		  ;

actioncmd: actioncmdtype list
		 ;

actioncmdtype: TOK_CHECK
			 | TOK_CLEAN
			 ;

// Target interpretation

target: list ':' targetcmds
	  ;

targetcmds: targetcmd
		  | targetcmds ',' targetcmd
		  ;

targetcmd: TOK_RULE STRING
		 | TOK_TARGET TOK_PREFIX STRING
		 | TOK_TARGET TARGETTYPE
		 | TOK_INPUT list
		 | TOK_REQUIRES list
		 ;

// list goo

list: listitem listfilter
	| '[' listitems ']' listfilter
	;

listfilter:
		  | TOK_FILTER REGEXP
		  | TOK_FILTER func
		  ;

listitems: listitem
		 | listitems ',' listitem
		 ;

listitem: STRING
		| func
		;

// Function

func: FUNCTION '(' funcparams ')'
	;

funcparams:
		  | STRING
		  | funcparams ',' STRING
		  ;

%%

void yyerror( YYLTYPE *locp, Builder &bld, char const *msg )
{
	bld.error( locp, msg );
}

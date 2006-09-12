%defines
%{
# include <string>
# include "buildparser.h"
# include "build.tab.h"
# include "action.h"
void yyerror( YYLTYPE *locp, BuildParser &bld, char const *msg );
%}

%parse-param { BuildParser &bld }
%lex-param { BuildParser &bld }
%pure-parser

%locations

%debug
%error-verbose
%union {
	char *strval;
}

%token <strval>		STRING		"string literal"
%token <strval>		TARGETTYPE	"target type"
%token <strval>		FUNCTION	"function name"
%token <strval>		PERFORM		"perform name"

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
%token TOK_AGGREGATE			"aggregate"

%token ',' ':' '=' '(' ')'

%destructor { delete[] $$; } STRING FUNCTION

%%

// General input format
input:
	 | input rule
	 | input action
	 | input target
	 | input set
	 ;

// Rule interpretation
rule: TOK_RULE STRING ':'
	{
		bld.addRule( $2 );
	}
	  rulecmds
	;

rulecmds: rulecmd
		| rulecmds ',' rulecmd
		;

rulecmd: TOK_MATCHES func
	   {
		   bld.addRuleMatches();
	   }
	   | TOK_PRODUCES list
	   {
		   bld.addRuleProduces();
	   }
	   | TOK_REQUIRES list
	   {
		   bld.addRuleRequires();
	   }
	   | TOK_INPUT TOK_FILTER func
	   {
		   bld.addRuleInputFilter();
	   }
	   | TOK_PERFORM perf
	   {
		   bld.addRulePerform();
	   }
	   | TOK_AGGREGATE func
	   {
		   bld.setRuleAggregate();
	   }
	   ;

// Action interpretation
action: TOK_DEFAULT TOK_ACTION ':'
	  {
		  bld.addAction();
	  }
	    actioncmds
	  | STRING TOK_ACTION ':'
	  {
		  if( $1[0] == '\0' )
			  bld.error(
			  	&yylloc,
				"You cannot use an empty string as the name of an action."
				);
		  bld.addAction( $1 );
	  }
	    actioncmds
	  ;

actioncmds: actioncmd
		  | actioncmds ',' actioncmd
		  ;

actioncmd: TOK_CHECK list
		 {
			 bld.addCommand( Action::actCheck );
		 }
		 | TOK_CLEAN list
		 {
			 bld.addCommand( Action::actClean );
		 }
		 ;

// Target interpretation
target: list ':'
	  {
		  bld.newTarget();
	  }
	    targetcmds
	  ;

targetcmds: targetcmd
		  | targetcmds ',' targetcmd
		  ;

targetcmd: TOK_RULE STRING
		 {
			 bld.setTargetRule( $2 );
		 }
		 | TOK_TARGET TOK_PREFIX STRING
		 {
			 bld.setTargetPrefix( $3 );
		 }
		 | TOK_TARGET TARGETTYPE
		 {
			 bld.setTargetType( $2 );
		 }
		 | TOK_INPUT list
		 {
			 bld.addTargetInput();
		 }
		 | TOK_REQUIRES list
		 {
			 bld.addTargetRequires();
		 }
		 | TOK_SET targetset
		 ;

targetset: STRING '=' STRING
		 {
			 bld.addTargetSet( $1, $3, setSet );
		 }
		 | STRING TOK_ADDSET STRING
		 {
			 bld.addTargetSet( $1, $3, setAdd );
		 }
		 ;

// global set
set: TOK_SET setwhat
   ;

setwhat: STRING '=' STRING
	   {
		   bld.addGlobalSet( $1, $3, setSet );
	   }
	   | STRING TOK_ADDSET STRING
	   {
		   bld.addGlobalSet( $1, $3, setAdd );
	   }
	   ;

// list goo
list: singlelistitem listfilter
	| '['
	{
		bld.newList();
	}
	  listitems ']' listfilter
	;

listfilter:
		  | TOK_FILTER func
		  {
			  bld.filterList();
		  }
		  ;

listitems: listitem
		 | listitems ',' listitem
		 ;

listitem: STRING
		{
			bld.addListString( $1 );
		}
		| func
		{
			bld.addListFunc();
		}
		;

singlelistitem: STRING
			  {
				  bld.newList();
				  bld.addListString( $1 );
			  }
			  | func
			  {
				  bld.newList();
				  bld.addListFunc();
			  }
			  ;

// Function
func: FUNCTION
	{
		bld.newFunctionCall( $1 );
	}
	  '(' funcparams ')'
	;

funcparams:
		  | STRING { bld.addFunctionParam( $1 ); }
		  | funcparams ',' STRING { bld.addFunctionParam( $3 ); }
		  ;

// Perform
perf: PERFORM
	{
		bld.newPerform( $1 );
	}
	  '(' perfparams ')'
	;

perfparams:
		  | STRING
		  {
			  bld.addPerformParam( $1 );
		  }
		  | perfparams ',' STRING
		  {
			  bld.addPerformParam( $3 );
		  }
		  ;
%%

void yyerror( YYLTYPE *locp, BuildParser &bld, char const *msg )
{
	bld.error( locp, msg );
}

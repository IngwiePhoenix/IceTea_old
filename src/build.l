%{
# include <string>
# include "builder.h"
# include "build.tab.h"
# include "stringrep.h"

std::string strbuf;
%}

%x regexp
%x strsq
%x strdq
%x comment
%option noyywrap nounput batch debug

%{
//# define YY_USER_ACTION  yylloc->columns (yyleng);
%}
%%

[,:=]								return yytext[0];
"+="								return TOK_ADDSET;

"default"							return TOK_DEFAULT;
"action"							return TOK_ACTION;
"create"							return TOK_CREATE;
"file"								return TOK_FILE;
"from"								return TOK_FROM;
"files"								return TOK_FILES;
"in"								return TOK_IN;
"using"								return TOK_USING;
"rule"								return TOK_RULE;
"requires"							return TOK_REQUIRES;
"for"								return TOK_FOR;
"set"								return TOK_SET;
"matches"							return TOK_MATCHES;
"all"								return TOK_ALL;
"one"								return TOK_ONE;
"perform"							return TOK_PERFORM;
"produces"							return TOK_PRODUCES;
"command"							return TOK_COMMAND;
"check"								return TOK_CHECK;
"clean"								return TOK_CLEAN;

"..."\n								/* elipsis line continuation */
\n+									return TOK_EOL;
[ \t\r]*							/* whitespace */

"/"									{
	BEGIN( regexp );
	strbuf = "";
}
<regexp>[^\n/]*						strbuf += yytext;
<regexp>"/"							{
	BEGIN( INITIAL );
	yylval->strval = stringdup( strbuf.c_str() );
	return REGEXP;
}

"#".*								/* single line comment */

[^ \t\r\n\'\":=,/][^ \t\r\n\'\":=,]*						{
	yylval->strval = stringdup( yytext );
	return STRING;
}

\"									{
	BEGIN( strdq );
	strbuf = "";
}
\'									{
	BEGIN( strsq );
	strbuf = "";
}

<strdq>[^\\\n\"]+					{
	strbuf += yytext;
}

<strsq>[^\\\n\']+					{
	strbuf += yytext;
}

<strdq,strsq>\\n					strbuf += "\n";
<strdq,strsq>\\t					strbuf += "\t";
<strdq,strsq>\\r					strbuf += "\r";
<strdq,strsq>\\b					strbuf += "\b";
<strdq,strsq>\\f					strbuf += "\f";

<strdq>\"							{
	BEGIN( INITIAL );
	yylval->strval = stringdup( strbuf.c_str() );
	return STRING;
}

<strsq>\'							{
	BEGIN( INITIAL );
	yylval->strval = stringdup( strbuf.c_str() );
	return STRING;
}

%%

void Builder::scanBegin()
{
	yy_flex_debug = false;
	if( !(yyin = fopen( file.c_str(), "r" )) )
		fprintf( stderr, "cannot open %s\n", file.c_str() );
}

void Builder::scanEnd()
{
	fclose( yyin );
}


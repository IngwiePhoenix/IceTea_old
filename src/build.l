/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

%{
  #include "buildparser.h"
  #include "bu/string.h"

char *fstrdup( const Bu::String &s )
{
	char *sRet = new char[s.getSize()+1];
	memcpy( sRet, s.getStr(), s.getSize()+1 );
	return sRet;
}

char *rstrdup( const char *sIn )
{
	char *sRet = new char[strlen(sIn)+1];
	strcpy( sRet, sIn );
	return sRet;
}
void build_error( YYLTYPE *locp, yyscan_t yyscanner, BuildParser &bld, const char *msg );

Bu::String sBuf;
int iStrDepth = 0;
%}

%{
  #define YY_USER_ACTION	yylloc->last_column += yyleng;
%}

%x strdq
%x BlockComment
%option noyywrap nounput batch debug bison-bridge bison-locations reentrant
%option prefix="build_"
%option outfile="src/build.yy.c"
%option header-file="src/build.yy.h"
%%
\n+									{
	yylloc->last_line += yyleng;
	yylloc->first_line = yylloc->last_line;
	yylloc->first_column = yylloc->last_column = 0;
}
[ \t\r]+							{
	yylloc->first_line = yylloc->last_line;
	yylloc->first_column = yylloc->last_column+1;
}
"#".*								/* eol comment */
"//".*								/* eol comment */

"/*"											{
	BEGIN( BlockComment );
}

<BlockComment>"*/"								{
	BEGIN( INITIAL );
}

<BlockComment>\n+								{
	yylloc->last_column = -yyleng;
	yylloc->last_line += yyleng;
}
<BlockComment>.									{	}
	/*
<BlockComment>[^*\n/]+							{	}
<BlockComment>"*"[^/\n]+						{	}
<BlockComment>"*"|"/"							{	}
	*/

[(){}[\],.;=<>!+*/-]				return yytext[0];
"+="								return OP_ADDSETP;
"<<"								return OP_ADDSETR;
"=="								return OP_CMPEQUAL;
"!="								return OP_INEQUAL;
"<="								return OP_LTEQUAL;
">="								return OP_GTEQUAL;
"target"							return TOK_TARGET;
"input"								return TOK_INPUT;
"output"							return TOK_OUTPUT;
"unset"								return TOK_UNSET;
"set"								return TOK_SET;
"condition"							return TOK_CONDITION;
"requires"							return TOK_REQUIRES;
"auto"								return TOK_AUTO;
"config"							return TOK_CONFIG;
"display"							return TOK_DISPLAY;
"type"								return TOK_TYPE;
"int"								return TOK_INT;
"float"								return TOK_FLOAT;
"bool"								return TOK_BOOL;
"version"							return TOK_VERSION;
"string"							return TOK_STRING;
"default"							return TOK_DEFAULT;
"allow"								return TOK_ALLOW;
"rule"								return TOK_RULE;
"action"							return TOK_ACTION;
"profile"							return TOK_PROFILE;
"if"								return TOK_IF;
"then"								return TOK_THEN;
"else"								return TOK_ELSE;
"include"							return TOK_INCLUDE;
"warning"							return TOK_WARNING;
"error"								return TOK_ERROR;
"notice"							return TOK_NOTICE;
"cache"								return TOK_CACHE;
"always"							return TOK_ALWAYS;
"never"								return TOK_NEVER;
"global"							return TOK_GLOBAL;
"local"								return TOK_LOCAL;
"for"								return TOK_FOR;
"in"								return TOK_IN;
"do"								return TOK_DO;
"return"							return TOK_RETURN;
"function"							return TOK_FUNCTION;
"continue"							return TOK_CONTINUE;
"break"								return TOK_BREAK;
"value"								return TOK_VALUE;
"all"								return TOK_ALL;
"export"							return TOK_EXPORT;
"tag"								return TOK_TAG;
"null"								return TOK_NULL;

"true"								{
	yylval->bVal = true;
	return LTR_BOOL;
}
"false"								{
	yylval->bVal = false;
	return LTR_BOOL;
}

[a-zA-Z_][a-zA-Z0-9_]*:				{
	yytext[yyleng-1] = '\0';
	yylval->sVal = rstrdup( yytext );
	return LTR_PROFILE;
}

[a-zA-Z_][a-zA-Z0-9_]*				{
	yylval->sVal = rstrdup( yytext );
	if( b.isKeyword( yylval->sVal ) )
		return LTR_KEYWORD;
	else if( b.isCond( yylval->sVal ) )
		return LTR_CONDITION;
	return LTR_UNDEF;
}

([1-9][0-9]*)|(0)					{
	yylval->iVal = strtol( yytext, NULL, 10 );
	return LTR_INT;
}

(0\.0+)|((([1-9][0-9]*)|(0))\.[0-9]*)		{
	yylval->fVal = strtof( yytext, NULL );
	return LTR_FLOAT;
}

\"									{
	BEGIN( strdq );
	sBuf.clear();
	iStrDepth = 0;
}
<strdq>[^\\\n\"$()]+				{
	sBuf += yytext;
}
<strdq>\$\$							{
	sBuf += "$$";
}
<strdq>\$\(							{
	iStrDepth++; // TODO:  Should this really count depth?  I dunno...
	sBuf += "$(";
}
<strdq>\\\)							sBuf += "\\)";
<strdq>\)							{
	if( iStrDepth > 0 )
		iStrDepth--;
	sBuf += ")";
}
<strdq>[$(]							{
	sBuf += yytext;
}
<strdq>\n							{
	build_error( yylloc, yyscanner, b, "newline encountered in string");
}
<strdq>\\n							sBuf += "\n";
<strdq>\\t							sBuf += "\t";
<strdq>\\r							sBuf += "\r";
<strdq>\\b							sBuf += "\b";
<strdq>\\f							sBuf += "\f";
<strdq>\\\\							sBuf += "\\";
<strdq>\\\"							sBuf += "\"";
<strdq>\\\'							sBuf += "\'";
<strdq>\\\(							sBuf += "(";
<strdq>\\\`							sBuf += "`";
<strdq>\\.							printf("Invalid escape sequence.\n");
<strdq>\"[ \t\r\n]*\"				{/* Ignore spaces between strings. */}
<strdq>\"							{
	if( iStrDepth > 0 )
	{
		sBuf += "\"";
	}
	else
	{
		BEGIN( INITIAL );
		yylval->sVal = fstrdup( sBuf );
		return LTR_STRING;
	}
}

<<EOF>>								{
	build_pop_buffer_state( yyscanner );
	if( !YY_CURRENT_BUFFER )
		yyterminate();
	else
		b.endInclude( yylloc );
}

.									{
	build_error( yylloc, yyscanner, b, "invalid character");
}
%%

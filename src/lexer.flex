	int lineNum = 1;
%%

"="|"("|")"|"{"|"}"|";"|","			return yytext[0];
"=="								return TOK_COMPARE;
"target"							return TOK_TARGET;

\n									++lineNum;
[ \t\r]*

\/\/.*
"#".*


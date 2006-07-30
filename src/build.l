	int lineNum = 1;
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

\n+									return TOX_EOL;
[ \t\r]*

\/\/.*
"#".*


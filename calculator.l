/*token recognition*/

%{
    # include "ast.h"
    # include "calculator.tab.h"
%}


%%
[-+*/=|,;(){}] 	{return yytext[0];}

">"         { yylval.fn = 1; return CMP; }/*compare operation*/
"<"         { yylval.fn = 2; return CMP; }
"<>"		{ yylval.fn = 3; return CMP; }
"!="		{ yylval.fn = 3; return CMP; }
"=="		{ yylval.fn = 4; return CMP; }
">="		{ yylval.fn = 5; return CMP; }
"<="		{ yylval.fn = 6; return CMP; }

"if"		{ return IF; }/*control flow*/
"then"		{ return THEN; }
"else"		{ return ELSE; }
"while"		{ return WHILE; }
"do"		{ return DO; }

"sqrt"		{ yylval.fn = my_sqrt; return FUNC; }/*built in functions*/
"print"		{ yylval.fn = print; return FUNC; }

[a-zA-Z][a-zA-Z0-9]*	{ yylval.s = lookup(yytext); return VAR; }/*Var. Decleration*/

[0-9]+"."[0-9]*? |
"."?[0-9]+?	{ yylval.d = atof(yytext); return NUMBER; }

[ \t]		{}
"\n"		{ return EOL; }

.		{ yyerror("Unknown Char %c\n", *yytext); }
%%

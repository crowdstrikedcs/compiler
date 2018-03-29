%{
    #	include <stdio.h>
    #	include <stdlib.h>
    #	include "ast.h"
%}

%union
{
    struct ast *a;
    double d;
    struct symbol *s;		/* which symbol */
    int fn;			        /* which function */
}

/* declare tokens */
%token <d> NUMBER
%token <s> VAR
%token <fn> FUNC
%token EOL

%token IF THEN ELSE WHILE DO

%nonassoc <fn> CMP

%type <a> expression statement block exp

%start calc

%%

statement: IF expression THEN '{' block '}'             { $$ = flow('IF', $2, $5, NULL); }
| IF expression THEN '{' block '}' ELSE '{' block '}'	{ $$ = flow('IF', $2, $5, $9); }
| WHILE expression DO '{' block '}'                     { $$ = flow('WHILE', $2, $5, NULL); }
| expression ';'
;

block:                                                  { $$ = NULL; }
| statement block
{
    if ($2 == NULL)
    {
        $$ = $1;
    }
    else
    {
        $$ = ast('L', $1, $2);
    }
}
;

expression: expression CMP expression                   { $$ = cmp($2, $1, $3); }
| expression '+' expression                             { $$ = ast('+', $1,$3); }
| expression '-' expression                             { $$ = ast('-', $1,$3);}
| expression '*' expression                             { $$ = ast('*', $1,$3); }
| expression '/' expression                             { $$ = ast('/', $1,$3); }
| '(' expression ')'                                    { $$ = $2; }
| '-' expression                                        { $$ = ast('MINUS', $2, NULL); }
| NUMBER                                                { $$ = number($1); }
| FUNC '(' exp ')'                                      { $$ = func($1, $3); }
| VAR                                                   { $$ = reference($1); }
| VAR '=' expression                                    { $$ = assignment($1, $3); }
;

exp: expression
| expression ',' exp                                { $$ = ast('L', $1, $3); }
;

calc:
| calc statement EOL
{
    printf("= %4.4g\n> ", eval($2));
}
| calc error EOL {
    yyerrok; printf("> ");
}
;
%%

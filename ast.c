#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "ast.h"

// symbol table to hash a symbol
static unsigned symhash(char *sym)
{
    unsigned int hash = 0;
    unsigned c;
    while(c = *sym++)
    {
        hash = hash*9 ^ c;
    }
    return hash;
}
struct symbol* lookup(char* sym)
{
    struct symbol *sp = &symtab[symhash(sym)%50];
    int scount = 50;
    while(--scount >= 0)
    {
        if(sp->name && !strcmp(sp->name, sym))
        {
            return sp;
        }
        if(!sp->name)
        {
            sp->name = strdup(sym);
            sp->value = 0;
            sp->func = NULL;
            return sp;
        }
        
        if(++sp >= symtab+50)
        {
            sp = symtab; // try the next entry
        }
    }
    return NULL;
}
struct ast* ast(int nodetype, struct ast *l, struct ast *r)
{
    struct ast *a = malloc(sizeof(struct ast));
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}
struct ast* number(double d)
{
    struct numval *a = malloc(sizeof(struct numval));
    a->nodetype = 'K';
    a->number = d;
    return (struct ast *)a;
}
struct ast* cmp(int cmptype, struct ast *l, struct ast *r)
{
    struct ast *a = malloc(sizeof(struct ast));
    a->nodetype = '0' + cmptype;
    a->l = l;
    a->r = r;
    return a;
}
struct ast* func(int functype, struct ast *l)
{
    struct fncall *a = malloc(sizeof(struct fncall));
    a->nodetype = 'F';
    a->l = l;
    a->functype = functype;
    return (struct ast *)a;
}
struct ast* reference(struct symbol *s)
{
    struct symref *a = malloc(sizeof(struct symref));
    a->nodetype = 'N';
    a->s = s;
    return (struct ast *)a;
}
struct ast* assignment(struct symbol *s, struct ast *v)
{
    struct symasgn *a = malloc(sizeof(struct symasgn));
    a->nodetype = '=';
    a->s = s;
    a->v = v;
    return (struct ast *)a;
}
struct ast* flow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el)
{
    struct flow *a = malloc(sizeof(struct flow));
    a->nodetype = nodetype;
    a->cond = cond;
    a->tl = tl;
    a->el = el;
    return (struct ast *)a;
}
double eval(struct ast *a)
{
    double v;
    if(!a)
    { // default value
        return 0.0;
    }
    switch(a->nodetype)
    {
        // we have a number
        case 'K': v = ((struct numval *)a)->number; break;
        // Variable Name
        case 'N': v = ((struct symref *)a)->s->value; break;
        // Variable Assignment
        case '=': v = ((struct symasgn *)a)->s->value = eval(((struct symasgn *)a)->v); break;
        // expressions
        case '+': v = eval(a->l) + eval(a->r); break;
        case '-': v = eval(a->l) - eval(a->r); break;
        case '*': v = eval(a->l) * eval(a->r); break;
        case '/': v = eval(a->l) / eval(a->r); break;
        case 'MINUS': v = -eval(a->l); break;
        // comparisons
        case '1': v = (eval(a->l) > eval(a->r))? 1 : 0; break;
        case '2': v = (eval(a->l) < eval(a->r))? 1 : 0; break;
        case '3': v = (eval(a->l) != eval(a->r))? 1 : 0; break;
        case '4': v = (eval(a->l) == eval(a->r))? 1 : 0; break;
        case '5': v = (eval(a->l) >= eval(a->r))? 1 : 0; break;
        case '6': v = (eval(a->l) <= eval(a->r))? 1 : 0; break;
        // if flow
        case 'IF':
        if( eval( ((struct flow *)a)->cond) != 0)
        {
            if( ((struct flow *)a)->tl)
            { // else branch if exists
                v = eval( ((struct flow *)a)->tl);
            }
            else
            {
                v = 0.0; // default value
            }
        }
        else
        {
            if( ((struct flow *)a)->el)
            { // if branch if exists
                v = eval(((struct flow *)a)->el);
            }
            else
            {
                v = 0.0; // default value
            }
        }
        break;
        case 'WHILE': // while flow
        v = 0.0;		/* default value */
        if( ((struct flow *)a)->tl)
        {
            while( eval(((struct flow *)a)->cond) != 0)
            { // loop
                v = eval(((struct flow *)a)->tl);
            }
        }
        break; // val. ready to return
        // Funtion List
        case 'L': eval(a->l); v = eval(a->r); break;
        // built in function
        case 'F': v = callbuiltin((struct fncall *)a); break;
        // error
        default: printf("Error could not trace.", a->nodetype);
    }
    return v;
}
static double callbuiltin(struct fncall *f)
{
    enum bifs functype = f->functype;
    double v = eval(f->l);
    switch(functype)
    {
        case my_sqrt: return sqrt(v);
        case print: printf("= %4.4g\n", v); return v;
        default: yyerror("Unknown function %d", functype); return 0.0;
    }
}
void yyerror(char *s, ...)
{
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "error: ");
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}
int yywrap()
{
    return 1;
}

int main()
{
    printf("> ");
    return yyparse();
}

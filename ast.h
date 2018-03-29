/* stores synbols and values */
struct symbol
{
    char *name;                 /* Var. name*/
    double value;               /* Value*/
    struct ast *func;		    /* stmt for the function */
    struct symlist *syms;
};

/* table of symbols */
struct symbol symtab[50];
struct symbol *lookup(char*);

enum bifs
{				/* built-in functions type */
    my_sqrt = 1,
    print
};

/* nodes in the Abstract Syntax Tree */
/* all have common initial nodetype */
/* this nodetype changes based on the op to be preformed */
struct ast
{
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct fncall
{				            /* built-in function */
    int nodetype;			/* Function */
    struct ast *l;
    enum bifs functype;
};

struct flow
{
    int nodetype;			/* If or While */
    struct ast *cond;		/* condition */
    struct ast *tl;			/* then or do */
    struct ast *el;			/* else */
};

struct numval
{
    int nodetype;			/* Number */
    double number;
};

struct symref
{
    int nodetype;			/* Symbol */
    struct symbol *s;
};

struct symasgn
{
    int nodetype;			/* Assignment */
    struct symbol *s;
    struct ast *v;			/* value */
};

struct ast *ast(int nodetype, struct ast *l, struct ast *r);
struct ast *cmp(int cmptype, struct ast *l, struct ast *r);
struct ast *func(int functype, struct ast *l);
struct ast *reference(struct symbol *s);
struct ast *assignment(struct symbol *s, struct ast *v);
struct ast *number(double d);
struct ast *flow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

static double callbuiltin(struct fncall*);
double eval(struct ast *);
void yyerror(char *s, ...);
int yywrap();

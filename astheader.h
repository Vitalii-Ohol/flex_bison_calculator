#ifndef __AST__H__
#define __AST__H__


extern int yylineno;
void yyerror(const char *s, ...);
extern int yylex();
extern unsigned int cursor;


struct AST {
    int node_type;
    struct AST *left, *right;
    double number;
};

struct ast_num {
	int node_type;
	double number;
};


//TODO: for memory optimization can divide into FUNCsym and VARsym with according ID specification (to eliminate allocation of unuzed variables in struct)
struct object {
    char *name;
    double value;
    struct AST *function;
    struct object_list *parameters;
};
struct object_list {
    struct object *object;
    struct object_list *next;
};


enum build_in_func_type{
	FN_PI,
	FN_E,
	FN_MOD,
	FN_POW,
	FN_FLOOR,
	FN_CEIL,
	FN_ABS,
	FN_FACTORIAL,
	FN_COS,
	FN_SIN,
	FN_TAN,
	FN_SINH,
	FN_COSH,
	FN_TANH,
	FN_EXP,
	FN_SQRT,
	FN_LOG2,
	FN_LOG10,
	FN_PRINT,
	FN_RETURN
};
struct ast_build_in_func_call {
    int node_type;
    struct AST *l;
    enum build_in_func_type func_type;
    int args_num;
};
struct ast_user_func_call {
    int node_type;
    struct AST *body;
    struct object *object;
};


struct ast_reference {
    int node_type;
    struct object *object;
};
struct ast_equal {
    int node_type;
    struct object *object;
    struct AST *value;
};

struct flow {
    int node_type;
    struct AST *condition;
    struct AST *body_left, *body_right;
};



struct AST *create_ast(int node_type, struct AST *left, struct AST *right);
struct AST *create_number(double number);
struct AST *create_comparison(int cmptype, struct AST *left, struct AST *right);
struct AST *create_reference(struct object *object);
struct AST *create_assign(struct object *s, struct AST *v);
struct AST *create_func(int func_type, struct AST *l);
struct AST *create_func_no_args(int func_type);
struct AST *create_call(struct object *s, struct AST *l);
struct AST *create_flow(int node_type, struct AST *condition,  struct AST *body_left,  struct AST *body_right);
struct object_list *create_object_list(struct object *object, struct object_list *next);


void make_reference(struct object *object, struct object_list *lst, struct AST *statements);
double call_built_in(struct ast_build_in_func_call *fn);
struct object *get_object(const char *s);
double call_user(struct ast_user_func_call *ufn);


double eval(struct AST *ast);
void free_ast(struct AST *node);
void free_object_list(struct object_list *s);



#endif

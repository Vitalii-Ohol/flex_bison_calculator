#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "astheader.h"
#include "func.h"

struct object_list *var_list = NULL;


struct AST *create_ast(int node_type, struct AST *left, struct AST *right) {
	struct AST *a = malloc(sizeof(struct AST));
    if(a == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
	a->node_type = node_type;
	a->left = left;
	a->right = right;
    return a;
}
struct AST *create_number(double number) {
	struct ast_num *n = malloc(sizeof(struct ast_num));
    if(n == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
	n->node_type = 'K';
	n->number = number;
    return (struct AST*)n;
}
struct AST *create_comparison(int cmptype, struct AST *left, struct AST *right) {
	struct AST *n = malloc(sizeof(struct AST));
    if(n == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    n->node_type = '0' + cmptype;
    n->left = left;
    n->right = right;
    return n;
}
struct AST *create_func(int func_type, struct AST *l) {
    struct ast_build_in_func_call *fn = malloc(sizeof(struct ast_user_func_call));
    if(fn == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    fn->node_type = 'F';
    fn->l = l;
    fn->func_type = func_type;
    fn->args_num = 1;
    return (struct AST*)fn;
}
struct AST *create_func_no_args(int func_type) {
    struct ast_build_in_func_call *fn = malloc(sizeof(struct ast_build_in_func_call));
    if(fn == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    fn->node_type = 'F';
    fn->l = 0;
    fn->func_type = func_type;
    fn->args_num = 0;
    return (struct AST*)fn;
}
struct AST *create_reference(struct object *object) {
    struct ast_reference *ref = malloc(sizeof(struct ast_reference));
    if(ref == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    ref->node_type = 'N';
    ref->object = object;
    return (struct AST*)ref;
}
struct AST *create_assign(struct object *object, struct AST *value) {
    struct ast_equal *equal = malloc(sizeof(struct ast_equal));
    if(equal == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    equal->node_type = '=';
    equal->object = object;
    equal->value = value;
    return (struct AST*)equal;
}
struct AST *create_call(struct object *object, struct AST *body) {
    struct ast_user_func_call *fn = malloc(sizeof(struct ast_user_func_call));
    if(fn == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    fn->node_type = 'C';
    fn->object = object;
    fn->body = body;
    return (struct AST*)fn;
}
struct AST *create_flow(int node_type, struct AST *condition, struct AST *body_left, struct AST *body_right) {
    struct flow *flow = malloc(sizeof(struct flow));
    if(flow == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    flow->node_type = node_type;
    flow->condition = condition;
    flow->body_left = body_left;
    flow->body_right = body_right;
    return (struct AST*)flow;
}
struct object_list *create_object_list(struct object *object, struct object_list *next) {
    struct object_list *ol = malloc(sizeof(struct object_list));
    if(ol == NULL) {
        yyerror("Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }
    ol->object = object;
    ol->next = next;
    return ol;
}


void make_reference(struct object *object, struct object_list *lst, struct AST *statements) {
	if(object == NULL) return;
    if(object->parameters != NULL){
		free_object_list(object->parameters);
		object->parameters = NULL;
	}
    if(object->function != NULL){
		free_ast(object->function);
		object->function = NULL;
	}
    object->parameters = lst;
    object->function = statements;
}
struct object *get_object(const char *s) {
	for (struct object_list *node = var_list; node != NULL; node = node->next) {
		if (strcmp(node->object->name, s) == 0) {
			return node->object;
		}
	}


	struct object *symb = malloc(sizeof(struct object));
	symb->name = strdup(s);
    struct object_list *node = malloc(sizeof(struct object_list));
    node->object = symb;
    node->next = var_list;
	var_list = node;

	return symb;
}
double call_built_in(struct ast_build_in_func_call *fn) {
	if(fn == NULL) return 0;
	enum build_in_func_type func_type = fn->func_type;
	double v = 0;
	if(fn->args_num >= 1) {
		v = eval(fn->l);
		switch(func_type) {
			case FN_RETURN:
				return v;
			case FN_PRINT:
				printf("PRINT: %.6f\n", v);
				break;
			case FN_SQRT:
				return sqrt(v);
			case FN_EXP:
				return exp(v);
			case FN_FLOOR:
				return floor(v);
			case FN_CEIL:
				return ceil(v);
			case FN_ABS:
				return abs(v);
			case FN_COS:
				return cos(v);
			case FN_SIN:
				return sin(v);
			case FN_TAN:
				return tan(v);
			case FN_SINH:
				return sinh(v);
			case FN_COSH:
				return cosh(v);
			case FN_TANH:
				return tanh(v);
			case FN_LOG2:
				return log2(v);
			case FN_LOG10:
				return log10(v);
			default:
				break;
		}
	} else {
		switch(func_type) {
			case FN_PI:
				return M_PI;
				break;
			case FN_E:
				return M_E;
			default:
				break;
		}
	}
	return 0;
}
double call_user(struct ast_user_func_call *ufn) {
	if(ufn == NULL) return 0;
	if(ufn->object == NULL) return 0;
    struct object *fn = ufn->object;

	if(ufn->body == NULL) return 0;
    struct AST *args = ufn->body;

    struct object_list *sl;
    double *oldval, *newval;
    double v;
    int nargs;
    int i;

	if(fn->parameters == NULL) return 0;
    sl = fn->parameters;
    for(nargs = 0; sl; sl = sl->next)
        nargs++;
    

    oldval = malloc(nargs * sizeof(double));
    newval = malloc(nargs * sizeof(double));
    
    if(oldval == NULL || newval == NULL) {
        yyerror("Out of space..\n");
        return 0.0;
    }
    
    for(i = 0; i < nargs; ++i) {
        if(!args) {
            yyerror("Invalid arguments..\n");
            free(oldval);
            free(newval);
            return 0;
        }
        
        if(args->node_type == 'L') {
            newval[i] = eval(args->left);
            args = args->right;
        } else {
            newval[i] = eval(args);
            args = NULL;
        }
    }
    sl = fn->parameters;
    for( i = 0; i < nargs; i++) {
        struct object *s = sl->object;
        oldval[i] = s->value;
        s->value = newval[i];
        sl = sl->next;
    }
    free(newval);

    if(fn->function == NULL) {
		free(oldval);
        yyerror("Function not found..\n");
        return 0;
    }

    v = eval(fn->function);
    
    sl = fn->parameters;
    for(i = 0; i < nargs; i++) {
        struct object *s = sl->object;
        s->value = oldval[i];
        sl = sl->next;
    }
    
    free(oldval);
    return v;
}


double eval(struct AST *ast) {
	if(ast == NULL) return 0;
	double v = 0;
	switch(ast->node_type) {
		case 'K':
			v = ((struct ast_num*)ast)->number;
			break;
		case 'N':
			v = ((struct ast_reference*)ast)->object->value;
			break;
		case '=':
			v = ((struct ast_equal*)ast)->object->value = eval( ((struct ast_equal*)ast)->value);
			break;
		case '+':
			v = eval(ast->left) + eval(ast->right);
			break;
		case '-':
			v = eval(ast->left) - eval(ast->right);
			break;
		case '*':
			v = eval(ast->left) * eval(ast->right);
			break;
		case '/':
			v = eval(ast->left) / eval(ast->right);
			break;
		case '%':
			v = fmod(eval(ast->left), eval(ast->right));
			break;
		case '^':
			v = pow(eval(ast->left), eval(ast->right));
			break;
		case '!':
			v = factorial(eval(ast->left));
			break;
		case '|':
			v = eval(ast->left);
			if(v < 0) v = -v;
			break;
		case 'M':
			v = -eval(ast->left);
			break;
		case '1':
			v = (eval(ast->left) > eval(ast->right)) ? 1 : 0;
			break;
		case '2':
			v = (eval(ast->left) < eval(ast->right)) ? 1 : 0;
			break;
		case '3':
			v = (eval(ast->left) != eval(ast->right)) ? 1 : 0;
			break;
		case '4':
			v = (eval(ast->left) == eval(ast->right)) ? 1 : 0;
			break;
		case '5':
			v = (eval(ast->left) >= eval(ast->right)) ? 1 : 0;
			break;
		case '6':
			v = (eval(ast->left) <= eval(ast->right)) ? 1 : 0;
			break;
		case 'I':
			if(eval(((struct flow *)ast)->condition) != 0) {
				if (((struct flow*)ast)->body_left != NULL)
					v = eval(((struct flow*)ast)->body_left);
				else
					v = 0.0;
			} else {
				if(((struct flow*)ast)->body_right != NULL)
					v = eval(((struct flow*)ast)->body_right);
				else
					v = 0.0;
			}
			break;
		case 'W':
			v = 0.0;
			if(((struct flow*)ast)->body_left != NULL) {
				while(eval(((struct flow*)ast)->condition) != 0) {
					v = eval(((struct flow*)ast)->body_left);
				}
			}
			break;
		case 'L':
			eval(ast->left); v = eval(ast->right);
			break;
		case 'F':
			v = call_built_in(((struct ast_build_in_func_call*)ast));
			break;
		case 'C':
			v = call_user(((struct ast_user_func_call*)ast));
			break;
		default:
			//printf("Error reached invalid node type: %d [%c] \n", ast->node_type, ast->node_type);
			//yyerror("");
			return 0;
	}
	return v;
}
void free_ast(struct AST *node) {
	if(node == NULL || node->node_type == 0) return;
	switch(node->node_type) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '^':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case 'L':
			free_ast(node->right);
			node->right = NULL;
		case 'M':
		case 'C':
		case 'F':
		case '!':
			free_ast(node->left);
			node->left = NULL;
		case 'K':
		case 'N':
			break;
		case '=': {
				struct ast_equal *equal = (struct ast_equal *)node;
				free(equal->value);
				equal->value = NULL;
			}
			break;
		case 'I':
		case 'W': {
				struct flow *w = (struct flow*)node;
				free(w->condition);
				w->condition = NULL;
				if(w->body_left != NULL){
					free_ast(w->body_left);
					w->body_left = NULL;
				}
				if(w->body_right != NULL){
					free_ast(w->body_right);
					w->body_right = NULL;
				}
			}
			break;
		default:
			//yyerror("Error invalid node");
			//printf("NODE: %c %d\n", node->node_type, node->node_type);
			return;
	}
	free(node);
}
void free_object_list(struct object_list *sl) {
    struct object_list *nsl;
    while(sl != NULL) {
        nsl = sl->next;
        free(sl);
        sl = nsl;
    }
}



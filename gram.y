%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gram.tab.h"
#include "astheader.h"

/* Flex functions */
extern int yylex(void);
extern void yyterminate();
extern FILE* yyin;

%}

%define parse.error verbose

%union {
    struct AST *a;
	double num;
	struct object *s;
	struct object_list *sl;
	int fn;
}


%nonassoc UMINUS
%token<num> NUMBER
%token<a> DIV MUL ADD SUB EQUALS MOD POW FACTORIAL
%token<fn> FUNC
%token<fn> CMP
%token<s> NAME
%token L_BRACKET R_BRACKET
%token COMMA SEMICOLON EOA EOL
%token IF_KEYWORD THEN_KEYWORD ELSE_KEYWORD
%token WHILE_KEYWORD DO_KEYWORD
%token LET_KEYWORD VAR_KEYWORD
%token END_KEYWORD

%type<a> program_input calculation flow body
%type<a> expression expression_list
%type<sl> symlist

/* Set operator precedence, follows BODMAS rules. */
%left SUB ADD
%left MUL DIV MOD
%left POW FACTORIAL
%left L_BRACKET R_BRACKET


%%
program_input:
																					/* nothing */
		| program_input EOA															{ printf("Please enter a calculation:\n"); }
		| program_input calculation error EOA										/* nothing */
		| program_input calculation EOA												{ double d = eval($2); free_ast($2); }
		| program_input NAME EOA													{ printf("variable %s == %.8f\n", $2->name, $2->value); }
		;

calculation:
		flow
		| LET_KEYWORD NAME L_BRACKET symlist R_BRACKET EQUALS body END_KEYWORD		{ make_reference($2, $4, $7); }
		;

body:
		| flow
		| flow EOA body																{ $$ = create_ast('L', $1, $3); }

flow: 
		IF_KEYWORD expression THEN_KEYWORD body END_KEYWORD							{ $$ = create_flow('I', $2, $4, NULL); }
		| IF_KEYWORD expression THEN_KEYWORD body ELSE_KEYWORD body END_KEYWORD		{ $$ = create_flow('I', $2, $4, $6); }
		| WHILE_KEYWORD expression DO_KEYWORD body END_KEYWORD						{ $$ = create_flow('W', $2, $4, NULL); }
		| VAR_KEYWORD NAME EQUALS expression										{ $$ = create_assign($2, $4); }
		| expression
		;

expression:
		error																		/* nothing */
		| expression CMP expression													{ $$ = create_comparison($2, $1, $3); }
		| expression DIV expression													{ $$ = create_ast('/', $1, $3); }
		| expression MUL expression													{ $$ = create_ast('*', $1, $3); }
		| expression ADD expression													{ $$ = create_ast('+', $1, $3); }
		| expression SUB expression													{ $$ = create_ast('-', $1, $3); }
		| expression MOD expression													{ $$ = create_ast('%', $1, $3); }
		| expression POW expression													{ $$ = create_ast('^', $1, $3); }
		| expression FACTORIAL														{ $$ = create_ast('!', $1, NULL); }
		| SUB expression %prec UMINUS												{ $$ = create_ast('M', $2, NULL); }
		| L_BRACKET expression R_BRACKET											{ $$ = $2; }
		| NUMBER																	{ $$ = create_number($1); }
		| NAME																		{ $$ = create_reference($1); }
		| FUNC L_BRACKET expression_list R_BRACKET									{ $$ = create_func($1, $3); }
		| FUNC L_BRACKET R_BRACKET													{ $$ = create_func_no_args($1); }
		| NAME L_BRACKET expression_list R_BRACKET									{ $$ = create_call($1, $3); }
		;

expression_list:
		expression																	/* nothing */
		| expression COMMA expression_list											{ $$ = create_ast('L', $1, $3); }
		;

symlist:
		NAME																		{ $$ = create_object_list($1, NULL); }
		| NAME COMMA symlist														{ $$ = create_object_list($1, $3); }
		;


%%

/* Entry point */
int main(int argc, char **argv)
{
	char c[100];
	printf("Command line or File? (Enter C or F): ");
	scanf("%1s", c);
	
	if (strcmp(c, "f")==0 || strcmp(c, "F")==0) {
		// File input
		printf("ENTER FILE NAME:  ");
		scanf("%s", c);

		yyin = fopen(c, "r");
		if (!yyin) {
			printf("ERROR: Couldn't open file %s\n", c);
			exit(EXIT_FAILURE);
		}
		yyparse();
		
		printf("All done with %s\n", c);
	}
	else {
		// Command line
		printf("You are in command line! Please enter you commands: \n");
		yyin = stdin;
		yyparse();
	}
}

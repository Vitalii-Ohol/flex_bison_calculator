%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gram.tab.h"
#include "astheader.h"


extern double baseToDecimal(const char* str, int base);
extern double hexadecimalToDecimal(const char* str);

//extern void yyerror(const char *s);

%}

DIGIT			[0-9]
LETTER			[a-zA-Z]

HEX				[0-9A-F]
WORD			[a-zA-Z_]
ALNUM			[0-9a-zA-Z_]


DECIMAL			{DIGIT}({DIGIT}|_)*(\.{DIGIT}({DIGIT}|_)*)?
HEXADECIMAL		0x{HEX}({HEX}|_)*(\.{HEX}({HEX}|_)*)?
BINARY			0b[01][01_]*(\.[01][01_]*)?
OCTAL			0o[0-7][0-7_]*(\.[0-7][0-7_]*)?
VAR_NAME		{LETTER}({ALNUM})*



/* Stop the error "undefined reference to 'yywrap'" from Flex */
%option noyywrap


%%
[ \t]										/* nothing: skip whitespace */
[\n]										/* nothing: skip new line */
"/*"[^*]*[*]+([^*/][^*]*[*]+)*[/]			{ printf("COMMENT: %s\n", yytext); }
"//"[^\n]*									{ printf("COMMENT: %s\n", yytext); }
"#"[^\n]*									{ printf("COMMENT: %s\n", yytext); }
{DECIMAL}									{ yylval.num = baseToDecimal(yytext, 10); return NUMBER; }
{BINARY}									{ yylval.num = baseToDecimal(&(yytext[2]), 2); return NUMBER; }
{OCTAL}										{ yylval.num = baseToDecimal(&(yytext[2]), 8); return NUMBER; }
{HEXADECIMAL}								{ yylval.num = hexadecimalToDecimal(&(yytext[2])); return NUMBER; }
"+"											{ return ADD; }
"-"											{ return SUB; }
"*"											{ return MUL; }
"/"											{ return DIV; }
"%"											{ return MOD; }
"^"											{ return POW; }
"="											{ return EQUALS; }
"("											{ return L_BRACKET; }
")"											{ return R_BRACKET; }
","											{ return COMMA; }
"!"											{ return FACTORIAL; }
";"											{ return EOA; }
">"											{ yylval.fn = 1; return CMP; }
"<"											{ yylval.fn = 2; return CMP; }
("!="|"<>")									{ yylval.fn = 3; return CMP; }
"=="										{ yylval.fn = 4; return CMP; }
">="										{ yylval.fn = 5; return CMP; }
"<="										{ yylval.fn = 6; return CMP; }
(print|PRINT)								{ yylval.fn = FN_PRINT; return FUNC; }
(return|RETURN)								{ yylval.fn = FN_RETURN; return FUNC; }
(e|E)										{ yylval.fn = FN_E; return FUNC; }
(pi|PI)										{ yylval.fn = FN_PI; return FUNC; }
(mod|MOD)									{ yylval.fn = FN_MOD; return FUNC; }
(pow|POW)									{ yylval.fn = FN_POW; return FUNC; }
(floor|FLOOR)								{ yylval.fn = FN_FLOOR; return FUNC; }
(ceil|CEIL)									{ yylval.fn = FN_CEIL; return FUNC; }
(round|ROUND)								{ yylval.fn = FN_CEIL; return FUNC; }
(abs|ABS)									{ yylval.fn = FN_ABS; return FUNC; }
(cos|COS)									{ yylval.fn = FN_COS; return FUNC; }
(sin|SIN)									{ yylval.fn = FN_SIN; return FUNC; }
(tan|TAN)									{ yylval.fn = FN_TAN; return FUNC; }
(sinh|SINH)									{ yylval.fn = FN_SINH; return FUNC; }
(cosh|COSH)									{ yylval.fn = FN_COSH; return FUNC; }
(tanh|TANH)									{ yylval.fn = FN_TANH; return FUNC; }
(exp|EXP)									{ yylval.fn = FN_EXP; return FUNC; }
(sqrt|SQRT)									{ yylval.fn = FN_SQRT; return FUNC; }
(log|LOG)									{ yylval.fn = FN_LOG2; return FUNC; }
(log2|LOG2)									{ yylval.fn = FN_LOG2; return FUNC; }
(log10|LOG10)								{ yylval.fn = FN_LOG10; return FUNC; }
(if|IF)										{ return IF_KEYWORD; }
(then|THEN)									{ return THEN_KEYWORD; }
(else|ELSE)									{ return ELSE_KEYWORD; }
(while|WHILE)								{ return WHILE_KEYWORD; }
(do|DO)										{ return DO_KEYWORD; }
(let|LET)									{ return LET_KEYWORD; }
(end|END)									{ return END_KEYWORD; }
(var|VAR)									{ return VAR_KEYWORD; }
(exit|EXIT)									{ printf("Quitting...\n"); exit(EXIT_SUCCESS); }
{VAR_NAME}									{ yylval.s = get_object(yytext); return NAME; }
.											{ yyerror("Undefined symbol"); exit(EXIT_FAILURE); }
%%


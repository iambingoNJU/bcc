%{
#include "syntax.tab.h"
#include "tree/bt.h"
int lex_error = 0;
%}

%option yylineno
	
SPACE			[\t\n ]+
COMMENT			\/\/.*
COMMENT2		\/\*

INT				0|[1-9][0-9]*|0[xX][0-9a-fA-F]+|0[0-7]+
FLOAT			[0-9]*\.[0-9]+([eE][+-]?[0-9]+)?
ID				[a-zA-Z_][a-zA-Z0-9_]{0,62}
SEMI			;
COMMA			,
ASSIGNOP		=
RELOP			>|<|>=|<=|==|!=
PLUS			\+
MINUS			-
STAR			\*
DIV				\/
AND				&&
OR				\|\|
DOT				\.
NOT				!
TYPE			int|float
LP				\(
RP				\)
LB				\[
RB				\]
LC				\{
RC				\}
STRUCT			struct
RETURN			return
IF				if
ELSE			else
WHILE			while


%%
{SPACE}			{ }
{COMMENT}		{ }
{COMMENT2}		{ char c = input(), next = input(); 
				  while(!((c == '*')&&(next == '/'))) {
					  c = next; next = input();
					  } }

{INT}			{ yylval.node = init_node(yylineno, "INT", yytext); return INT; }
{FLOAT}			{ yylval.node = init_node(yylineno, "FLOAT", yytext); return FLOAT; }
{SEMI}			{ yylval.node = init_node(yylineno, "SEMI", NULL); return SEMI; }
{COMMA}			{ yylval.node = init_node(yylineno, "COMMA", NULL); return COMMA; }
{ASSIGNOP}		{ yylval.node = init_node(yylineno, "ASSIGNOP", NULL); return ASSIGNOP; }
{RELOP}			{ yylval.node = init_node(yylineno, "RELOP", yytext); return RELOP; }
{PLUS}			{ yylval.node = init_node(yylineno, "PLUS", NULL); return PLUS; }
{MINUS}			{ yylval.node = init_node(yylineno, "MINUS", NULL); return MINUS; }
{STAR}			{ yylval.node = init_node(yylineno, "STAR", NULL); return STAR; }
{DIV}			{ yylval.node = init_node(yylineno, "DIV", NULL); return DIV; }
{AND}			{ yylval.node = init_node(yylineno, "AND", NULL); return AND; }
{OR}			{ yylval.node = init_node(yylineno, "OR", NULL); return OR; }
{DOT}			{ yylval.node = init_node(yylineno, "DOT", NULL); return DOT; }
{NOT}			{ yylval.node = init_node(yylineno, "NOT", NULL); return NOT; }
{TYPE}			{ yylval.node = init_node(yylineno, "TYPE", yytext); return TYPE; }
{LP}			{ yylval.node = init_node(yylineno, "LP", NULL); return LP; }
{RP}			{ yylval.node = init_node(yylineno, "RP", NULL); return RP; }
{LB}			{ yylval.node = init_node(yylineno, "LB", NULL); return LB; }
{RB}			{ yylval.node = init_node(yylineno, "RB", NULL); return RB; }
{LC}			{ yylval.node = init_node(yylineno, "LC", NULL); return LC; }
{RC}			{ yylval.node = init_node(yylineno, "RC", NULL); return RC; }
{STRUCT}		{ yylval.node = init_node(yylineno, "STRUCT", NULL); return STRUCT; }
{RETURN}		{ yylval.node = init_node(yylineno, "RETURN", NULL); return RETURN; }
{IF}			{ yylval.node = init_node(yylineno, "IF", NULL); return IF; }
{ELSE}			{ yylval.node = init_node(yylineno, "ELSE", NULL); return ELSE; }
{WHILE}			{ yylval.node = init_node(yylineno, "WHILE", NULL); return WHILE; }
{ID}			{ yylval.node = init_node(yylineno, "ID", yytext); return ID;}
.				{ lex_error = 1; printf("Error type A at Line %d: Mysterious characters \"%s\".\n", yylineno, yytext); }


%%

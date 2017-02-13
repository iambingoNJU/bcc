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

{INT}			{ init_node(&yylval.node, yylineno, "INT", yytext); return INT; }
{FLOAT}			{ init_node(&yylval.node, yylineno, "FLOAT", yytext); return FLOAT; }
{SEMI}			{ init_node(&yylval.node, yylineno, "SEMI", NULL); return SEMI; }
{COMMA}			{ init_node(&yylval.node, yylineno, "COMMA", NULL); return COMMA; }
{ASSIGNOP}		{ init_node(&yylval.node, yylineno, "ASSIGNOP", NULL); return ASSIGNOP; }
{RELOP}			{ init_node(&yylval.node, yylineno, "RELOP", yytext); return RELOP; }
{PLUS}			{ init_node(&yylval.node, yylineno, "PLUS", NULL); return PLUS; }
{MINUS}			{ init_node(&yylval.node, yylineno, "MINUS", NULL); return MINUS; }
{STAR}			{ init_node(&yylval.node, yylineno, "STAR", NULL); return STAR; }
{DIV}			{ init_node(&yylval.node, yylineno, "DIV", NULL); return DIV; }
{AND}			{ init_node(&yylval.node, yylineno, "AND", NULL); return AND; }
{OR}			{ init_node(&yylval.node, yylineno, "OR", NULL); return OR; }
{DOT}			{ init_node(&yylval.node, yylineno, "DOT", NULL); return DOT; }
{NOT}			{ init_node(&yylval.node, yylineno, "NOT", NULL); return NOT; }
{TYPE}			{ init_node(&yylval.node, yylineno, "TYPE", yytext); return TYPE; }
{LP}			{ init_node(&yylval.node, yylineno, "LP", NULL); return LP; }
{RP}			{ init_node(&yylval.node, yylineno, "RP", NULL); return RP; }
{LB}			{ init_node(&yylval.node, yylineno, "LB", NULL); return LB; }
{RB}			{ init_node(&yylval.node, yylineno, "RB", NULL); return RB; }
{LC}			{ init_node(&yylval.node, yylineno, "LC", NULL); return LC; }
{RC}			{ init_node(&yylval.node, yylineno, "RC", NULL); return RC; }
{STRUCT}		{ init_node(&yylval.node, yylineno, "STRUCT", NULL); return STRUCT; }
{RETURN}		{ init_node(&yylval.node, yylineno, "RETURN", NULL); return RETURN; }
{IF}			{ init_node(&yylval.node, yylineno, "IF", NULL); return IF; }
{ELSE}			{ init_node(&yylval.node, yylineno, "ELSE", NULL); return ELSE; }
{WHILE}			{ init_node(&yylval.node, yylineno, "WHILE", NULL); return WHILE; }
{ID}			{ init_node(&yylval.node, yylineno, "ID", yytext); return ID;}
.				{ lex_error = 1; printf("Error type A at Line %d: Mysterious characters \"%s\".\n", yylineno, yytext); }


%%

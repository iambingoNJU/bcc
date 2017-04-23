%{
#include "syntax.tab.h"
#include "tree/bt.h"

int lex_error = 0;
%}

%option yylineno
	
SPACE			[ \t\n]+
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
				  while(!((c == '*') && (next == '/'))) {
						  c = next; next = input();
					  } }

{INT}			{ yylval.node = init_node(yylineno, TYPE_INT, yytext);		return INT;		}
{FLOAT}			{ yylval.node = init_node(yylineno, TYPE_FLOAT, yytext);	return FLOAT;	}
{SEMI}			{ yylval.node = init_node(yylineno, TYPE_SEMI, NULL);		return SEMI;	}
{COMMA}			{ yylval.node = init_node(yylineno, TYPE_COMMA, NULL);		return COMMA;	}
{ASSIGNOP}		{ yylval.node = init_node(yylineno, TYPE_ASSIGNOP, NULL);	return ASSIGNOP;}
{RELOP}			{ yylval.node = init_node(yylineno, TYPE_RELOP, yytext);	return RELOP;	}
{PLUS}			{ yylval.node = init_node(yylineno, TYPE_PLUS, NULL);		return PLUS;	}
{MINUS}			{ yylval.node = init_node(yylineno, TYPE_MINUS, NULL);		return MINUS;	}
{STAR}			{ yylval.node = init_node(yylineno, TYPE_STAR, NULL);		return STAR;	}
{DIV}			{ yylval.node = init_node(yylineno, TYPE_DIV, NULL);		return DIV;		}
{AND}			{ yylval.node = init_node(yylineno, TYPE_AND, NULL);		return AND;		}
{OR}			{ yylval.node = init_node(yylineno, TYPE_OR, NULL);			return OR;		}
{DOT}			{ yylval.node = init_node(yylineno, TYPE_DOT, NULL);		return DOT;		}
{NOT}			{ yylval.node = init_node(yylineno, TYPE_NOT, NULL);		return NOT;		}
{TYPE}			{ yylval.node = init_node(yylineno, TYPE_TYPE, yytext);		return TYPE;	}
{LP}			{ yylval.node = init_node(yylineno, TYPE_LP, NULL);			return LP;		}
{RP}			{ yylval.node = init_node(yylineno, TYPE_RP, NULL);			return RP;		}
{LB}			{ yylval.node = init_node(yylineno, TYPE_LB, NULL);			return LB;		}
{RB}			{ yylval.node = init_node(yylineno, TYPE_RB, NULL);			return RB;		}
{LC}			{ yylval.node = init_node(yylineno, TYPE_LC, NULL);			return LC;		}
{RC}			{ yylval.node = init_node(yylineno, TYPE_RC, NULL);			return RC;		}
{STRUCT}		{ yylval.node = init_node(yylineno, TYPE_STRUCT, NULL);		return STRUCT;	}
{RETURN}		{ yylval.node = init_node(yylineno, TYPE_RETURN, NULL);		return RETURN;	}
{IF}			{ yylval.node = init_node(yylineno, TYPE_IF, NULL);			return IF;		}
{ELSE}			{ yylval.node = init_node(yylineno, TYPE_ELSE, NULL);		return ELSE;	}
{WHILE}			{ yylval.node = init_node(yylineno, TYPE_WHILE, NULL);		return WHILE;	}
{ID}			{ yylval.node = init_node(yylineno, TYPE_ID, yytext);		return ID;		}

.				{ lex_error = 1; printf("Error type A at Line %d: Mysterious characters \"%s\".\n", yylineno, yytext); }


%%

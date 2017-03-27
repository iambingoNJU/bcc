
%{
#include "lex.yy.c"
#include "ir/translate.h"
int yyerror(char* msg);
%}

/* declared types */

%union {
	//int type_int;
	//float type_float;
	struct Node* node;
	//char str[64];
}

%token<node>		INT
%token<node>		FLOAT
%token<node>		ID
%token<node>		TYPE
%token<node>		SEMI COMMA
%token<node>		ASSIGNOP
%token<node>		RELOP
%token<node>		PLUS MINUS STAR DIV
%token<node>		AND OR NOT
%token<node>		DOT
%token<node>		LP RP LB RB LC RC
%token<node>		STRUCT RETURN IF ELSE WHILE

%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE

%right ASSIGNOP
%left OR
%left AND
%left RELOP 
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%type<node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag
%type<node> VarDec FunDec VarList ParamDec CompSt Stmt DefList Def DecList Dec
%type<node> Exp Args StmtList

%%
Program : ExtDefList		{ expend(&$$, $1->lineno, "Program", 1, $1);
							  if(!lex_error){
								  semantic_analyse($$);
								  //show_symbTable(curSymbTable);
								  //display($$);
								  print_code_list(translate_Program($$));
							  }
							  free_tree($$);
							}
	;
ExtDefList :				{ /* $$ = NULL; */ expend(&$$, -1, "ExtDefList", 0); }
	| ExtDef ExtDefList		{ expend(&$$, $1->lineno, "ExtDefList", 2, $1, $2); }
	;
ExtDef : Specifier ExtDecList SEMI	{ expend(&$$, $1->lineno, "ExtDef", 3, $1, $2, $3); }
	| Specifier SEMI		{ expend(&$$, $1->lineno, "ExtDef", 2, $1, $2); }
	| Specifier FunDec SEMI				{ expend(&$$, $1->lineno, "ExtDef", 3, $1, $2, $3); }
	| Specifier FunDec CompSt			{ expend(&$$, $1->lineno, "ExtDef", 3, $1, $2, $3); }
	| Specifier error SEMI	{ lex_error = 1; printf("Missing \";\".\n"); }
	| Specifier ExtDecList	{ lex_error = 1; printf("Missing \";\".\n"); }
	| Specifier { lex_error = 1; printf("Missing \";\".\n"); }
	;
ExtDecList : VarDec			{ expend(&$$, $1->lineno, "ExtDecList", 1, $1); }
	| VarDec COMMA ExtDecList			{ expend(&$$, $1->lineno, "ExtDecList", 3, $1, $2, $3); }
	;

Specifier : TYPE			{ expend(&$$, $1->lineno, "Specifier", 1, $1); }
	| StructSpecifier		{ expend(&$$, $1->lineno, "Specifier", 1, $1); }
	;
StructSpecifier : STRUCT OptTag LC DefList RC	{ expend(&$$, $1->lineno, "StructSpecifier", 5, $1, $2, $3, $4, $5); }
	| STRUCT Tag			{ expend(&$$, $1->lineno, "StructSpecifier", 2, $1, $2); }
	;
OptTag :		{ /*$$ = NULL;*/ expend(&$$, -1, "OptTag", 0); }
	| ID		{ expend(&$$, $1->lineno, "OptTag", 1, $1); }
	;
Tag : ID		{ expend(&$$, $1->lineno, "Tag", 1, $1); }
	;

VarDec : ID		{ expend(&$$, $1->lineno, "VarDec", 1, $1); }
	| VarDec LB INT RB		{ expend(&$$, $1->lineno, "VarDec", 4, $1, $2, $3, $4); }
	| VarDec LB INT error RB	{ lex_error = 1; printf("Missing \"]\".\n"); }
	;
FunDec : ID LP VarList RP	{ expend(&$$, $1->lineno, "FunDec", 4, $1, $2, $3, $4); }
	| ID LP RP				{ expend(&$$, $1->lineno, "FunDec", 3, $1, $2, $3); }
	| ID LP error LC	{ lex_error = 1; printf("Missing \")\".\n"); }
	;
VarList : ParamDec COMMA VarList	{ expend(&$$, $1->lineno, "VarList", 3, $1, $2, $3); }
	| ParamDec	{ expend(&$$, $1->lineno, "VarList", 1, $1);}
	;
ParamDec : Specifier VarDec		{ expend(&$$, $1->lineno, "ParamDec", 2, $1, $2); }
	;

CompSt : LC DefList StmtList RC { expend(&$$, $1->lineno, "CompSt", 4, $1, $2, $3, $4); }
	| LC error	{ lex_error = 1; printf("Syntax error.\n"); }
	;
StmtList :		{ /*$$ = NULL;*/ expend(&$$, -1, "StmtList", 0); }
	| Stmt StmtList		{ expend(&$$, $1->lineno, "StmtList", 2, $1, $2); }
	;
Stmt : Exp SEMI			{ expend(&$$, $1->lineno, "Stmt", 2, $1, $2); }
	| CompSt			{ expend(&$$, $1->lineno, "Stmt", 1, $1); }
	| RETURN Exp SEMI	{ expend(&$$, $1->lineno, "Stmt", 3, $1, $2, $3); }
	| IF LP Exp RP Stmt %prec LOWER_THEN_ELSE	{ expend(&$$, $1->lineno, "Stmt", 5, $1, $2, $3, $4, $5); }
	| IF LP Exp RP Stmt ELSE Stmt	{ expend(&$$, $1->lineno, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
	| WHILE LP Exp RP Stmt { expend(&$$, $1->lineno, "Stmt", 5, $1, $2, $3, $4, $5); }
	| Exp error ELSE	{ lex_error = 1; printf("Missing \";\".\n"); }
	| Exp error Stmt	{ lex_error = 1; printf("Missing \";\".\n"); }
	;

DefList:  { /*$$ = NULL;*/ expend(&$$, -1, "DefList", 0); }
	| Def DefList	{ expend(&$$, $1->lineno, "DefList", 2, $1, $2); }
	;
Def : Specifier DecList SEMI	{ expend(&$$, $1->lineno, "Def", 3, $1, $2, $3); }
	| Specifier DecList error SEMI { lex_error = 1; printf("Missing \";\".\n"); }
	;
DecList : Dec		{ expend(&$$, $1->lineno, "DecList", 1, $1); }
	| Dec COMMA DecList			{ expend(&$$, $1->lineno, "DecList", 3, $1, $2, $3); }
	;
Dec : VarDec		{ expend(&$$, $1->lineno, "Dec", 1, $1); }
	| VarDec ASSIGNOP Exp		{ expend(&$$, $1->lineno, "Dec", 3, $1, $2, $3); }
	;

Exp : Exp ASSIGNOP Exp		{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| Exp AND Exp	{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| Exp OR Exp	{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| Exp RELOP Exp	{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| Exp PLUS Exp	{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| Exp MINUS Exp	{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| Exp STAR Exp	{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| Exp DIV Exp	{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| LP Exp RP		{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| MINUS Exp		{ expend(&$$, $1->lineno, "Exp", 2, $1, $2); }
	| NOT Exp		{ expend(&$$, $1->lineno, "Exp", 2, $1, $2); } 
	| ID LP Args RP		{ expend(&$$, $1->lineno, "Exp", 4, $1, $2, $3, $4); }
	| ID LP RP		{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| Exp LB Exp RB		{ expend(&$$, $1->lineno, "Exp", 4, $1, $2, $3, $4); }
	| Exp LB Exp error RB { lex_error = 1; printf("Missing \"]\".\n"); }
	| Exp DOT ID	{ expend(&$$, $1->lineno, "Exp", 3, $1, $2, $3); }
	| ID	{ expend(&$$, $1->lineno, "Exp", 1, $1); }
	| INT	{ expend(&$$, $1->lineno, "Exp", 1, $1); }
	| FLOAT	{ expend(&$$, $1->lineno, "Exp", 1, $1); }
	;
Args : Exp COMMA Args	{ expend(&$$, $1->lineno, "Args", 3, $1, $2, $3); }
	| Exp	{ expend(&$$, $1->lineno, "Args", 1, $1);	}
	;

%%

int yyerror(char* msg) {
	printf("Error type B at Line %d: ", yylineno);
	return 0;
}


%{
#include "lex.yy.c"
#include "ir/translate.h"

int yyerror(char* msg);
%}

/* declared types */
%union {
	struct Node* node;
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
Program : ExtDefList		{ $$ = expand_node("Program", 1, $1);
							  if(!lex_error){
								  semantic_analyse($$);
								  //show_symbTable(curSymbTable);
								  display($$);
								  print_code_list(translate_Program($$));
							  }
							  free_tree($$);
							}
	;
ExtDefList :				{ /*$$ = NULL;*/ $$ = expand_node("ExtDefList", 0); }
	| ExtDef ExtDefList		{ $$ = expand_node("ExtDefList", 2, $1, $2); }
	;
ExtDef : Specifier ExtDecList SEMI	{ $$ = expand_node("ExtDef", 3, $1, $2, $3); }
	| Specifier SEMI		{ $$ = expand_node("ExtDef", 2, $1, $2); }
	| Specifier FunDec SEMI				{ $$ = expand_node("ExtDef", 3, $1, $2, $3); }
	| Specifier FunDec CompSt			{ $$ = expand_node("ExtDef", 3, $1, $2, $3); }
	| Specifier error SEMI	{ lex_error = 1; printf("Missing \";\".\n"); }
	| Specifier ExtDecList	{ lex_error = 1; printf("Missing \";\".\n"); }
	| Specifier { lex_error = 1; printf("Missing \";\".\n"); }
	;
ExtDecList : VarDec			{ $$ = expand_node("ExtDecList", 1, $1); }
	| VarDec COMMA ExtDecList			{ $$ = expand_node("ExtDecList", 3, $1, $2, $3); }
	;

Specifier : TYPE			{ $$ = expand_node("Specifier", 1, $1); }
	| StructSpecifier		{ $$ = expand_node("Specifier", 1, $1); }
	;
StructSpecifier : STRUCT OptTag LC DefList RC	{ $$ = expand_node("StructSpecifier", 5, $1, $2, $3, $4, $5); }
	| STRUCT Tag			{ $$ = expand_node("StructSpecifier", 2, $1, $2); }
	;
OptTag :		{ /*$$ = NULL;*/ $$ = expand_node("OptTag", 0); }
	| ID		{ $$ = expand_node("OptTag", 1, $1); }
	;
Tag : ID		{ $$ = expand_node("Tag", 1, $1); }
	;

VarDec : ID		{ $$ = expand_node("VarDec", 1, $1); }
	| VarDec LB INT RB		{ $$ = expand_node("VarDec", 4, $1, $2, $3, $4); }
	| VarDec LB INT error RB	{ lex_error = 1; printf("Missing \"]\".\n"); }
	;
FunDec : ID LP VarList RP	{ $$ = expand_node("FunDec", 4, $1, $2, $3, $4); }
	| ID LP RP				{ $$ = expand_node("FunDec", 3, $1, $2, $3); }
	| ID LP error LC	{ lex_error = 1; printf("Missing \")\".\n"); }
	;
VarList : ParamDec COMMA VarList	{ $$ = expand_node("VarList", 3, $1, $2, $3); }
	| ParamDec	{ $$ = expand_node("VarList", 1, $1);}
	;
ParamDec : Specifier VarDec		{ $$ = expand_node("ParamDec", 2, $1, $2); }
	;

CompSt : LC DefList StmtList RC { $$ = expand_node("CompSt", 4, $1, $2, $3, $4); }
	| LC error	{ lex_error = 1; printf("Syntax error.\n"); }
	;
StmtList :		{ /*$$ = NULL;*/ $$ = expand_node("StmtList", 0); }
	| Stmt StmtList		{ $$ = expand_node("StmtList", 2, $1, $2); }
	;
Stmt : Exp SEMI			{ $$ = expand_node("Stmt", 2, $1, $2); }
	| CompSt			{ $$ = expand_node("Stmt", 1, $1); }
	| RETURN Exp SEMI	{ $$ = expand_node("Stmt", 3, $1, $2, $3); }
	| IF LP Exp RP Stmt %prec LOWER_THEN_ELSE	{ $$ = expand_node("Stmt", 5, $1, $2, $3, $4, $5); }
	| IF LP Exp RP Stmt ELSE Stmt	{ $$ = expand_node("Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
	| WHILE LP Exp RP Stmt { $$ = expand_node("Stmt", 5, $1, $2, $3, $4, $5); }
	| Exp error ELSE	{ lex_error = 1; printf("Missing \";\".\n"); }
	| Exp error Stmt	{ lex_error = 1; printf("Missing \";\".\n"); }
	;

DefList:  { /*$$ = NULL;*/ $$ = expand_node("DefList", 0); }
	| Def DefList	{ $$ = expand_node("DefList", 2, $1, $2); }
	;
Def : Specifier DecList SEMI	{ $$ = expand_node("Def", 3, $1, $2, $3); }
	| Specifier DecList error SEMI { lex_error = 1; printf("Missing \";\".\n"); }
	;
DecList : Dec		{ $$ = expand_node("DecList", 1, $1); }
	| Dec COMMA DecList			{ $$ = expand_node("DecList", 3, $1, $2, $3); }
	;
Dec : VarDec		{ $$ = expand_node("Dec", 1, $1); }
	| VarDec ASSIGNOP Exp		{ $$ = expand_node("Dec", 3, $1, $2, $3); }
	;

Exp : Exp ASSIGNOP Exp		{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| Exp AND Exp	{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| Exp OR Exp	{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| Exp RELOP Exp	{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| Exp PLUS Exp	{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| Exp MINUS Exp	{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| Exp STAR Exp	{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| Exp DIV Exp	{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| LP Exp RP		{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| MINUS Exp		{ $$ = expand_node("Exp", 2, $1, $2); }
	| NOT Exp		{ $$ = expand_node("Exp", 2, $1, $2); } 
	| ID LP Args RP		{ $$ = expand_node("Exp", 4, $1, $2, $3, $4); }
	| ID LP RP		{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| Exp LB Exp RB		{ $$ = expand_node("Exp", 4, $1, $2, $3, $4); }
	| Exp LB Exp error RB { lex_error = 1; printf("Missing \"]\".\n"); }
	| Exp DOT ID	{ $$ = expand_node("Exp", 3, $1, $2, $3); }
	| ID	{ $$ = expand_node("Exp", 1, $1); }
	| INT	{ $$ = expand_node("Exp", 1, $1); }
	| FLOAT	{ $$ = expand_node("Exp", 1, $1); }
	;
Args : Exp COMMA Args	{ $$ = expand_node("Args", 3, $1, $2, $3); }
	| Exp	{ $$ = expand_node("Args", 1, $1);	}
	;

%%

int yyerror(char* msg) {
	printf("Error type B at Line %d: ", yylineno);
	return 0;
}

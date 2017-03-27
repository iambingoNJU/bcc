#ifndef __BT_H__
#define __BT_H__

#include "../debug.h"
#include "../semantic/symbol_table.h"
#include "../semantic/type.h"

extern int yylineno;

struct Node {
	char type[16];
	char val[64];
	int lineno;
	int is_terminal;

	int is_var;
	V_type vt_syn;
	int is_implem;
	list_node_Item *vt_list;

	// for translation
	char *var_name;
	struct list_node_InterCode *code;

	struct Node *fc;
	struct Node *ns;
};

void init_node(struct Node**, int, const char*, char*);

void expend(struct Node**, int, const char*, int, ...);

void free_tree(struct Node*);

void display(struct Node*);

void semantic_analyse(struct Node*);

#endif
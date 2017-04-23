#ifndef __BT_H__
#define __BT_H__

#include "../debug.h"
#include "../semantic/type.h"

extern int yylineno;

enum {
#define X(symbol) TYPE_##symbol,
#include "token"
#undef X
};

extern char* type_list[];

struct Node {
	int type;
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

struct Node* init_node(int ln, int t, char* s);
struct Node* expand_node(int t, int n, ...);

void free_tree(struct Node*);
void display(struct Node*);

#endif

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "bt.h"

char* type_list[] = {
#define X(symbol) [TYPE_##symbol] = #symbol,
#include "token"
#undef X
};

struct Node* init_node(int ln, int t, char* s) {
	struct Node* n = (struct Node*)malloc(sizeof(struct Node));
	assert(n);

	memset(n, 0, sizeof(struct Node));
	n->type = t;
	if(s) strcpy(n->val, s);
	else n->val[0] = '\0';
	n->lineno = ln;
	n->is_terminal = 1;
	n->fc = NULL;
	n->ns = NULL;	

	return n;
}

struct Node* expand_node(int t, int n, ...) {
	struct Node *temp, *arg, *tr = (struct Node*)malloc(sizeof(struct Node));
	assert(tr && (n >= 0));
	memset(tr, 0, sizeof(struct Node));

	tr->type = t;
	tr->val[0] = '\0';
	tr->is_terminal = 0;
	tr->ns = NULL;

	va_list arg_ptr;
	va_start(arg_ptr, n);
	int i = 0;
	//There maybe exist NULL struct Node pointer.
	while(!(arg = va_arg(arg_ptr, struct Node*))) i++;
	if(i < n) {
		tr->fc = arg;
		tr->lineno = arg->lineno;
		i++;
		temp = tr->fc;
		for(; i < n; i++) {
			if((arg = va_arg(arg_ptr, struct Node*)) != NULL) {
				temp->ns = arg;
				temp = temp->ns;
			}
		}
	} else {
		tr->fc = NULL;
		Log("Node '%s' has no child node.\n", type_list[t]);
		tr->lineno = -1;
	}
	va_end(arg_ptr);

	return tr;
}

void free_tree(struct Node *tr) {
	if(tr != NULL) {
		free_tree(tr->fc);
		free_tree(tr->ns);
		free(tr);
	}
}


void display(struct Node *tr) {
	static int nr_tab = 0;
	if(tr != NULL) {
		int i;
		for(i = 0; i < nr_tab; i++)	printf("  ");
		printf("%s", type_list[tr->type]);
		if(tr->type == TYPE_INT) {
			printf(": %ld", strtol(tr->val, NULL, 0));
		} else if(tr->type == TYPE_FLOAT) {
			printf(": %f", atof(tr->val));
		} else if((tr->type == TYPE_ID) || (tr->type == TYPE_TYPE)) {
			printf(": %s", tr->val);
		}
		if(!tr->is_terminal) printf(" (%d)", tr->lineno);
		printf("\n");

		for(tr = tr->fc; tr != NULL; tr = tr->ns) {
			nr_tab++;
			display(tr);
			nr_tab--;
		}
	}
}



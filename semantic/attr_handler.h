
#ifndef __ATTR_HANDLER_H__
#define __ATTR_HANDLER_H__

#include "type.h"
#include "symbol_table.h"

typedef struct Node Node;
typedef int (*inh_attr_handler)(Node *father, Node *child);
typedef int (*syn_attr_handler)(Node *ast);

#define inh_helper(symbol) \
static int symbol##_inh_handler(struct Node *father, struct Node *child)
#define nop_inh_helper(symbol) \
static int symbol##_inh_handler(struct Node *father, struct Node *child) { \
	return 0; \
}

#define syn_helper(symbol) \
static int symbol##_syn_handler(struct Node *ast)
#define nop_syn_helper(symbol) \
static int symbol##_syn_handler(struct Node *ast) { \
	return 0; \
}

void typeerror(int err, int lineno, const char *msg);

void semantic_analyse(struct Node *ast);

#endif /* __ATTR_HANDLER_H__ */

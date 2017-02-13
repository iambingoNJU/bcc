#ifndef __TYPE_H__
#define __TYPE_H__

#include "clist.h"
#include "../debug.h"

#define MAX_SYMBOL_LEN 64

typedef enum DATA_TYPE { T_BASIC = 32, T_ARRAY, T_STRUCTURE } DT;
typedef enum BASIC_TYPE { T_INT = 64, T_FLOAT } BT;

typedef struct V_type_* V_type;
typedef struct F_type_* F_type;

typedef struct Item {
	char name[MAX_SYMBOL_LEN];
	V_type type;
} Item;

DEFINE_LIST_NODE(Item)
EXTERN_LIST_PUSH_BACK(Item)
EXTERN_LIST_PUSH_FRONT(Item)
EXTERN_LIST_MERGE(Item)
list_node_Item* list_find_Item(list_node_Item*, char*);

struct V_type_ {
	DT kind;
	union {
		BT basic;	/* basic */
		struct {	/* array */
			V_type elem;
			unsigned sz;
		} array;
		struct {	/* structure */
			char name[MAX_SYMBOL_LEN];
			list_node_Item *field; 
		} structure;
	};
};

struct F_type_ {
	V_type rtn_val;
	list_node_Item *args;
};

void gen_basic_vt(V_type *pbvt, int t);
void gen_array_vt(V_type *pavt, V_type cvt, int sz);
void gen_struct_vt(V_type *psvt, char *name, list_node_Item *hf);
int type_match(V_type t1, V_type t2);
void show_vtype(V_type t);
int vt_size(V_type t);
int args_match(list_node_Item *n1, list_node_Item *n2);

#endif /* __TYPE_H__ */

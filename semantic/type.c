
#include "type.h"
#include <stdio.h>

IMPLEMENT_LIST_PUSH_BACK(Item)
IMPLEMENT_LIST_PUSH_FRONT(Item)
IMPLEMENT_LIST_MERGE(Item)

list_node_Item* list_find_Item(list_node_Item *h, char *name) {
	while(h != NULL) {
		if(strcmp((h->data).name, name) == 0)
			return h;
		h = h->next;
	}
	return NULL;
}

void gen_basic_vt(V_type *pbvt, int t) {
	Assert(pbvt, "gen_basic_vt error");
	V_type nt = (V_type)malloc(sizeof(struct V_type_));
	nt->kind = T_BASIC;
	if(t == 1) {
		nt->basic = T_INT;
	} else if(t == 2) {
		nt->basic = T_FLOAT;
	}
	*pbvt = nt;
	//Log("Generating type %d", t);
}

void gen_array_vt(V_type *pavt, V_type cvt, int sz) {
	Assert(pavt, "gen_array_vt error");
	V_type nt = (V_type)malloc(sizeof(struct V_type_));
	nt->kind = T_ARRAY;
	nt->array.elem = cvt;
	nt->array.sz = sz;
	*pavt = nt;
	//Log("Generating type array");
}

void gen_struct_vt(V_type *psvt, char *name, list_node_Item *hf) {
	Assert(psvt, "gen_struct_vt error");
	V_type nt = (V_type) malloc(sizeof(struct V_type_));
	nt->kind = T_STRUCTURE;
	if(name)	strcpy((nt->structure).name, name);
	else (nt->structure).name[0] = '\0';
	(nt->structure).field = hf;
	*psvt = nt;
	//Log("Generating type struct");
}

int type_match(V_type t1, V_type t2) {
	//Log("Type checking: t1 = %p, t2 = %p", t1, t2);
	if((t1 == NULL) || (t2 == NULL)) {
		Log("Match error!");
		return 0;
	}
	if(t1 == t2) return 1;
	if(t1->kind != t2->kind) {
		//Log("Type missmatching");
		return 0;
	} else {
		if(t1->kind == T_BASIC) {
			return t1->basic == t2->basic;
		} else if(t1->kind == T_ARRAY) {
			//return (t1->array.sz == t2->array.sz) && type_match(t1->array.elem, t2->array.elem);
			return type_match(t1->array.elem, t2->array.elem);
		} else if(t2->kind == T_STRUCTURE) {
			return args_match(t1->structure.field, t2->structure.field);
		}
	}
	//Log("finished");
	return 0;
}

int args_match(list_node_Item *n1, list_node_Item *n2) {
	if((n1 == NULL) && (n2 == NULL)) {
		Log("both NULL");
		return 1;
	} else if((n1 == NULL) || (n2 == NULL)) {
		if(n1 == NULL)
			Log("arg1 is NULL");
		else
			Log("arg2 is NULL");
		return 0;
	}

	if(type_match(n1->data.type, n2->data.type) == 0) {
		return 0;
	} else {
		return args_match(n1->next, n2->next);
	}
}

void show_vtype(V_type t) {
	if(t == NULL) {
		printf("arg is null pointer.");
		return;
	}
	
	switch(t->kind) {
		case T_BASIC:	{
			if(t->basic == T_INT) {
				printf("Basic(int)");
			} else if(t->basic == T_FLOAT) {
				printf("Basic(float)");
			}
			break;
		}
		case T_ARRAY:	{
			printf("Array[%d, ", (t->array).sz);
			show_vtype((t->array).elem);
			printf("]");
			break;
		}
		case T_STRUCTURE:{
			printf("Struct{\n");
			list_node_Item *tmp = (t->structure).field;
			while(tmp != NULL) {
				show_vtype((tmp->data).type);
				printf("\t%s\n", (tmp->data).name);
				tmp = tmp->next;
			}
			printf("}");
			break;
		}
		default:	printf("Undefined type[%d].\n", t->kind);
	}
}

int vt_size(V_type t) {
	if(t == NULL) return 0;
	if(t->kind == T_BASIC) {
		return 4;
	} else if(t->kind == T_ARRAY) {
		return (t->array).sz * vt_size((t->array).elem);
	} else if(t->kind == T_STRUCTURE) {
		int sum = 0;
		list_node_Item *tmp = (t->structure).field;
		while(tmp != NULL) {
			sum += vt_size((tmp->data).type);
			tmp = tmp->next;
		}
		return sum;
	}

	printf("Invalid type.\n");
	return -1;
}

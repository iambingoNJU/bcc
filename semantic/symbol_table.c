
#include "symbol_table.h"
#include <stdio.h>

void typeerror(int err, int lineno, const char *msg);

/* list of all the symbol table */
list_node_pSymbTable* SymbTableList = NULL;
/* Global symbol table defination */
list_node_SymbItem* GlobalSymbTable[SYMB_TABLE_SIZE];
/* current symbol table */
pSymbTable curSymbTable = GlobalSymbTable;

/* operation on list of SymbItem */
IMPLEMENT_LIST_PUSH_FRONT(SymbItem)
IMPLEMENT_LIST_FREE(SymbItem)
list_node_SymbItem* list_find_SymbItem(list_node_SymbItem *h, char *name) {
	while(h != NULL) {
		if(strcmp(name, (h->data).name) == 0) return h;
		h = h->next;
	}
	return NULL;
}

/* operation on SymbTable */
void init_symbTable(pSymbTable pst) {
	int i;
	for(i = 0; i < SYMB_TABLE_SIZE; i++) {
		pst[i] = NULL;
	}
}

// add some system function to global system table
// curSymbTable must be GlobalSymbTable
void add_sys_func() {
	V_type vt = gen_basic_vt(1);

	Item item;
	strcpy(item.name, "");
	item.type = vt;

	list_node_Item *pItem = (list_node_Item*)malloc(sizeof(list_node_Item));
	pItem->next = NULL;
	memcpy(&(pItem->data), &item, sizeof(Item));

	pushSymbTable();
	add_func_to_symbol_table("read", vt, NULL, 1, -1);
	add_func_to_symbol_table("write", NULL, pItem, 1, -1);
	popSymbTable();
}

pSymbTable newSymbTable() {
	pSymbTable pst;
	pst = (pSymbTable)malloc(SYMB_TABLE_SIZE * sizeof(list_node_SymbItem*));
	Assert(pst, "New symbol table error!");
	init_symbTable(pst);
	return pst;
}

void show_symbTable(pSymbTable pst) {
	int i;
	for(i = 0; i < SYMB_TABLE_SIZE; i++) {
		list_node_SymbItem *tmp = pst[i];
		while(tmp != NULL) {
			printf("Idx: %d -- Name: %s\n", i, (tmp->data).name);
			tmp = tmp->next;
		}
	}
}

void dumpCurSymbTable() {
	show_symbTable(curSymbTable);
}

void freeSymbTable(pSymbTable pst) {
	Assert(pst, "NULL symbol table can't be freed.");
	int i;
	for(i = 0; i < SYMB_TABLE_SIZE; i++) {
		list_free_SymbItem(&(pst[i]));
	}
	free(pst);
}

IMPLEMENT_LIST_PUSH_FRONT(pSymbTable)
IMPLEMENT_LIST_FREE(pSymbTable)

void pushSymbTable() {
	Log("Push symbol table");
	list_push_front_pSymbTable(&SymbTableList, &curSymbTable);
	Assert(SymbTableList, "push symbol table error");
	curSymbTable = newSymbTable();
	Assert(curSymbTable, "Push symbol table error!");
}

void popSymbTable() {
	Log("Pop symbol table");
	freeSymbTable(curSymbTable);
	Assert(SymbTableList, "pop symbol table error");
	curSymbTable = SymbTableList->data;
	SymbTableList = SymbTableList->next;
}

unsigned hash_pjw(char* name) {
	unsigned val = 0, i;
	for(; *name; ++name) {
		val = (val << 2) + *name;
		if((i = val & ~(SYMB_TABLE_SIZE - 1)) != 0)
			val = (val ^ (i >> 12)) & (SYMB_TABLE_SIZE - 1);
	}
	return val;
}

list_node_SymbItem* search_SymbTableList(char *name) {
	// find in current SymbTable
	list_node_SymbItem *h = curSymbTable[hash_pjw(name)];
	list_node_SymbItem *res = list_find_SymbItem(h, name);
	if(res != NULL) return res;

	// find along the SymbTableList
	list_node_pSymbTable *tmp = SymbTableList;
	while(tmp != NULL) {
		h = (tmp->data)[hash_pjw(name)];
		res = list_find_SymbItem(h, name);
		if(res != NULL) {
			break;
		}
		tmp = tmp->next;
	} 

	return res;
}

V_type query_var(char *name) {
	Log("in query var");
	Log("Querying %s", name);
	//list_node_SymbItem *h = curSymbTable[hash_pjw(name)];
	//list_node_SymbItem *res = list_find_SymbItem(h, name);
	list_node_SymbItem *res = search_SymbTableList(name);
	if(res == NULL) {
		Log("Can't find %s", name);
		return NULL;
	} else {
		if((res->data).vf == T_VAR) {
			return (res->data).vt;
		} else {
			Log("'%s' is not a variable.", name);
			return NULL;
		}
	}
}

V_type query_locale_var(char *name) {
	Log("in query locale var");
	Log("Querying %s", name);
	list_node_SymbItem *h = curSymbTable[hash_pjw(name)];
	list_node_SymbItem *res = list_find_SymbItem(h, name);
	if(res == NULL) {
		Log("Can't find %s", name);
		return NULL;
	} else {
		if((res->data).vf == T_VAR) {
			return (res->data).vt;
		} else {
			Log("'%s' is not a variable.", name);
			return NULL;
		}
	}
}

V_type query_type(char *name) {
	Log("in query type");
	Log("Querying %s", name);
	list_node_SymbItem *res = search_SymbTableList(name);
	if(res == NULL) {
		Log("Can't find %s", name);
		return NULL;
	} else {
		if((res->data).vf == T_TYPE) {
			return (res->data).vt;
		} else {
			Log("'%s' is not a type.", name);
			return NULL;
		}
	}
}

F_type query_func(char *name, int flag) {
	Log("in query func");
	Log("Querying %s", name);
	list_node_SymbItem *res = search_SymbTableList(name);
	if(res == NULL) {
		Log("Can't find %s", name);
		return NULL;
	} else {
		if((res->data).vf == T_FUNC) {
			if(flag == 0) {
				return (res->data).ft;
			} else {
				return ((res->data).f_implement) ? (res->data).ft : NULL;
			}
		} else {
			Log("'%s' is not a function.", name);
			return NULL;
		}
	}
}

void add_var_to_symbol_table(char *name, V_type vt, int flag) {
	Log("Add %s", name);
	SymbItem si;
	memset(&si, 0, sizeof(SymbItem));
	strcpy(si.name, name);
	if(flag == 0) {
		si.vf = T_VAR;
	} else if(flag == 1) {
		si.vf = T_TYPE;
	}
	si.vt = vt;
	list_push_front_SymbItem(&(curSymbTable[hash_pjw(name)]), &si);
}

void add_func_to_symbol_table(char *name, V_type rtn, list_node_Item *args, int flag, int ln) {
	Log("Add %s", name);
	pSymbTable tmpSym = curSymbTable;
	curSymbTable = SymbTableList->data;

	list_node_SymbItem *res = search_SymbTableList(name);
	if(res != NULL) {
		Assert((res->data).f_implement == 0, "Redefined function");
		if(flag == 1) {
			(res->data).f_implement = 1;	
		}
		curSymbTable = tmpSym;
		return;
	}
	SymbItem si;
	memset(&si, 0, sizeof(SymbItem));
	strcpy(si.name, name);
	si.lineno = ln;
	si.vf = T_FUNC;
	si.ft = (F_type)malloc(sizeof(struct F_type_));
	(si.ft)->rtn_val = rtn;
	(si.ft)->args = args;
	if(flag == 0) {
		si.f_extern = 1;
		Log("Function extern.");
	} else {
		si.f_implement = 1;
		Log("Function implementation.");
	}
	list_push_front_SymbItem(&(curSymbTable[hash_pjw(name)]), &si);
	curSymbTable = tmpSym;
}

void check_undefined_func() {
	int i;
	for(i = 0; i < SYMB_TABLE_SIZE; i++) {
		list_node_SymbItem *h = curSymbTable[i];
		while(h != NULL) {
			if(h->data.vf == T_FUNC) {
				if(h->data.f_implement == 0) {
					typeerror(18, h->data.lineno, h->data.name);
				}
			}
			h = h->next;
		}
	}
}

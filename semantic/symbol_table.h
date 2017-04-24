#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "type.h"
#include "../debug.h"

#define SYMB_TABLE_SIZE 128

typedef enum SYMBOL_TYPE { T_VAR = 16, T_FUNC, T_TYPE } ST;

typedef struct SymbolItem { /* item of the symbol table */
	char name[MAX_SYMBOL_LEN];
	int lineno;
	ST vf;
	union {
		V_type vt;	/* pointer!!! */
		struct {
			F_type ft;
			int f_extern, f_implement;
		};
	};
} SymbItem;

DEFINE_LIST_NODE(SymbItem)

typedef list_node_SymbItem** pSymbTable;
extern pSymbTable curSymbTable;
DEFINE_LIST_NODE(pSymbTable)

void add_var_to_symbol_table(char *name, V_type vt, int flag);
void add_func_to_symbol_table(char *name, V_type rtn, list_node_Item *args, int flag, int lineno);
V_type query_var(char *name);
V_type query_locale_var(char *name);
V_type query_type(char *name);
F_type query_func(char *name, int flag);

void init_symbTable(pSymbTable pst);
void show_symbTable(pSymbTable pst);
void add_sys_func();
void pushSymbTable();
void popSymbTable();
void check_undefined_func();
void dumpCurSymbTable();

#endif /* __SYMBOL_TABLE_H__ */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "bt.h"

int in_struct = 0;


void init_node(struct Node** pn, int ln, const char* t, char* s) {
	struct Node* n = (struct Node*)malloc(sizeof(struct Node));
	assert(n && t);
	memset(n, 0, sizeof(struct Node));
	strcpy(n->type, t);
	if(s) strcpy(n->val, s);
	else n->val[0] = '\0';
	n->lineno = ln;
	n->is_terminal = 1;
	n->fc = NULL;
	n->ns = NULL;	
	*pn = n;
}

void expend(struct Node **ptr, int ln, const char* t, int n, ...) {
	struct Node *temp, *arg, *tr = (struct Node*)malloc(sizeof(struct Node));
	assert(tr && ptr && n >= 0);
	memset(tr, 0, sizeof(struct Node));
	strcpy(tr->type, t);
	tr->val[0] = '\0';
	tr->lineno = ln;
	tr->is_terminal = 0;
	tr->ns = NULL;
	//There maybe exist NULL struct Node pointer.
	va_list arg_ptr;
	va_start(arg_ptr, n);
	struct Node* arr[10];
	int i = 0;
	while(!(arg = va_arg(arg_ptr, struct Node*))) i++;
	if(i < n) {
		tr->fc = arg;
		i++;
		temp = tr->fc;
		for(; i < n; i++) {
			if((arg = va_arg(arg_ptr, struct Node*)) != NULL) {
				temp->ns = arg;
				temp = temp->ns;
			}
		}
	} else tr->fc = NULL;
	*ptr = tr;
	va_end(arg_ptr);
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
		printf("%s", tr->type);
		if(strcmp(tr->type, "INT") == 0 ) {
			printf(": %ld", strtol(tr->val, NULL, 0));
		} else if(strcmp(tr->type, "FLOAT") == 0) {
			printf(": %f", atof(tr->val));
		} else if(!strcmp(tr->type, "ID") || !strcmp(tr->type, "TYPE")) {
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


void typeerror(int err, int lineno, const char *msg) {
	printf("Error type %d at Line %d: ", err, lineno);
	switch(err) {
		case 1:		printf("Undefined variable '%s'.\n", msg);	break;
		case 2:		printf("Undefined function '%s'.\n", msg);	break;
		case 3:		printf("Redefined variable '%s'.\n", msg);	break;
		case 4:		printf("Redefined function '%s'.\n", msg);	break;
		case 5:		printf("Type mismatched for assignment.\n");	break;
		case 6:		printf("The left-hand side of an assignment must be a variable.\n");	break;
		case 7:		printf("Type mismatched for operands.\n");	break;
		case 8:		printf("Type mismatched for return.\n");	break;
		case 9:		printf("Function '%s' is not applicable for arguments.\n", msg);	break;
		case 10:	printf("'%s' is not an array.\n", msg);	break;
		case 11:	printf("'%s' is not an function.\n", msg);	break;
		case 12:	printf("'%s' is not an integer.\n", msg);	break;
		case 13:	printf("Illegal use of '.'.\n");	break;
		case 14:	printf("Non-existent field '%s'.\n", msg);	break;
		case 15:	printf("Redefined/Initialized field '%s'.\n", msg);	break;
		case 16:	printf("Duplicated name '%s'.\n", msg);	break;
		case 17:	printf("Undefined structure '%s'.\n", msg);	break;
		case 18:	printf("Undefined function '%s'.\n", msg); break;
		case 19:	printf("Inconsistent declaration of function '%s'.\n", msg); break;
		default:	printf("Unkown error.");
	}
}

void semantic_analyse(struct Node *ast) {
	if(ast != NULL) {
		//do recursively
		struct Node *tmp;
		for(tmp = ast->fc; tmp != NULL; tmp = tmp->ns) {
			//preorder
			if(strcmp(ast->type, "ExtDef") == 0) {
				if(strcmp(tmp->type, "ExtDecList") == 0) {
					tmp->vt_syn = ast->fc->vt_syn;
				} else if(strcmp(tmp->type, "FunDec") == 0) {
					tmp->vt_syn = ast->fc->vt_syn;
					if(strcmp(tmp->ns->type, "CompSt") == 0) {
						tmp->ns->vt_syn = ast->fc->vt_syn;
						tmp->is_implem = 1;
						tmp->ns->is_implem = 1;
					} else if(strcmp(tmp->ns->type, "SEMI") == 0) {
						tmp->is_implem = 0;
						tmp->ns->is_implem = 0;
					}
				}
			} else if((strcmp(ast->type, "ExtDecList") == 0) &&
				((strcmp(tmp->type, "VarDec") == 0) ||
				(strcmp(tmp->type, "ExtDecList") == 0))) {
				tmp->vt_syn = ast->vt_syn;
			} else if((strcmp(ast->type, "VarDec") == 0) &&
				(strcmp(tmp->type, "ID") == 0)) {
				tmp->vt_syn = ast->vt_syn;
				tmp->is_var = 1;
			} else if((strcmp(ast->type, "VarDec") == 0) &&
				(strcmp(tmp->type, "VarDec") == 0)) {
				gen_array_vt(&(tmp->vt_syn), ast->vt_syn, (int)strtol(ast->fc->ns->ns->val, NULL, 0)) ;
			} else if(strcmp(ast->type, "FunDec") == 0) {
				if(strcmp(tmp->type, "ID") == 0) {
					tmp->vt_syn = ast->vt_syn;
					tmp->is_var = 0;
				} else if(strcmp(tmp->type, "LP") == 0) {	// function arguments in child symbol table
					pushSymbTable();
				}
			} else if((strcmp(ast->type, "ParamDec") == 0) &&
				(strcmp(tmp->type, "VarDec") == 0)) {
				tmp->vt_syn = ast->fc->vt_syn;
			} else if((strcmp(ast->type, "Def") == 0) &&
				(strcmp(tmp->type, "DecList") == 0)) {
				tmp->vt_syn = ast->fc->vt_syn;
			} else if((strcmp(ast->type, "DecList") == 0) &&
				((strcmp(tmp->type, "Dec") == 0) ||
				(strcmp(tmp->type, "DecList") == 0))) {
				tmp->vt_syn = ast->vt_syn;
			} else if((strcmp(ast->type, "Dec") == 0) &&
				(strcmp(tmp->type, "VarDec") == 0)) {
				tmp->vt_syn = ast->vt_syn;
			} else if(strcmp(ast->type, "CompSt") == 0) {
				if(strcmp(tmp->type, "LC") == 0) { 
					if(ast->is_implem == 0) {
						pushSymbTable();
					}
				} else if(strcmp(tmp->type, "StmtList") == 0) {
					tmp->vt_syn = ast->vt_syn;
				} else if(strcmp(tmp->type, "RC") == 0) {
					//dumpCurSymbTable();
					popSymbTable();
					//dumpCurSymbTable();
				}
			} else if(strcmp(ast->type, "StructSpecifier") == 0) {
				if(strcmp(tmp->type, "LC") == 0) {
					pushSymbTable();
					in_struct = 1;
				} else if(strcmp(tmp->type, "RC") == 0) {
					//dumpCurSymbTable();
					popSymbTable();
					//dumpCurSymbTable();
					in_struct = 0;
				}
			} else if((strcmp(ast->type, "StmtList") == 0) &&
				(strcmp(tmp->type, "Stmt") == 0)) {
				tmp->vt_syn = ast->vt_syn;
				ast->fc->ns->vt_syn = ast->vt_syn;
			} else if((strcmp(ast->type, "Stmt") == 0) &&
				((strcmp(tmp->type, "CompSt") == 0) ||
				(strcmp(tmp->type, "Stmt") == 0))) {
				tmp->vt_syn = ast->vt_syn;
			}
			//Log("from %s to %s", ast->type, tmp->type);
			semantic_analyse(tmp);
		}

		//postorder
		if(ast->type == NULL) {
		} else if(strcmp(ast->type, "Program") == 0) { /* Program */
			check_undefined_func();
		} else if(strcmp(ast->type, "INT") == 0) { /* INT */
			gen_basic_vt(&(ast->vt_syn), 1);
		} else if(strcmp(ast->type, "FLOAT") == 0) { /* FLOAT */
			gen_basic_vt(&(ast->vt_syn), 2);
		} else if(strcmp(ast->type, "TYPE") == 0) { /* TYPE */
			if(strcmp(ast->val, "int") == 0) {
				gen_basic_vt(&(ast->vt_syn), 1);
			} else if(strcmp(ast->val, "float") == 0) {
				gen_basic_vt(&(ast->vt_syn), 2);
			}
		} else if(strcmp(ast->type, "ID") == 0) { /* ID */
			if(ast->is_var == 1) {
				if(query_locale_var(ast->val) == NULL) {
					add_var_to_symbol_table(ast->val, ast->vt_syn, 0);
				} else {
					if(in_struct == 0) {
						typeerror(3, ast->lineno, ast->val);
					} else if(in_struct == 1) {
						typeerror(15, ast->lineno, ast->val);
					}
				}
			}
		} else if(strcmp(ast->type, "Exp") == 0) { /* Exp */
			if((strcmp(ast->fc->type, "INT") == 0)
				|| (strcmp(ast->fc->type, "FLOAT") == 0)) {
				ast->vt_syn = ast->fc->vt_syn;

				// generate code
				/*ast->var_name = new_temp();
				list_push_back_InterCode(&(ast->code), gen_code(IC_ASSIGN, ast->var_name, ast->fc->val, NULL));
				Log("%s := #%s\n", ast->var_name, ast->fc->val);
				*/
			} else if(strcmp(ast->fc->type, "ID") == 0) {
				if(ast->fc->ns == NULL) { // variable reference
					ast->vt_syn = query_var(ast->fc->val);
					if(ast->vt_syn == NULL) {
						typeerror(1, ast->fc->lineno, ast->fc->val);
					}
					ast->fc->is_var = 1;
					ast->fc->vt_syn = ast->vt_syn;
					ast->is_var = ast->fc->is_var;

					/*ast->var_name = ast->fc->val;
					Log("ID: %s\n", ast->var_name);
					*/
				} else {	// function calling
					V_type vt = query_var(ast->fc->val);
					if(vt != NULL) typeerror(11, ast->fc->lineno, ast->fc->val);
					F_type ft = query_func(ast->fc->val, 0);
					F_type ft2 = query_func(ast->fc->val, 1);
					if(ft == NULL) {
						typeerror(2, ast->fc->lineno, ast->fc->val);
					} else if(ft2 == NULL) {
						typeerror(18, ast->fc->lineno, ast->fc->val);
					} else {
						if(args_match(ft->args, ast->fc->ns->ns->vt_list) == 0) {
							typeerror(9, ast->fc->ns->ns->lineno, ast->fc->val);
						}
						ast->vt_syn = ft->rtn_val;

						ast->fc->vt_syn = ft->rtn_val;
						ast->fc->vt_list = ft->args;
					}

					/*list_node_Item *args = ast->fc->ns->ns->vt_list;
					while(args != NULL) {
						list_push_back_InterCode(&(ast->code), gen_code(IC_ARG, args->data.name, NULL, NULL));
						Log("ARG %s\n", args->data.name);
						args = args->next;
					}
					ast->var_name = new_temp();
					list_push_back_InterCode(&(ast->code), gen_code(IC_ECALL, ast->var_name, ast->fc->val, NULL));
					Log("%s := CALL %s\n", ast->var_name, ast->fc->val);
					*/
				}
			} else if(strcmp(ast->fc->type, "Exp") == 0) {
				if(strcmp(ast->fc->ns->type, "ASSIGNOP") == 0) {
					if(type_match(ast->fc->vt_syn, ast->fc->ns->ns->vt_syn) == 0)
						typeerror(5, ast->fc->ns->lineno, NULL);
					if(ast->fc->is_var != 1)
						typeerror(6, ast->fc->lineno, NULL);
					ast->vt_syn = ast->fc->vt_syn;

					/*list_merge_InterCode(&(ast->code), ast->fc->ns->ns->code);
					list_push_back_InterCode(&(ast->code), gen_code(IC_ASSIGN, ast->fc->var_name, ast->fc->ns->ns->var_name, NULL));
					ast->var_name = new_temp();
					list_push_back_InterCode(&(ast->code), gen_code(IC_ASSIGN, ast->var_name, ast->fc->var_name, NULL));
					Log("%s := %s\n", ast->fc->var_name, ast->fc->ns->ns->var_name);
					Log("%s := %s\n", ast->var_name, ast->fc->var_name);
					*/
				} else if((strcmp(ast->fc->ns->type, "AND") == 0)
					|| (strcmp(ast->fc->ns->type, "OR") == 0)) {
					if(!(ast->fc->vt_syn && (ast->fc->vt_syn->kind == T_BASIC) &&
						(ast->fc->vt_syn->basic == T_INT) &&
						ast->fc->ns->ns->vt_syn && (ast->fc->ns->ns->vt_syn->kind == T_BASIC) &&
						(ast->fc->ns->ns->vt_syn->basic == T_INT))) {
						typeerror(7, ast->fc->lineno, NULL);

					}
					ast->vt_syn = ast->fc->vt_syn;

				} else if((strcmp(ast->fc->ns->type, "RELOP") == 0)
					|| (strcmp(ast->fc->ns->type, "PLUS") == 0)
					|| (strcmp(ast->fc->ns->type, "MINUS") == 0)
					|| (strcmp(ast->fc->ns->type, "STAR") == 0)
					|| (strcmp(ast->fc->ns->type, "DIV") == 0)) {
					if(!(ast->fc->vt_syn && (ast->fc->vt_syn->kind == T_BASIC) &&
						(type_match(ast->fc->vt_syn, ast->fc->ns->ns->vt_syn) == 1))) {
						typeerror(7, ast->fc->lineno, NULL);
					} 
					ast->vt_syn = ast->fc->vt_syn;

					/*list_merge_InterCode(&(ast->code), ast->fc->code);
					list_merge_InterCode(&(ast->code), ast->fc->ns->ns->code);
					ast->var_name = new_temp();
					if(strcmp(ast->fc->ns->type, "PLUS") == 0) {
						list_push_back_InterCode(&(ast->code), gen_code(IC_ADD, ast->var_name, ast->fc->var_name, ast->fc->ns->ns->var_name));
						Log("%s := %s + %s\n", ast->var_name, ast->fc->var_name, ast->fc->ns->ns->var_name);
					} else if(strcmp(ast->fc->ns->type, "MINUS") == 0) {
						list_push_back_InterCode(&(ast->code), gen_code(IC_SUB, ast->var_name, ast->fc->var_name, ast->fc->ns->ns->var_name));
						Log("%s := %s - %s\n", ast->var_name, ast->fc->var_name, ast->fc->ns->ns->var_name);
					} else if(strcmp(ast->fc->ns->type, "STAR") == 0) {
						list_push_back_InterCode(&(ast->code), gen_code(IC_MUL, ast->var_name, ast->fc->var_name, ast->fc->ns->ns->var_name));
						Log("%s := %s * %s\n", ast->var_name, ast->fc->var_name, ast->fc->ns->ns->var_name);
					} else if(strcmp(ast->fc->ns->type, "DIV") == 0) {
						list_push_back_InterCode(&(ast->code), gen_code(IC_DIV, ast->var_name, ast->fc->var_name, ast->fc->ns->ns->var_name));
						Log("%s := %s / %s\n", ast->var_name, ast->fc->var_name, ast->fc->ns->ns->var_name);
					} else if(strcmp(ast->fc->ns->type, "RELOP") == 0) {
					}
					*/
				} else if(strcmp(ast->fc->ns->type, "LB") == 0) {
					if(ast->fc->vt_syn && (ast->fc->vt_syn->kind != T_ARRAY)) {
						typeerror(10, ast->fc->lineno, ast->fc->fc->val);
					}
					if(!(ast->fc->ns->ns->vt_syn && (ast->fc->ns->ns->vt_syn->kind == T_BASIC) &&
						(ast->fc->ns->ns->vt_syn->basic == T_INT))) {
						typeerror(12, ast->fc->ns->ns->lineno, ast->fc->ns->ns->fc->val);
					}
					ast->vt_syn = ast->fc->vt_syn->array.elem;
					ast->is_var = 1;
				} else if(strcmp(ast->fc->ns->type, "DOT") == 0) {
					V_type vt1 = query_var(ast->fc->fc->val);
					if(vt1->kind != T_STRUCTURE) {
						typeerror(13, ast->fc->lineno, NULL);
					}

					list_node_Item *type = (vt1->structure).field, *res = NULL;
					res = list_find_Item(type, ast->fc->ns->ns->val);
					if(res == NULL) {
						typeerror(14, ast->fc->ns->ns->lineno, ast->fc->ns->ns->val); 
					} else {
						ast->vt_syn = (res->data).type;
						ast->is_var = 1;
					}
					/*
					V_type vt = query_var(ast->fc->ns->ns->val);
					if(vt == NULL) {
						typeerror(14, ast->fc->ns->ns->lineno, ast->fc->ns->ns->val); 
					} else {
						ast->vt_syn = vt;
						ast->is_var = 1;
					}
					*/
				}
			} else if(strcmp(ast->fc->type, "MINUS") == 0) {
				if(ast->fc->ns->vt_syn && (ast->fc->ns->vt_syn->kind != T_BASIC)) {
					typeerror(7, ast->fc->ns->lineno, NULL);
				}
				ast->vt_syn = ast->fc->ns->vt_syn;

				/*list_merge_InterCode(&(ast->code), ast->fc->ns->code);
				ast->var_name = new_temp();
				list_push_back_InterCode(&(ast->code), gen_code(IC_SUB, ast->var_name, "#0", ast->fc->ns->var_name));
				Log("%s := #0 - %s\n", ast->var_name, ast->fc->ns->var_name);
				*/
			} else if(strcmp(ast->fc->type, "NOT") == 0) {
				if(!(ast->fc->ns->vt_syn && (ast->fc->ns->vt_syn->kind == T_BASIC) &&
					(ast->fc->ns->vt_syn->basic == T_INT))) {
					typeerror(7, ast->fc->ns->lineno, NULL);
				}
				ast->vt_syn = ast->fc->ns->vt_syn;
			} else if(strcmp(ast->fc->type, "LP") == 0) {
				ast->vt_syn = ast->fc->ns->vt_syn;

				/*ast->var_name = ast->fc->ns->var_name;
				ast->code = ast->fc->ns->code;
				*/
			}
		} else if(strcmp(ast->type, "Specifier") == 0) { /* Specifier */
			ast->vt_syn = ast->fc->vt_syn;
		} else if(strcmp(ast->type, "StructSpecifier") == 0) { /* StructSpecifier */
			if((strcmp(ast->fc->ns->type, "OptTag") == 0) && ast->fc->ns->fc) {
				gen_struct_vt(&(ast->fc->ns->fc->vt_syn), ast->fc->ns->fc->val, ast->fc->ns->ns->ns->vt_list);
				if((query_type(ast->fc->ns->fc->val) == NULL) && (query_locale_var(ast->fc->ns->fc->val) == NULL)) {
					add_var_to_symbol_table(ast->fc->ns->fc->val, ast->fc->ns->fc->vt_syn, 1);
				} else {
					typeerror(16, ast->fc->ns->fc->lineno, ast->fc->ns->fc->val);
				}
				ast->vt_syn = ast->fc->ns->fc->vt_syn;
			} else if(strcmp(ast->fc->ns->type, "Tag") == 0) {
				V_type tvt = query_type(ast->fc->ns->fc->val);
				if(tvt == NULL) typeerror(17, ast->fc->ns->fc->lineno, ast->fc->ns->fc->val);
				else { ast->vt_syn = tvt; ast->fc->ns->vt_syn = tvt; }
			} else if(strcmp(ast->fc->ns->type, "LC") == 0) {
				gen_struct_vt(&(ast->vt_syn), NULL, ast->fc->ns->ns->vt_list);
			}
		} else if((strcmp(ast->type, "DefList") == 0) && ast->fc) { /* DefList */
			ast->vt_list = ast->fc->vt_list;
			if(ast->vt_list != NULL) {
				list_merge_Item(&(ast->vt_list), ast->fc->ns->vt_list);
			} else {
				ast->vt_list = ast->fc->ns->vt_list;
			}
		} else if(strcmp(ast->type, "Def") == 0) { /* Def */
			ast->vt_list = ast->fc->ns->vt_list;
		} else if(strcmp(ast->type, "DecList") == 0) { /* DecList */
			if(ast->fc->ns == NULL) {
				ast->vt_list = ast->fc->vt_list;
			} else {
				ast->vt_list = ast->fc->vt_list;
				list_merge_Item(&(ast->vt_list), ast->fc->ns->ns->vt_list);
			}
		} else if(strcmp(ast->type, "Dec") == 0) { /* Dec */
			ast->vt_list = ast->fc->vt_list;
			if(ast->fc->ns) {
				if(!type_match(ast->fc->vt_syn, ast->fc->ns->ns->vt_syn)) {
					typeerror(5, ast->fc->lineno, NULL);
				}
				if(in_struct == 1) {
					typeerror(15, ast->fc->lineno, ast->fc->fc->val);
				}
			}
		} else if(strcmp(ast->type, "VarDec") == 0) { /* VarDec */
			if(strcmp(ast->fc->type, "ID") == 0) {
				Item i;
				strcpy(i.name, ast->fc->val);
				i.type = ast->fc->vt_syn;
				list_push_back_Item(&(ast->vt_list), &i);
			} else if(strcmp(ast->fc->type, "VarDec") == 0) {
				ast->vt_list = ast->fc->vt_list;
			}
			ast->vt_syn = ast->fc->vt_syn;
		} else if(strcmp(ast->type, "ParamDec") == 0) { /* ParamDec */
			ast->vt_list = ast->fc->ns->vt_list;
		} else if((strcmp(ast->type, "VarList") == 0)) { /* VarList */
			if(ast->fc->ns == NULL) {
				ast->vt_list = ast->fc->vt_list;
			} else {
				list_push_back_Item(&(ast->vt_list), &(ast->fc->vt_list->data));
				list_merge_Item(&(ast->vt_list), ast->fc->ns->ns->vt_list);
			}
		} else if(strcmp(ast->type, "FunDec") == 0) { /* FunDec */
			F_type ft = query_func(ast->fc->val, 1);	// check if it has been defined
			if(ft != NULL) {
				if(ast->is_implem == 1) {
					typeerror(4, ast->fc->lineno, ast->fc->val);
				} else {
					if(args_match(ft->args, ast->fc->ns->ns->vt_list) == 0) {
						typeerror(19, ast->fc->lineno, ast->fc->val);
					} else {
						ast->fc->vt_list = ft->args;
					}
				}
			} else {
				F_type ft2 = query_func(ast->fc->val, 0);	// check if it has been externed
				if(ft2 != NULL) {
					if(args_match(ft2->args, ast->fc->ns->ns->vt_list) == 0) {
						typeerror(19, ast->fc->lineno, ast->fc->val);
					}
				}
				add_func_to_symbol_table(ast->fc->val, ast->vt_syn, ast->fc->ns->ns->vt_list, ast->is_implem, ast->lineno);
				ast->fc->vt_list = ast->fc->ns->ns->vt_list;
			}
			if(ast->is_implem == 0) {
				//dumpCurSymbTable();
				popSymbTable();
				//dumpCurSymbTable();
			}
		} else if(strcmp(ast->type, "Stmt") == 0) { /* Stmt */
			if(strcmp(ast->fc->type, "RETURN") == 0) {
				if(type_match(ast->vt_syn, ast->fc->ns->vt_syn) == 0) {
					typeerror(8, ast->fc->ns->lineno, NULL);
				}

				/*list_merge_InterCode(&(ast->code), ast->fc->code);
				list_push_back_InterCode(&(ast->code), gen_code(IC_RET, ast->fc->ns->var_name, NULL, NULL));
				Log("RETURN %s\n", ast->fc->ns->var_name);
				*/
			} else if(strcmp(ast->fc->type, "Exp") == 0) {
				//ast->code = ast->fc->code;
			} else if((strcmp(ast->fc->type, "IF") == 0) ||
				(strcmp(ast->fc->type, "WHILE") == 0)) {
				if(!(ast->fc->ns->ns->vt_syn && (ast->fc->ns->ns->vt_syn->kind == T_BASIC) &&
					(ast->fc->ns->ns->vt_syn->basic == T_INT))) {
					typeerror(7, ast->fc->ns->ns->lineno, NULL);
				}
			}
		} else if(strcmp(ast->type, "Args") == 0) { /* Args */
			Item i;
			i.name[0] = '\0';
			//strcpy(i.name, ast->fc->var_name);
			i.type = ast->fc->vt_syn;
			if(ast->fc->ns == NULL) {
				list_push_front_Item(&(ast->vt_list), &i);
			} else {
				ast->vt_list = ast->fc->ns->ns->vt_list;
				list_push_front_Item(&(ast->vt_list), &i);
			}
		}
	}
}



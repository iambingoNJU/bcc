
#include "../tree/bt.h"
#include "attr_handler.h"

int in_struct = 0;

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


nop_inh_helper(NOTYPE)

// terminal symbol
nop_inh_helper(INT)
nop_inh_helper(FLOAT)
nop_inh_helper(TYPE)
nop_inh_helper(ID)
nop_inh_helper(SEMI)
nop_inh_helper(COMMA)
nop_inh_helper(ASSIGNOP)
nop_inh_helper(RELOP)
nop_inh_helper(PLUS)
nop_inh_helper(MINUS)
nop_inh_helper(STAR)
nop_inh_helper(DIV)
nop_inh_helper(AND)
nop_inh_helper(OR)
nop_inh_helper(DOT)
nop_inh_helper(NOT)
nop_inh_helper(LP)
nop_inh_helper(RP)
nop_inh_helper(LB)
nop_inh_helper(RB)
nop_inh_helper(LC)
nop_inh_helper(RC)
nop_inh_helper(STRUCT)
nop_inh_helper(RETURN)
nop_inh_helper(IF)
nop_inh_helper(ELSE)
nop_inh_helper(WHILE)
nop_inh_helper(SPACE)
nop_inh_helper(COMMENT)
nop_inh_helper(COMMENT2)
// non-terminal symbol
nop_inh_helper(Program)
nop_inh_helper(ExtDefList)
nop_inh_helper(Specifier)
nop_inh_helper(OptTag)
nop_inh_helper(Tag)
nop_inh_helper(VarList)
nop_inh_helper(DefList)
nop_inh_helper(Exp)
nop_inh_helper(Args)

inh_helper(ExtDef) {
	if(child->type == TYPE_ExtDecList) {
		child->vt_syn = father->fc->vt_syn;
	} else if(child->type == TYPE_FunDec) {
		child->vt_syn = father->fc->vt_syn;
		if(child->ns->type == TYPE_CompSt) {
			child->ns->vt_syn = father->fc->vt_syn;
			child->is_implem = 1;
			child->ns->is_implem = 1;
		} else if(child->ns->type == TYPE_SEMI) {
			child->is_implem = 0;
			child->ns->is_implem = 0;
		}
	}

	return 0;
}

inh_helper(ExtDecList) {
	if((child->type == TYPE_VarDec) || (child->type == TYPE_ExtDecList)) {
		child->vt_syn = father->vt_syn;
	}

	return 0;
}

inh_helper(VarDec) {
	if(child->type == TYPE_ID) {
		child->vt_syn = father->vt_syn;
		child->is_var = 1;
	} else if(child->type == TYPE_VarDec) {
		child->vt_syn = gen_array_vt(father->vt_syn, (int)strtol(father->fc->ns->ns->val, NULL, 0)) ;
	}

	return 0;
}

inh_helper(FunDec) {
	if(child->type == TYPE_ID) {
		child->vt_syn = father->vt_syn;
		child->is_var = 0;
	} else if(child->type == TYPE_LP) {	// function arguments in child symbol table
		pushSymbTable();
	}

	return 0;
}

inh_helper(ParamDec) {
	if(child->type == TYPE_VarDec) {
		child->vt_syn = father->fc->vt_syn;
	}

	return 0;
}

inh_helper(Def) {
	if(child->type == TYPE_DecList) {
		child->vt_syn = father->fc->vt_syn;
	}

	return 0;
}

inh_helper(DecList) {
	if((child->type == TYPE_Dec) || (child->type == TYPE_DecList)) {
		child->vt_syn = father->vt_syn;
	}

	return 0;
}

inh_helper(Dec) {
	if(child->type == TYPE_VarDec) {
		child->vt_syn = father->vt_syn;
	}

	return 0;
}

inh_helper(CompSt) {
	if(child->type == TYPE_LC) { 
		if(father->is_implem == 0) {
			pushSymbTable();
		}
	} else if(child->type == TYPE_StmtList) {
		child->vt_syn = father->vt_syn;
	} else if(child->type == TYPE_RC) {
		//dumpCurSymbTable();
		popSymbTable();
		//dumpCurSymbTable();
	}

	return 0;
}

inh_helper(StructSpecifier) {
	if(child->type == TYPE_LC) {
		pushSymbTable();
		in_struct = 1;
	} else if(child->type == TYPE_RC) {
		//dumpCurSymbTable();
		popSymbTable();
		//dumpCurSymbTable();
		in_struct = 0;
	}

	return 0;
}

inh_helper(StmtList) {
	if(child->type == TYPE_Stmt) {
		child->vt_syn = father->vt_syn;
		father->fc->ns->vt_syn = father->vt_syn;
	}

	return 0;
}

inh_helper(Stmt) {
	if((child->type == TYPE_CompSt) || (child->type == TYPE_Stmt)) {
		child->vt_syn = father->vt_syn;
	}

	return 0;
}

/* All inherited attribute hander list */
inh_attr_handler inh_attr_handler_list[] = {
#define X(symbol) [TYPE_##symbol] = symbol##_inh_handler,
#include "../tree/token"
#undef X
};

nop_syn_helper(NOTYPE)

// terminal symbol
nop_syn_helper(SEMI)
nop_syn_helper(COMMA)
nop_syn_helper(ASSIGNOP)
nop_syn_helper(RELOP)
nop_syn_helper(PLUS)
nop_syn_helper(MINUS)
nop_syn_helper(STAR)
nop_syn_helper(DIV)
nop_syn_helper(AND)
nop_syn_helper(OR)
nop_syn_helper(DOT)
nop_syn_helper(NOT)
nop_syn_helper(LP)
nop_syn_helper(RP)
nop_syn_helper(LB)
nop_syn_helper(RB)
nop_syn_helper(LC)
nop_syn_helper(RC)
nop_syn_helper(STRUCT)
nop_syn_helper(RETURN)
nop_syn_helper(IF)
nop_syn_helper(ELSE)
nop_syn_helper(WHILE)
nop_syn_helper(SPACE)
nop_syn_helper(COMMENT)
nop_syn_helper(COMMENT2)

// non-terminal symbol
nop_syn_helper(ExtDefList)
nop_syn_helper(ExtDef)
nop_syn_helper(ExtDecList)
nop_syn_helper(OptTag)
nop_syn_helper(Tag)
nop_syn_helper(CompSt)
nop_syn_helper(StmtList)


syn_helper(Program) {
	check_undefined_func();

	return 0;
}

syn_helper(INT) {
	ast->vt_syn = gen_basic_vt(1);

	return 0;
}

syn_helper(FLOAT) {
	ast->vt_syn = gen_basic_vt(2);

	return 0;
}

syn_helper(TYPE) {
	if(strcmp(ast->val, "int") == 0) {
		ast->vt_syn = gen_basic_vt(1);
	} else if(strcmp(ast->val, "float") == 0) {
		ast->vt_syn = gen_basic_vt(2);
	}

	return 0;
}

syn_helper(ID) {
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

	return 0;
}

syn_helper(Exp) {
	if((ast->fc->type == TYPE_INT)
		|| (ast->fc->type == TYPE_FLOAT)) {
		ast->vt_syn = ast->fc->vt_syn;
	} else if(ast->fc->type == TYPE_ID) {
		if(ast->fc->ns == NULL) { // variable reference
			ast->vt_syn = query_var(ast->fc->val);
			if(ast->vt_syn == NULL) {
				typeerror(1, ast->fc->lineno, ast->fc->val);
			}
			ast->fc->is_var = 1;
			ast->fc->vt_syn = ast->vt_syn;
			ast->is_var = ast->fc->is_var;
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
		}
	} else if(ast->fc->type == TYPE_Exp) {
		if(ast->fc->ns->type == TYPE_ASSIGNOP) {
			if(type_match(ast->fc->vt_syn, ast->fc->ns->ns->vt_syn) == 0)
				typeerror(5, ast->fc->ns->lineno, NULL);
			if(ast->fc->is_var != 1)
				typeerror(6, ast->fc->lineno, NULL);
			ast->vt_syn = ast->fc->vt_syn;
		} else if((ast->fc->ns->type == TYPE_AND)
			|| (ast->fc->ns->type == TYPE_OR)) {
			if(!(ast->fc->vt_syn && (ast->fc->vt_syn->kind == T_BASIC) &&
				(ast->fc->vt_syn->basic == T_INT) &&
				ast->fc->ns->ns->vt_syn && (ast->fc->ns->ns->vt_syn->kind == T_BASIC) &&
				(ast->fc->ns->ns->vt_syn->basic == T_INT))) {
				typeerror(7, ast->fc->lineno, NULL);

			}
			ast->vt_syn = ast->fc->vt_syn;

		} else if((ast->fc->ns->type == TYPE_RELOP)
			|| (ast->fc->ns->type == TYPE_PLUS)
			|| (ast->fc->ns->type == TYPE_MINUS)
			|| (ast->fc->ns->type == TYPE_STAR)
			|| (ast->fc->ns->type == TYPE_DIV)) {
			if(!(ast->fc->vt_syn && (ast->fc->vt_syn->kind == T_BASIC) &&
				(type_match(ast->fc->vt_syn, ast->fc->ns->ns->vt_syn) == 1))) {
				typeerror(7, ast->fc->lineno, NULL);
			} 
			ast->vt_syn = ast->fc->vt_syn;
		} else if(ast->fc->ns->type == TYPE_LB) {
			if(ast->fc->vt_syn && (ast->fc->vt_syn->kind != T_ARRAY)) {
				typeerror(10, ast->fc->lineno, ast->fc->fc->val);
			}
			if(!(ast->fc->ns->ns->vt_syn && (ast->fc->ns->ns->vt_syn->kind == T_BASIC) &&
				(ast->fc->ns->ns->vt_syn->basic == T_INT))) {
				typeerror(12, ast->fc->ns->ns->lineno, ast->fc->ns->ns->fc->val);
			}
			ast->vt_syn = ast->fc->vt_syn->array.elem;
			ast->is_var = 1;
		} else if(ast->fc->ns->type == TYPE_DOT) {
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
	} else if(ast->fc->type == TYPE_MINUS) {
		if(ast->fc->ns->vt_syn && (ast->fc->ns->vt_syn->kind != T_BASIC)) {
			typeerror(7, ast->fc->ns->lineno, NULL);
		}
		ast->vt_syn = ast->fc->ns->vt_syn;
	} else if(ast->fc->type == TYPE_NOT) {
		if(!(ast->fc->ns->vt_syn && (ast->fc->ns->vt_syn->kind == T_BASIC) &&
			(ast->fc->ns->vt_syn->basic == T_INT))) {
			typeerror(7, ast->fc->ns->lineno, NULL);
		}
		ast->vt_syn = ast->fc->ns->vt_syn;
	} else if(ast->fc->type == TYPE_LP) {
		ast->vt_syn = ast->fc->ns->vt_syn;
	}

	return 0;
}


syn_helper(Specifier) {
	ast->vt_syn = ast->fc->vt_syn;

	return 0;
}

syn_helper(StructSpecifier) {
	if((ast->fc->ns->type == TYPE_OptTag) && ast->fc->ns->fc) {
		ast->fc->ns->fc->vt_syn = gen_struct_vt(ast->fc->ns->fc->val, ast->fc->ns->ns->ns->vt_list);
		if((query_type(ast->fc->ns->fc->val) == NULL) && (query_locale_var(ast->fc->ns->fc->val) == NULL)) {
			add_var_to_symbol_table(ast->fc->ns->fc->val, ast->fc->ns->fc->vt_syn, 1);
		} else {
			typeerror(16, ast->fc->ns->fc->lineno, ast->fc->ns->fc->val);
		}
		ast->vt_syn = ast->fc->ns->fc->vt_syn;
	} else if(ast->fc->ns->type == TYPE_Tag) {
		V_type tvt = query_type(ast->fc->ns->fc->val);
		if(tvt == NULL) typeerror(17, ast->fc->ns->fc->lineno, ast->fc->ns->fc->val);
		else { ast->vt_syn = tvt; ast->fc->ns->vt_syn = tvt; }
	} else if(ast->fc->ns->type == TYPE_LC) {
		ast->vt_syn = gen_struct_vt(NULL, ast->fc->ns->ns->vt_list);
	}

	return 0;
}

syn_helper(DefList) {
	if(ast->fc) {
		ast->vt_list = ast->fc->vt_list;
		if(ast->vt_list != NULL) {
			list_merge_Item(&(ast->vt_list), ast->fc->ns->vt_list);
		} else {
			ast->vt_list = ast->fc->ns->vt_list;
		}
	}

	return 0;
}

syn_helper(Def) {
	ast->vt_list = ast->fc->ns->vt_list;

	return 0;
}

syn_helper(DecList) {
	if(ast->fc->ns == NULL) {
		ast->vt_list = ast->fc->vt_list;
	} else {
		ast->vt_list = ast->fc->vt_list;
		list_merge_Item(&(ast->vt_list), ast->fc->ns->ns->vt_list);
	}

	return 0;
}

syn_helper(Dec) {
	ast->vt_list = ast->fc->vt_list;
	if(ast->fc->ns) {
		if(!type_match(ast->fc->vt_syn, ast->fc->ns->ns->vt_syn)) {
			typeerror(5, ast->fc->lineno, NULL);
		}
		if(in_struct == 1) {
			typeerror(15, ast->fc->lineno, ast->fc->fc->val);
		}
	}

	return 0;
}

syn_helper(VarDec) {
	if(ast->fc->type == TYPE_ID) {
		Item i;
		strcpy(i.name, ast->fc->val);
		i.type = ast->fc->vt_syn;
		list_push_back_Item(&(ast->vt_list), &i);
	} else if(ast->fc->type == TYPE_VarDec) {
		ast->vt_list = ast->fc->vt_list;
	}
	
	ast->vt_syn = ast->fc->vt_syn;

	return 0;
}

syn_helper(ParamDec) {
	ast->vt_list = ast->fc->ns->vt_list;

	return 0;
}

syn_helper(VarList) {
	if(ast->fc->ns == NULL) {
		ast->vt_list = ast->fc->vt_list;
	} else {
		list_push_back_Item(&(ast->vt_list), &(ast->fc->vt_list->data));
		list_merge_Item(&(ast->vt_list), ast->fc->ns->ns->vt_list);
	}

	return 0;
}

syn_helper(FunDec) {
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

	return 0;
}

syn_helper(Stmt) {
	if(ast->fc->type == TYPE_RETURN) {
		if(type_match(ast->vt_syn, ast->fc->ns->vt_syn) == 0) {
			typeerror(8, ast->fc->ns->lineno, NULL);
		}
	} else if(ast->fc->type == TYPE_Exp) {
	} else if((ast->fc->type == TYPE_IF) ||
		(ast->fc->type == TYPE_WHILE)) {
		if(!(ast->fc->ns->ns->vt_syn && (ast->fc->ns->ns->vt_syn->kind == T_BASIC) &&
			(ast->fc->ns->ns->vt_syn->basic == T_INT))) {
			typeerror(7, ast->fc->ns->ns->lineno, NULL);
		}
	}

	return 0;
}

syn_helper(Args) {
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

	return 0;
}


/* All synthesized attribute handler list */
syn_attr_handler syn_attr_handler_list[] = {
#define X(symbol) [TYPE_##symbol] = symbol##_syn_handler,
#include "../tree/token"
#undef X
};

void semantic_analyse(struct Node *ast) {
	if(ast != NULL) {
		//do recursively
		struct Node *tmp;
		for(tmp = ast->fc; tmp != NULL; tmp = tmp->ns) {
			//preorder
			inh_attr_handler_list[ast->type](ast, tmp);
			//Log("from %s to %s", ast->type, tmp->type);
			semantic_analyse(tmp);
		}

		//postorder
		syn_attr_handler_list[ast->type](ast);
	}
}



/*

*/

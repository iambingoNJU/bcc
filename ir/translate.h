#ifndef __TRANSLATE_H__
#define __TRANSLATE_H__

#include "ir.h"
#include "../tree/bt.h"

list_node_InterCode* translate_Exp(struct Node*, char* *place);
list_node_InterCode* translate_Stmt(struct Node*);
list_node_InterCode* translate_Cond(struct Node*, char *label_true, char *label_false);
list_node_InterCode* translate_Dec(struct Node* dec);
list_node_InterCode* translate_DecList(struct Node* list);
list_node_InterCode* translate_Def(struct Node* def);
list_node_InterCode* translate_DefList(struct Node* list);
list_node_InterCode* translate_StmtList(struct Node* list);
list_node_InterCode* translate_CompSt(struct Node*);
list_node_InterCode* translate_Args(struct Node*, list_node_Item **arg_list);
list_node_InterCode* translate_FunDec(struct Node* fun);
list_node_InterCode* translate_VarDec(struct Node* var);
list_node_InterCode* translate_ExtDecList(struct Node* list);
list_node_InterCode* translate_ExtDef(struct Node* extdef);
list_node_InterCode* translate_ExtDefList(struct Node* list);
list_node_InterCode* translate_Program(struct Node* prog);

#endif /* __TRANSLATE_H__ */

#ifndef __IR_H__
#define __IR_H__

#include "../debug.h"
#include "../semantic/clist.h"

#define TEMP_LEN 8
#define LABEL_LEN 10

/* intermediate code type */
typedef enum IC_TYPE {	IC_LABEL, IC_FUNC, IC_ASSIGN, 
						IC_ADD, IC_SUB, IC_MUL, IC_DIV,
						IC_EREF, IC_EDER, IC_DERE,
						IC_GOTO, IC_COND, IC_RET,
						IC_DEC, IC_ARG, IC_ECALL,
						IC_PARAM, IC_READ, IC_WRITE,
					 } IC_TYPE;
typedef struct InterCode {
	IC_TYPE kind;
	char *res, *op1, *op2, *op;
} InterCode;


/* code list */
DEFINE_LIST_NODE(InterCode)
EXTERN_LIST_PUSH_BACK(InterCode)
EXTERN_LIST_MERGE(InterCode)
EXTERN_LIST_FREE(InterCode)


char* new_temp();
char* new_label();

InterCode* gen_code(IC_TYPE code_type, char* res, char* op1, char* op2, char* op);
void print_code(InterCode* pic);
void print_code_list(list_node_InterCode *h);


#endif /* __IR_H__ */

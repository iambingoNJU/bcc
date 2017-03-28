#include "ir.h"

IMPLEMENT_LIST_PUSH_BACK(InterCode)
IMPLEMENT_LIST_MERGE(InterCode)
IMPLEMENT_LIST_FREE(InterCode)

char* new_temp() {
	static int tmp_i = 1;

	char *name = (char*)malloc(TEMP_LEN * sizeof(char));
	memset(name, 0, TEMP_LEN * sizeof(char));

	name[0] = 't';
	sprintf(name + 1, "%d", tmp_i);
	tmp_i ++;

	return name;
}


char* new_label() {
	static int label_i = 1;

	char *name = (char*)malloc(LABEL_LEN * sizeof(char));
	memset(name, 0, LABEL_LEN * sizeof(char));

	name[0] = 'l';
	name[1] = 'a';
	name[2] = 'b';
	name[3] = 'e';
	name[4] = 'l';
	sprintf(name + 5, "%d", label_i);
	label_i ++;

	return name;
}


InterCode* gen_code(IC_TYPE code_type, char* res, char* op1, char* op2, char* op) {
	InterCode *pic = (InterCode*)malloc(sizeof(struct InterCode));
	pic->kind = code_type;
	pic->res = res;
	pic->op1 = op1;
	pic->op2 = op2;
	pic->op = op;

	return pic;
}

void print_code(InterCode* pic) {
	Assert(pic, "Invalid code");
	switch(pic->kind) {
		case IC_LABEL:		printf("LABEL %s :\n", pic->res);							break;
		case IC_FUNC:		printf("\nFUNCTION %s :\n", pic->res);						break;
		case IC_ASSIGN:		printf("%s := %s\n", pic->res, pic->op1);					break;
		case IC_ADD:		printf("%s := %s + %s\n", pic->res, pic->op1, pic->op2);	break;
		case IC_SUB:		printf("%s := %s - %s\n", pic->res, pic->op1, pic->op2);	break;
		case IC_MUL:		printf("%s := %s * %s\n", pic->res, pic->op1, pic->op2);	break;
		case IC_DIV:		printf("%s := %s / %s\n", pic->res, pic->op1, pic->op2);	break;
		case IC_EREF:		printf("%s := &%s\n", pic->res, pic->op1);					break;
		case IC_EDER:		printf("%s := *%s\n", pic->res, pic->op1);					break;
		case IC_DERE:		printf("*%s := %s\n", pic->res, pic->op1);					break;
		case IC_GOTO:		printf("GOTO %s\n", pic->res);								break;
		case IC_COND:		printf("IF %s %s %s GOTO %s\n", pic->op1, pic->op, pic->op2, pic->res);				break;
		case IC_RET:		printf("RETURN %s\n", pic->res);							break;
		case IC_DEC:		printf("DEC %s %s\n", pic->res, pic->op1);					break;
		case IC_ARG:		printf("ARG %s\n", pic->res);								break;
		case IC_ECALL:		printf("%s := CALL %s\n", pic->res, pic->op1);				break;
		case IC_PARAM:		printf("PARAM %s\n", pic->res);								break;
		case IC_READ:		printf("READ %s\n", pic->res);								break;
		case IC_WRITE:		printf("WRITE %s\n", pic->res);								break;
		default:			printf("Invalid instruction.\n");
	}
}

void print_code_list(list_node_InterCode *h) {
	while(h != NULL) {
		print_code(&(h->data));
		h = h->next;
	}
}


#include "syntax.tab.h"
#include "tree/bt.h"

void yyrestart (FILE *input_file  );
int yyparse (void);

int main(int argc, char** argv)
{
	if(argc <= 1)	return 1;
	FILE* f = fopen(argv[1], "r");
	if(!f) {
		perror(argv[1]);
		return 1;
	}

	init_symbTable(curSymbTable);
	add_sys_func();

	yyrestart(f);
	yyparse();
	return 0;
}

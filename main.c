
#include "debug.h"
#include "syntax.tab.h"
#include "tree/bt.h"
#include "semantic/symbol_table.h"
#include "options.h"

void yyrestart (FILE *input_file  );
int yyparse (void);

Option options[] = {
	{ "-f", "--file", 0, NULL },
	{ "-ast", "--abstract-syntax-tree", 0, NULL },
	{ "-sa", "--semantic-analyse", 0, NULL },
	{ "-pcl", "--print-code-list", 0, NULL },
};

int main(int argc, char** argv) {

	for(int i = 1; i < argc; i ++) {
		unsigned j;
		for(j = 0; j < ARRAY_SIZE(options); j ++) {
			if((strcmp(argv[i], options[j].arg) == 0) ||
				   (strcmp(argv[i], options[j].opt) == 0))	{
				options[j].exist = 1;
				if(j == _F) {
					options[j].val = argv[++ i];
				}
				break;
			}
		}

		if(j == ARRAY_SIZE(options)) {
			printf("Invalid argument: %s\n", argv[i]);
		}
	}

	FILE *f = NULL;
	if(options[_F].exist) {
		f = fopen(options[_F].val, "r");
		assert(f);
	} else {
		f = stdin;
	}


	init_symbTable(curSymbTable);
	add_sys_func();

	yyrestart(f);
	yyparse();
	return 0;
}

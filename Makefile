CC := gcc
CFLAGS := -g -lfl -ly

INC_FILE := $(shell find . -name "*.h")
SRC_FILE := $(shell find . -path "./test" -prune -o -name "*.c" -print)
SRC_FILE += syntax.tab.c

TARGET := parser

$(TARGET) : lex syntax $(SRC_FILE) $(INC_FILE)
	$(CC) -o $@ $(SRC_FILE) $(CFLAGS)

lex: lexical.l
	flex lexical.l

syntax: syntax.y
	bison -d syntax.y

clean:
	-rm lex.yy.c
	-rm syntax.tab.*
	-rm $(TARGET)

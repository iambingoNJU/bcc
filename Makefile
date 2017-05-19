
CC := gcc
CFLAGS := -g -lfl -ly -Wall

INC := $(shell find . -name "*.h")
SRC := $(shell find . -path "./test" -prune -o -name "*.c" -print)
SRC += syntax.tab.c
#OBJ := $(subst .c,.o, $(SRC))
TARGET := bcc

.PHONY: clean count


$(TARGET): lex syntax $(SRC)
	$(CC) -o $@ $(SRC) $(CFLAGS)

lex: lexical.l
	flex lexical.l

syntax: syntax.y
	bison -d syntax.y

count:
	find . -name "*.c" -o -name "*.h" | xargs grep -v ^$$ | wc -l

clean:
	-rm lex.yy.c
	-rm syntax.tab.*
	-rm $(TARGET)
	-rm core

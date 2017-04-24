
#ifndef __OPTIONS_H__
#define __OPTIONS_H__

enum {
	_F,
	_AST,
	_SA,
	_PCL,
};

typedef struct {
	const char *arg;
	const char *opt;
	int exist;
	char *val;
} Option;

extern Option options[];

#endif /* __OPTIONS_H__ */

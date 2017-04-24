#ifndef __DEBUG_H__
#define __DEBUG_H__

//#define DEBUG

#include <stdio.h>
#include <assert.h>

#ifdef DEBUG
#define Log(format, ...) \
	do { \
		fprintf(stdout, "\33[1;34m[%s,%d,%s] " format "\33[0m\n", \
				__FILE__, __LINE__, __func__, ## __VA_ARGS__); \
		fflush(stdout); \
	} while(0)
#else
#define Log(format, ...)
#endif /* DEBUG */

#define Assert(cond, ...) \
	do { \
		if(!(cond)) { \
			fflush(stdout); \
			fprintf(stderr, "\33[1;31m"); \
			fprintf(stderr, __VA_ARGS__); \
			fprintf(stderr, "\33[0m\n"); \
			assert(cond); \
		} \
	} while(0)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#endif /* __DEBUG_H__ */

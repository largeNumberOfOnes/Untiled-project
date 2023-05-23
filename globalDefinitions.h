#include <assert.h>

#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#define TRUE 1
#define FALSE 0

#define errout(...) fprintf(stdout, __VA_ARGS__)

#define DOT { printf("\033[35mDOT: SOMEONE CALL ME FROM LINE: %d, %s, %s\033[39m\n", __LINE__, __FUNCTION__, __FILE__); }
#define CAP { printf("\033[31mCAP: SOMEONE CALL ME FROM LINE: %d, %s, %s\033[39m\n", __LINE__, __FUNCTION__, __FILE__); exit(0); }

#define MES(str) printf("\033[33mMES: %s\n\033[39m",str);


#endif

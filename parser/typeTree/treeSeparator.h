#include "treeElements.h"

#ifndef TREESEPARATOR_H
#define TREESEPARATOR_H


TypeTree* typeTree_treeSeparator(char* str);


#endif



//? Tests
// char* str = "Int->Float->Bool->(Int->Bool)->Cane(Int|Lenin)";
// char* str = "Int->Float->Bool->Array(Int)->Lenin";
// char* str = "Float->Float->Bool->Array(Int)->Lenin";
// char* str = "Char->Float->Bool->Array(Int)->Lenin";
// char* str = "Bool->Float->Bool->Array(Int)->Lenin";
// char* str = "Array->Float->Bool->Array(Int)->Lenin";
// char* str = "Array(Int)->Float->Bool->Array(Int)->Lenin";

// char* str = "Array()->Float->Bool->Array(Int)->Lenin";
// char* str = "Array(Array(Char))   ->   Float->Bool->Array(Bool)->Bool";
// char* str = "Array()   - >   Float->Bool->Array(Int)->Lenin";
// char* str = "Array()   -u  Float->Bool->Array(Int)->Lenin";
// char* str = "Array()";

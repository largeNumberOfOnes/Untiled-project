#include <stddef.h>
#include <stdio.h>
#include "../../globalDefinitions.h"

#ifndef TREEELEMENTS_H
#define TREEELEMENTS_H

//? ###########################################################################
//? ###########################################################################

// typedef struct root_t {
// 
// } Root;
// 
// typedef struct define_t {
// 
// } Define;

//? ###########################################################################
//? ###########################################################################

// main struct
typedef struct typeTree_t {
    
    int entity;
    size_t type;
    struct typeTree_t* child;

} TypeTree;

// entity values in TypeTree
enum {
    SIMPLE_ENTITY,
    STRUCT_ENTITY,
    ARRAY_ENTITY,
    CANE_ENTITY,
    LIST_ENTITY,
    LENIN_ENTITY,
    TREE_ENTITY,
    UNDEF_ENTITY,
};
// type values in TypeTree
enum {
    INT_TYPE,
    FLOAT_TYPE,
    CHAR_TYPE,
    BOOL_TYPE,
    UNDEF_TYPE,
};

// functions
TypeTree* typeTree_createFromString(char* str);
char* typeTree_convertToString(TypeTree* str);
int typeTree_print_graphviz_body(FILE* file, TypeTree* tree);
int typeTree_print_graphviz(char* filePath, TypeTree* tree);
int typeTree_print_file(FILE* file, TypeTree* tree);

int typeTree_detEntity(char* str, size_t len);
size_t typeTree_detType(char* str, size_t len);

char* typeTree_getEntityString(int entity);
char* typeTree_getTypeString(size_t type);

TypeTree* typeTree_init(int entity, unsigned long type);

int typeTree_isEqual(TypeTree *tree1, TypeTree *tree2);

//? ###########################################################################
//? ###########################################################################

TypeTree* deleteArrayTypeField(TypeTree* tree);
TypeTree* typeTree_deleteTree(TypeTree *tree);
TypeTree* typeTree_getLast(TypeTree *tree);
TypeTree* typeTree_deleteLast(TypeTree *tree);
TypeTree* typeTree_copy(TypeTree *tree);

int typeTree_getHeigh(TypeTree *tree);
TypeTree* typeTree_getArg(TypeTree *tree, int number);

int typeTree_putToBottom(TypeTree *tree, TypeTree *arg);

#endif

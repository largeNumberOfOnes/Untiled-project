
#include "../typeTree/treeElements.h"
#include "../scanner/scanner_structs.h"

#include "../typeTree/treeElements.h"
#include "../typeTree/treeSeparator.h"
#include <stddef.h>

// root --*---> main
//        *---> def

#ifndef TREE_STRUCT_H
#define TREE_STRUCT_H


//? ###########################################################################
//? ###########################################################################

typedef struct exptree_pair_t {

    struct exptree_t *func;
    struct exptree_t *arg;

} Pair;

typedef struct exptree_ifexptree_t {

    struct exptree_t *cond;
    struct exptree_t *arg1;
    struct exptree_t *arg2;

} Ifexptree;

typedef struct exptree_t {

    int type;

    size_t content;
    TypeTree *elemType;

} Exptree;

enum EXPTREE_TYPE_ENUM {
    EXPTREE_TYPE_APPLY,
    EXPTREE_TYPE_ELEM,
    EXPTREE_TYPE_VAR,
    EXPTREE_TYPE_FUNCTION,
    EXPTREE_TYPE_OPERATOR,
    EXPTREE_TYPE_IF,
};

Pair* Pair_init(Exptree *func, Exptree *arg);

int exptree_printToFile(Exptree *tree, char* filePath);
int exptree_printToFile_body(Exptree *tree, FILE *file);
int exptree_getPriority(Exptree *tree);

//? ###########################################################################
//? ###########################################################################

enum FUNCTION_NUTAPP_ENUM {
    FUNCTION_NUTAPP_FUNCTION,
    FUNCTION_NUTAPP_OPERATOR
};

typedef struct function_t {
    char* name;
    int argsCount;
    char** argNames;
    int priority;
    int nutapp;
    TypeTree *typeTree;
    char *typetreeString;
    Exptree *tree;
} Function;

Function* function_declaration(Token *token);
int function_isOper(Function * func);

int function_dump(Function *func, FILE *file);

//? ###########################################################################
//? ###########################################################################

typedef struct deflist_t {

    struct deflist_t *next;
    struct deflist_t *prev;

    Function *func;
    size_t line;

} Deflist;

Deflist* deflist_init(Function* func);
int deflist_append(Deflist* defList, Function* func);
Deflist* deflist_search(Deflist *deflist, char *name);

Deflist* fenceToDefList(Fence *fence);

Deflist* deflist_iteratorInit(Deflist *deflist);
Deflist* deflist_iteratorNext(Deflist *deflist);

//? ###########################################################################
//? ###########################################################################

typedef struct root_t {

    Deflist* deflist;

} Root;

Root* root_init();
int root_printToFile(Root *root, char *filePath);

//? ###########################################################################
//? ###########################################################################

int* updateVariablesIntoFunctions(Fence *fence, Root *root);

//? ###########################################################################
//? ###########################################################################

typedef struct scope_t {

    struct scope_t *next;
    struct scope_t *prev;

    Deflist *deflist;

} Scope;

int       scope_isInScope(Scope *scope, char* str);
TypeTree* scope_getType(Scope *scope, char* str);
int       scope_getPriority(Scope *scope, char* str);
Scope*    scope_fromDeflist(Deflist *deflist);
Function* scope_getFunction(Scope *scope, char *str);
Scope*    scope_init(Deflist *deflist);
Scope*    scope_delete(Scope *scope);

//? ###########################################################################
//? ###########################################################################

int exptree_dump(Exptree *tree, FILE *file);

int deflist_dump(Deflist *deflsit, FILE *file);
int scope_dump(Scope *scope, FILE *file);

int defineFunctions(Fence *fence, Root *root);

//? ###########################################################################
//? ###########################################################################

#endif // TREE_STRUCT_H


#include "tree_structs.h"
#include "../scanner/tokenTypesEnum.h"
#include "../scanner/scanner_structs.h"

// #include "../typeTree/treeElements.h"
#include "../typeTree/treeElements.h"
#include "../typeTree/treeSeparator.h"

#include "expressionAnalizer.h"

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Function* function_declaration(Token *token) {

    assert(token->type == TOKEN_TYPE_SOVEREIGN_def);
    Function *fn = (Function*) malloc(sizeof(Function));

    token = token->next;
    assert(token->type == TOKEN_TYPE_VARIABLE ||
           token->type == TOKEN_TYPE_OPERATOR ||
           token->type == TOKEN_TYPE_FUNCTION
    );
    if (token->type == TOKEN_TYPE_VARIABLE ||
        token->type == TOKEN_TYPE_FUNCTION
    ) {
        fn->nutapp = FUNCTION_NUTAPP_FUNCTION;
    } else {
        fn->nutapp = FUNCTION_NUTAPP_OPERATOR;
    }

    fn->name = token->string;

    token = token->next;
    Token *argsStart = token;
    int argsCount = 0;
    while (token->type == TOKEN_TYPE_VARIABLE) {
        token = token->next;
        ++argsCount;
    }
    DOT
    fn->argNames = (char**) malloc(sizeof(char*)*argsCount);
    fn->argsCount = argsCount;
    for (size_t q = 0; argsStart != token; argsStart = argsStart->next, ++q) {
        fn->argNames[q] = strdup(argsStart->string);
    }
    assert(argsStart == token);

    assert(token->type == TOKEN_TYPE_OPERATOR);
    assert(!strcmp(token->string, ":"));

    token = token->next;
    assert(token->type == TOKEN_TYPE_INT);
    fn->priority = atoi(token->string);
    // fn->priority

    token = token->next;
    assert(token->type == TOKEN_TYPE_OPERATOR);
    assert(!strcmp(token->string, ":"));
    
    token = token->next;
    assert(token->type == TOKEN_TYPE_TYPE);
    // fn->typeTree = typeTree_createFromString(token->string);
    fn->typetreeString = token->string;
    fn->typeTree = typeTree_treeSeparator(token->string);

    fn->tree = NULL;

    return fn;
}

enum FUNCTION_DECL_OVERLAP {
    EQUAL = 0,
    NO_NAMES,
    NO_ARGCOUNTS,
    NO_ARGNAMES,
    NO_NUTAPP,
    NO_PRIORITY,
    NO_TYPETREE,
};

enum FUNCTION_DECL_OVERLAP function_isFunctionDeclEqual(Function *fn1, Function *fn2) {

    if (strcmp(fn1->name, fn2->name)) return NO_NAMES;

    if (fn1->argsCount != fn2->argsCount) return NO_ARGCOUNTS;

    for (int q = 0; q < fn1->argsCount; ++q) {
        if (strcmp(fn1->argNames[q], fn2->argNames[q])) return NO_ARGNAMES;
    }

    if (fn1->nutapp != fn2->nutapp) return NO_NUTAPP;
    if (fn1->priority != fn2->priority) return NO_PRIORITY;
    
    //fn1->typeTree // devel

    return EQUAL;    
}

Function* function_definition(Token *token, Root *root) {
// token_printToFile(token, stdout);
    assert(token != NULL);
    assert(root != NULL);

    assert(token->type == TOKEN_TYPE_SOVEREIGN_def);

    Function *fnTest = function_declaration(token);

    assert(fnTest != NULL);

    Deflist *deflist = deflist_search(root->deflist, token->next->string);
    assert(deflist != NULL);
    Function *func = deflist->func;
    if (function_isFunctionDeclEqual(func, fnTest) != EQUAL) {
        printf("MES FROM--> %d\n", function_isFunctionDeclEqual(func, fnTest));
        CAP
    }

DOT
    while (strcmp(token->string, "=")){
        
        if (token->next == NULL) {
            DOT
            return NULL;
        }
        // printf("--> %s\n", token->string);
        token = token->next;
    }
    printf("--> %p\n", token);
    token = token->next;
    printf("--> %s\n", token->string);
    printf("--> %d\n", token->type);
DOT

    Scope *scope = scope_init(deflist_search(root->deflist, func->name));
    func->tree = expression(&token, scope);
    scope = scope_delete(scope);
    assert(func != NULL);

    return NULL;
}

int defineFunctions(Fence *fence, Root *root) {

    while (fence->prev) fence = fence->prev;

    // CAP

    while (fence) {

        if (!strcmp(fence->token->string, "def")) {
            DOT
            function_definition(fence->token, root);
        }

        fence = fence->next;
    }

    return 0;
}

//? ###########################################################################
//? ###########################################################################

Deflist* deflist_init(Function* func) {

    Deflist* defList = (Deflist*) malloc(sizeof(Deflist));

    defList->next = NULL;
    defList->prev = NULL;

    defList->func = func;

    return defList;
}

Deflist* deflist_search(Deflist *deflist, char *name) {

    while (deflist->prev) deflist = deflist->prev;

    while (deflist) {
        // printf("─┬─🢒 %s\n", name);
        // printf(" ╰─🢒 %s\n", deflist->func->name);
        if (!strcmp(deflist->func->name, name)) {
            return deflist;
        }
        deflist = deflist->next;
    }

    return NULL;
}

int deflist_append(Deflist* deflist, Function* func) {
DOT
    while (deflist->next) deflist = deflist->next;
DOT

    Deflist* newdeflist = deflist_init(func);

    deflist->next = newdeflist;
    newdeflist->prev = deflist;

    return 0;
}

int* updateVariablesIntoFunctions_(Token *token, Root *root) {

    while (token->prev) token = token->prev;

    while (token) {
        if ((token->type == TOKEN_TYPE_VARIABLE) &&
            (deflist_search(root->deflist, token->string))
        ) {
            token->type = TOKEN_TYPE_FUNCTION;
        }
        token = token->next;
    }

    return 0;
}

int* updateVariablesIntoFunctions(Fence *fence, Root *root) {

    while (fence->prev) fence = fence->prev;

    while (fence) {
        updateVariablesIntoFunctions_(fence->token, root);
        fence = fence->next;
    }

    return 0;
}

Deflist* fenceToDefList(Fence *fence) {

    Deflist* deflist = NULL;

    while (1) {
        DOT
        if (!fence) {
            return deflist;
            DOT
        }

        if (fence->token->type == TOKEN_TYPE_SOVEREIGN_def) {
            DOT
            if (deflist) {
            DOT
                deflist_append(deflist, function_declaration(fence->token));
            } else {
            DOT
                deflist = deflist_init(function_declaration(fence->token));
            }
        }

        fence = fence->next;
    }
}

int deflist_dump(Deflist *deflist, FILE *file) {
    
    while (deflist->prev) deflist = deflist->prev;

    while (deflist) {

        function_dump(deflist->func, file);
        fprintf(file, "\n\n");

        deflist = deflist->next;
    }

    return 0;
}

// int function_print(Function *func, FILE *file) {

//     fprintf(file, "%lu [shape=record, label=\"{Function|name: %s|argsCount: %d|", func, func->name);
//     for (int q = 0; q < func->argsCount; ++q) {

//     }
//     fprintf()

//         char* name;
//     int argsCount;
//     char** argNames;
//     int priority;
//     TypeTree *typeTree;
//     char *typetreeString;
//     ExprTree *tree;

// }

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree_structs.h"

Deflist* scope_search(Scope *scope, char* str) {

    assert(scope != NULL); // dev
    assert(str != NULL); // dev

    for (int q = 0; q < scope->deflist->func->argsCount; ++q) {
        if (!strcmp(str, scope->deflist->func->argNames[q])) {
            return scope->deflist;
        }
    }
    
    Scope *stepper = scope->next;
    while (stepper) {
        Deflist* ret = deflist_search(scope->deflist, str);
        if (ret) return ret;
        stepper = stepper->prev;
    }

    stepper = scope;
    while (stepper) {
        Deflist* ret = deflist_search(scope->deflist, str);
        if (ret) return ret;
        stepper = stepper->next;
    }

    return NULL;
}

Function* scope_getFunction(Scope *scope, char *str) {

    Deflist* ret = scope_search(scope, str);
    if (!ret) {
        // CAP // dev // mo such function in scope
        MES("no such function in scope");
    }

    return scope_search(scope, str)->func;
}

int scope_isInScope(Scope *scope, char* str) {

    return (scope_search(scope, str) != NULL);
}

TypeTree* scope_getType(Scope *scope, char* str) {
DOT
    assert(scope != NULL);
    assert(str != NULL);

    printf("str: %s\n", str);
    printf("--> %s\n", scope->deflist->func->name);
    Deflist *deflist = scope_search(scope, str);
    // assert(deflist != NULL);
    if (!deflist) return NULL;

    if (!strcmp(deflist->func->name, str)) {
        DOT
        return deflist->func->typeTree;
    } else {
        // char **list
        DOT
        for (int q = 0; q < deflist->func->argsCount; ++q) {
            if (!strcmp(deflist->func->argNames[q], str)) {
                // printf("--> %p\n", tree->child);
                return typeTree_getArg(deflist->func->typeTree, q);
            }
        }

        // CAP // dev
    }
DOT
    return NULL;
}

Scope* scope_init(Deflist *deflist) {

    assert(deflist != NULL);

    Scope *scope = (Scope*) malloc(sizeof(Scope));

    scope->deflist = deflist;
    scope->next = NULL;
    scope->prev = NULL;

    return scope;
}

int scope_append(Scope *scope, Deflist *deflist) {

    assert(scope != NULL);

    Scope *new = scope_init(deflist);
    assert(new != NULL);

    scope->next = new;
    new->prev = scope;

    return 0;
}


int scope_dump(Scope *scope, FILE *file) {

    assert(scope->prev == NULL);

    while (scope) {

        deflist_dump(scope->deflist, file);
        fprintf(file, "\n\n");

        scope = scope->next;
    }

    return 0;
}

Scope* scope_delete(Scope *scope) {

    while (scope->prev) scope = scope->prev;

    while (scope) {
        Scope *next = scope->next;
        free(scope);
        scope = next;
    }

    return NULL;
}
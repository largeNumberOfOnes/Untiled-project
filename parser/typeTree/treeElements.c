#include "treeElements.h"
#include "../../globalDefinitions.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>

#include <string.h>

#include <stdlib.h>

TypeTree* typeTree_init(int entity, unsigned long type) {

    TypeTree* tree = (TypeTree*) malloc(sizeof(TypeTree));
    tree->entity = entity;
    tree->type   = type;
    tree->child = NULL;

    return tree;
}

TypeTree* typeTree_append(TypeTree* child, int entity, unsigned long type) {

    TypeTree* tree = typeTree_init(entity, type);
    tree->child = child;

    return tree;
}


int typeTree_detEntity(char* str, size_t len) {
    printf("str: %.10s\n", str);
    if (!strncmp(str, "Struct", len)) {
        return STRUCT_ENTITY;
    }
    if (!strncmp(str, "Array", len)) {
        return ARRAY_ENTITY;
    }
    if (!strncmp(str, "Cane", len)) {
        return CANE_ENTITY;
    }
    if (typeTree_detType(str, len) != UNDEF_TYPE) {
        return SIMPLE_ENTITY;
    }

    return UNDEF_ENTITY;
}

size_t typeTree_detType(char* str, size_t len) {

    if (!strncmp(str, "Int", len)) {
        return INT_TYPE;
    }
    if (!strncmp(str, "Float", len)) {
        return FLOAT_TYPE;
    }
    if (!strncmp(str, "Bool", len)) {
        return BOOL_TYPE;
    }
    if (!strncmp(str, "Char", len)) {
        return CHAR_TYPE;
    }
    return UNDEF_TYPE;

}


char* typeTree_getEntityString(int entity) {
    switch (entity) {
        case 0: return "SIMPLE";
        case 1: return "STRUCT";
        case 2: return "ARRAY";
        case 3: return "CANE";
        case 4: return "LIST";
        case 5: return "LENIN";
        case 6: return "TREE";
        case 7: return "UNDEF";
        default:
            return NULL;
    }
}
char* typeTree_getTypeString(size_t type) { // dev
    switch (type) {
        case 0: return "Int";
        case 1: return "Float";
        case 2: return "Char";
        case 3: return "Bool";
        case 4: return "UNDEF";
        default:
            return NULL;
    }
}

int typeTree_print_graphviz_body(FILE* file, TypeTree* tree) {

    int details = 1;

    if (!tree) {
        return 0;
    }

    char* entity = typeTree_getEntityString(tree->entity);
    int isClearingEntityNeaded = entity ? 0 : 1;
    if (!entity) {
        entity = (char*) malloc(20*sizeof(char));
        sprintf(entity, "%d", tree->entity);
    }

    char* type = typeTree_getTypeString(tree->type);
    int isClearingTypeNeaded = type ? 0 : 1;
    if (!type) {
        type = (char*) malloc(20*sizeof(char));
        sprintf(type, "%lu", tree->type);
    }
        

    fprintf(file, 
        "\t%lu [shape=record, label=\"{ \n\t\t{ TypeTree | %p } | \n\t\t{ entity | %s } | \n\t\t{ type | <01> %s } | \n\t\t{ child | <02> %p } \n\t}\"]\n",
        (unsigned long) tree,
        tree,
        entity,
        type,
        tree->child
    );

    if (isClearingEntityNeaded) {
        free(entity);
    }
    if (isClearingTypeNeaded) {
        free(type);
    }

    if (tree->entity == ARRAY_ENTITY) {
        typeTree_print_graphviz_body(file, (TypeTree*)(tree->type));
        fprintf(file, "\t%lu:<01> -> %lu\n", (unsigned long)tree, (unsigned long)(tree->type));
    }
    if (tree->child) {
        typeTree_print_graphviz_body(file, tree->child);
        fprintf(file, "\t%lu:<02> -> %lu\n", (unsigned long)tree, (unsigned long)(tree->child));
    }


    return 0;
}

int typeTree_print_graphviz(char* filePath, TypeTree* tree) {

    FILE* file = fopen(filePath, "w+");

    fprintf(file, "digraph {\n");

    typeTree_print_graphviz_body(file, tree);

    fprintf(file, "}");

    fclose(file);

    return 0;
}

int typeTree_print_file(FILE* file, TypeTree* tree) {

    fprintf(file,
        "# address: %p\n# entity: %d\n# type: %lu\n# child: %p\n",
        tree,
        tree->entity,
        tree->type,
        tree->child
    );

    return 0;
}

int typeTree_isEqual(TypeTree *tree1, TypeTree *tree2) {

    if ((tree1 == NULL) && (tree2 == NULL)) return TRUE;

    if (tree1->entity != tree2->entity) return FALSE;

    switch (tree1->entity) {
        case SIMPLE_ENTITY:
            if (tree1->type != tree2->type) return FALSE;
        case ARRAY_ENTITY:
            if (!typeTree_isEqual(
                (TypeTree*)(tree1->type),
                (TypeTree*)(tree2->type)
            )) return FALSE;
    }

    if (!typeTree_isEqual(tree1->child, tree2->child)) return FALSE;

    return TRUE;
}

//? ###########################################################################
//? ###########################################################################

TypeTree* deleteArrayTypeField(TypeTree* tree) {

    if (!tree) return NULL;

    return typeTree_deleteTree((TypeTree*)(tree->type));
}

TypeTree* typeTree_deleteTree(TypeTree *tree) {

    if (!tree) return NULL;

    tree->child = typeTree_deleteTree(tree->child);

    switch (tree->entity) {
        case SIMPLE_ENTITY:
            break;
        case ARRAY_ENTITY:
            deleteArrayTypeField(tree);
            break;
        default:
            CAP // dev
    }

    free(tree);

    return NULL;
}

TypeTree* typeTree_getLast(TypeTree *tree) {

    while (tree->child) tree = tree->child;

    return tree;
}

TypeTree* typeTree_deleteLast(TypeTree *tree) {

    if (!tree) return NULL;

    if (!(tree->child)) return typeTree_deleteTree(tree);

    while (tree->child->child) tree = tree->child;

    tree->child = typeTree_deleteTree(tree->child);
    
    return NULL;
}

TypeTree* typeTree_copy(TypeTree *tree) {

    if (!tree) return NULL;

    TypeTree* new = typeTree_init(tree->entity, tree->type);

    switch (tree->entity) {
        case SIMPLE_ENTITY:
            break;
        case ARRAY_ENTITY:
            new->type = (size_t) typeTree_copy((TypeTree*)(tree->type));
            break;
        default:
            printf("With entity: %d\n", tree->entity);
            CAP // dev
    }

    new->child = typeTree_copy(tree->child);

    return new;
}

int typeTree_getHeigh(TypeTree *tree) {

    assert(tree != NULL); // dev

    int ret = 0;

    // while (tree->child != NULL) {
    while (tree) {
        ++ret;
        tree = tree->child;
    }

    return ret;
}

TypeTree* typeTree_getArg(TypeTree *tree, int number) {
    assert(tree != NULL); // dev
    printf("--> %d\n", number);

    printf("--> %p\n", tree->child);
DOT
    int newCount = typeTree_getHeigh(tree);
    assert(number < newCount); // dev   
    newCount = newCount - number - 1;
    assert(newCount > 0); // dev

    for (int q = 0; q < newCount; ++q) {
        tree = tree->child;
    }

    assert(tree != NULL); // dev

    TypeTree *ret = typeTree_copy(tree);

    typeTree_deleteTree(ret->child);
    ret->child = NULL;
DOT

    return ret;
}

//? ###########################################################################
//? ###########################################################################

static char* convertToString_simple(TypeTree* tree) {
    // IT SHOULD RETURN ONLY CONST STRINGS

    assert(tree != NULL);
    assert(tree->entity == SIMPLE_ENTITY);

    if (tree->type == INT_TYPE) return "Int";
    else if (tree->type == FLOAT_TYPE) return "Float";
    else if (tree->type == CHAR_TYPE) return "Char";
    else return NULL;
}

char* typeTree_convertToString(TypeTree* tree) {

    assert (tree != NULL); // dev

    char *str = (char*) malloc(1);
    str[0] = '\0';
    size_t length = 1;

    if  (tree->entity == SIMPLE_ENTITY) {
        char *temp = convertToString_simple(tree);
        // printf("--> %lu", strlen(temp));
        str = (char*) realloc(str, 2);
        // str = (char*) realloc(str, (strlen(str) + strlen(temp)));
        strcat(str, temp);
        
        if (tree->child) {
            temp = typeTree_convertToString(tree->child);
            str = (char*) realloc(str, strlen(str) + 2lu + strlen(temp));
            strcat(str, "->");
            strcat(str+2, temp);
            free(temp);
        }
        
        return str;
    } else {
        CAP // dev
    }

    return NULL;
}

int typeTree_putToBottom(TypeTree *tree, TypeTree *arg) {

    while (tree->child) tree = tree->child;

    tree->child = arg;

    return 0;
}

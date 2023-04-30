
#include "tree_structs.h"
#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "printf.h"

Root* root_init() {

    Root *root = (Root*) malloc(sizeof(Root));
    memset(root, 0, sizeof(Root));

    return root;
}

int root_printToFile(Root *root, char *filePath) {

    assert(root != NULL);
    assert(filePath != NULL);

    FILE *file = fopen(filePath, "w+");

    fprintf(file, "digraph {\n\n");
    fprintf(file, "\trankdir=TB\n");

    fprintf(file, "\troot\n");
    
    Deflist *deflist = root->deflist;
    while (deflist->prev) deflist = deflist->prev;
    
    fprintf(file, "\t{rank=same\n");

    while (deflist) {

        fprintf(file, "\t\t%lu [shape=rectangle, style=\"filled\",fillcolor=\"#900C3F\", label=\"Deflist\\n", (size_t) deflist);
        fprintf(file, "%s\\n", deflist->func->name);
        fprintf(file, "argCount = %d\\n", deflist->func->argsCount);
        fprintf(file, "args:\\n");
        for (int q = 0; q < deflist->func->argsCount; ++q) {
            fprintf(file, "--> %s\\n", deflist->func->argNames[q]);
        }
        fprintf(file, "priority = %d\\n", deflist->func->priority);
        fprintf(file, "%s\\n", deflist->func->typetreeString);
        fprintf(file, "\"]\n");
        fprintf(file, "\t\troot -> %lu\n\n", (size_t) deflist);

        deflist = deflist->next;
    }

    fprintf(file, "\t}\n");

    deflist = root->deflist;
    while (deflist->prev) deflist = deflist->prev;
    while (deflist) {

        typeTree_print_graphviz_body(file, deflist->func->typeTree);
        fprintf(file, "\t\t%lu -> %lu\n", (size_t) deflist, (size_t) (deflist->func->typeTree));

        if (deflist->func->tree) {
            exptree_printToFile_body(deflist->func->tree, file);
            fprintf(file, "\t\t%lu -> %lu\n", (size_t) deflist, (size_t) (deflist->func->tree));
        }

        deflist = deflist->next;
    }

    fprintf(file, "\n}\n");

    fclose(file);

    return 0;
}





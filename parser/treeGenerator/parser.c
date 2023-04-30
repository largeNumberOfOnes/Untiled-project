


#include "../scanner/scanner.h"

#include "tree_structs.h"

#include "parser.h"

#include "expressionAnalizer.h"

#include <stdlib.h>


Root* parser(Fence *fence) {

    assert(fence != NULL);

    Root *root = root_init();

    root->deflist = fenceToDefList(fence);

    updateVariablesIntoFunctions(fence, root);

    DOT
    MES("----------------------------------------------------");
    defineFunctions(fence, root);

    // while (fence->next->next) fence = fence->next;
    // DOT

    // Token *token = fence->token;
    // Scope *scope = scope_init(root->deflist);
    // Scope *scope = scope_init(deflist_search(root->deflist, "workWithFour"));
    // Scope *scope = scope_init(deflist_search(root->deflist, "thatSoundsScary"));
    // token_printToFile(token, stdout);

    // for (int q = 0; q < 11; ++q) { // for workWithFour
    // for (int q = 0; q < 12; ++q) { // for thatSoundsScary
    //     token = token->next;
    // }
    // printf("token: %s\n", token->string);
    // CAP

    // FILE *file = fopen("HHHHHHHHHHHHHHHHHHHH.txt", "w+"); // deb
    // scope_dump(scope, file); // deb
    // fclose(file); // deb

    // printf("----------------------------------------------------\n");
    // expression(&token, scope);



    return root;
}

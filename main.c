#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "globalDefinitions.h"

// #include "treeSeparator.h"

#include "parser/scanner/scanner_structs.h"
#include "parser/scanner/scanner.h"

#include "parser/treeGenerator/tree_structs.h"

#include "parser/scanner/createFence.h"

#include "parser/preprocessor/preprocessor.h"

#include "parser/treeGenerator/parser.h"

#include "parser/typeTree/treeSeparator.h"
#include "parser/typeTree/treeElements.h"

#include "parser/interpreter/interpreter.h"

//? ############################################################################
//? ############################################################################

int main() {

    // FILE* file = fopen("codeExample.code", "r");

    // fclose(file);

    printf("HI\n");

    

    // TypeTree* tree = treeSeparator_(str);

    // printf("-------------------------------------------------------------------------\n");

    // TypeTree* p = tree;
    // while (p) {
    //     printf("  |\n  |\n  V\n");
    //     typeTree_print_file(stdout, p);
    //     p = p->child;
    // }

    // typeTree_print_graphviz("graph001.gr", tree);


    // char *str = "def my_super_mega_very_important_attempt_to_create_function_func arg_name_1 \
    biefosahi interesting : 200 : Int->Int->Int->Int = (a+b * 241.124)*c+24";
    // char *str = "def func a b c : Int->Int->Int->Int = (12.4*a if a == 2 else 22.8*a + `* b 23) * c + pow 82 2 - 2 `pow 4";
    // char *str = "3 + 4 × 2 ÷ (1-5) ^ 2 ^ 3";
    // char *str = "def dsa";
    // char *str = "def 23 235.3258 dsa";
    // char *str = "def 23 (235.3258) dsa";

    // TypeTree *tree1 = typeTree_treeSeparator("Int");
    // // TypeTree *tree1 = typeTree_treeSeparator("Int->Float->Int->Char");
    // typeTree_print_graphviz("fence_dump", tree1);
    // system("dot -Tsvg fence_dump -o dump1.svg");
    // DOT
    // printf("---> |%s|", typeTree_convertToString(tree1));
    // DOT
    // CAP
    // // TypeTree *tree2 = typeTree_treeSeparator("Int->Array(Int)->Int->Int");
    // printf("h: %d\n", typeTree_getHeigh(tree1));
    // TypeTree *tree2 = typeTree_getArg(tree1, 3);

    // typeTree_print_graphviz("fence_dump", tree2);
    // system("dot -Tsvg fence_dump -o dump2.svg");

    // // printf("is %d\n", typeTree_isEqual(tree1, tree2));
    // CAP


    //? #######################################################################
    //? #######################################################################


    char* str = preprocessor("devel/codeExample006.txt");
    DOT
    Token *token = convertToTokens(str);
    // CAP
    Fence *fence = fence_createFenceFromTokens(token);
    fence_print(fence, "fence_dump");
    system("dot -Tsvg fence_dump -o fence.svg");

    Root* root = parser(fence);
    // Root* root = optimizer(root);

    root_printToFile(root, "rootDump.gr");
    system("dot -Tsvg rootDump.gr -o root.svg");

    DOT

    interpreter(root);

    CAP
    // printf("fence %p\n", fence);

    // FILE *dump = fopen("fence_dump", "w+");
    // fclose(dump); dump = NULL;



    // printGraphOfFencelListToFile(fence, "graphFence.gr");

    // Function *fn = function_declaration(token->next);

    // function_printToFile(fn, stdout);

    // DefList *deflist = fenceToDefList(fence);

    int w = 3;


    return 0;
}

//? ############################################################################
//? ############################################################################

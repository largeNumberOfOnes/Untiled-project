

#include "../scanner/tokenTypesEnum.h"
#include "../scanner/scanner_structs.h"

#include "tree_structs.h"


#include "../../globalDefinitions.h"
#include <assert.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "treeTypesEnum.h"

#include "expressionAnalizer.h"

#include "../typeTree/treeSeparator.h"

//? ###########################################################################
//? ###########################################################################

size_t getContent(Token *token, Scope *scope) {

    switch (token->type) {
        case TOKEN_TYPE_INT:
        case TOKEN_TYPE_FLOAT:
        case TOKEN_TYPE_CHAR:
        case TOKEN_TYPE_STRING:
            return (size_t) strdup(token->string);
        case TOKEN_TYPE_FUNCTION:
        case TOKEN_TYPE_OPERATOR:
        case TOKEN_TYPE_ACUTED_OPERATOR:
        case TOKEN_TYPE_ACUTED_FUNCTION:
            return (size_t) scope_getFunction(scope, token->string); // dev
            // char *type = scope_getType(token->string);
            // if (!type) CAP // dev
            // return typeTree_treeSeparator(type);
            // CAP
        case TOKEN_TYPE_VARIABLE:
            return (size_t) strdup(token->string);
        default:
            CAP
    }

}

TypeTree* tokenTypeToTypeTree(Token *token, Scope *scope) {

    switch (token->type) {
        case TOKEN_TYPE_INT:
            return typeTree_treeSeparator("Int");
        case TOKEN_TYPE_FLOAT:
            return typeTree_treeSeparator("Float");
        case TOKEN_TYPE_CHAR:
            return typeTree_treeSeparator("Char");
        case TOKEN_TYPE_STRING:
            return typeTree_treeSeparator("Array(Char)");
        case TOKEN_TYPE_FUNCTION:
        case TOKEN_TYPE_OPERATOR:
        case TOKEN_TYPE_ACUTED_OPERATOR:
        case TOKEN_TYPE_ACUTED_FUNCTION:
            return scope_getFunction(scope, token->string)->typeTree; // dev
        case TOKEN_TYPE_VARIABLE:
            DOT
            TypeTree* tree = scope_getType(scope, token->string);
            DOT
            return tree;
            // dev
            // CAP
            // char *type = scope_getType(token->string);
            // if (!type) CAP // dev
            // return typeTree_treeSeparator(type);
            // CAP
        // case TOKEN_TYPE_VARIABLE:
        default:
            CAP
    }

}

//? ###########################################################################
//? ###########################################################################

Pair* pair_init(Exptree *func, Exptree *arg) {

    Pair *pair = (Pair*) malloc(sizeof(Pair));

    pair->func = func;
    pair->arg = arg;

    return pair;
}

Exptree* exptree_initIf(Exptree *cond, Exptree *arg1, Exptree *arg2) {

    Exptree *tree = (Exptree*) malloc(sizeof(Exptree));

    tree->type = EXPTREE_TYPE_IF;
    tree->content = (size_t) malloc(sizeof(Ifexptree));

    assert(typeTree_isEqual(cond->elemType, typeTree_treeSeparator("Int")) == TRUE);
    ((Ifexptree*)(tree->content))->cond = cond;
    assert(typeTree_isEqual(arg1->elemType, arg2->elemType) == TRUE);
    assert(typeTree_getHeigh(arg1->elemType) == 1);
    ((Ifexptree*)(tree->content))->arg1 = arg1;
    ((Ifexptree*)(tree->content))->arg2 = arg2;


    TypeTree *to   = typeTree_copy(arg1->elemType);
    TypeTree *from = typeTree_treeSeparator("Int");
    tree->elemType = to;
    typeTree_putToBottom(to, from); // dev

    return tree;
}

Exptree* exptree_initApply(Exptree *func, Exptree *arg) {

    Exptree *tree = (Exptree*) malloc(sizeof(Exptree));

    tree->type = EXPTREE_TYPE_APPLY;

    TypeTree* last = typeTree_getLast(func->elemType);
    if (!typeTree_isEqual(last, arg->elemType)) {
        CAP // dev // cannot apply
    }

    tree->elemType = typeTree_copy(func->elemType);
    typeTree_deleteLast(tree->elemType);
    
    tree->content = (size_t) pair_init(func, arg);
    // tree->elemType = typetree;

    return tree;
}

Exptree* exptree_initSimple(Token *token, Scope *scope) {

    if (!token) return NULL;

    Exptree *tree = (Exptree*) malloc(sizeof(Exptree));

    if ((token->type == TOKEN_TYPE_INT) ||
        (token->type == TOKEN_TYPE_FLOAT) ||
        (token->type == TOKEN_TYPE_CHAR) ||
        (token->type == TOKEN_TYPE_STRING)
    ) {
        tree->type = EXPTREE_TYPE_ELEM;
    } else if ((token->type == TOKEN_TYPE_FUNCTION) ||
               (token->type == TOKEN_TYPE_ACUTED_OPERATOR)
    ) {
        tree->type = EXPTREE_TYPE_FUNCTION;
    } else if ((token->type == TOKEN_TYPE_OPERATOR) ||
               (token->type == TOKEN_TYPE_ACUTED_FUNCTION)
    ) {
        tree->type = EXPTREE_TYPE_OPERATOR;
    } else if (token->type == TOKEN_TYPE_VARIABLE) {
        tree->type = EXPTREE_TYPE_ELEM; // chan
    } else {
        printf("--> |%d|\n", token->type);
        CAP // dev
    }
    tree->elemType = tokenTypeToTypeTree(token, scope);
    if (!(tree->elemType)) return NULL;
    tree->content = getContent(token, scope);

    return tree;
}

int exptree_getPriority(Exptree *tree) {
    
    assert(tree != NULL);
    
    if (tree->type == EXPTREE_TYPE_APPLY) {
        return exptree_getPriority(((Pair*)(tree->content))->func);
    } else if (tree->type == EXPTREE_TYPE_FUNCTION ||
               tree->type == EXPTREE_TYPE_OPERATOR
    ) {
        return ((Function*)(tree->content))->priority;
    } else if (tree->type == EXPTREE_TYPE_ELEM) {
        CAP // dev
    } else {
        CAP // dev
    }

}

// Exptree* exptree_initFunction(Token *token, Scope *scope) {

//     assert(token == NULL);
//     assert(scope == NULL);
    
//     Exptree *tree = NULL;

//     if (token->type == TOKEN_TYPE_INT) {

//     }

//     if (token->type == TOKEN_TYPE_FUNCTION) {
//         //
//     } 

//     if (!tree) tree = exptree_simpleTypeDet(token);

//     if (tree) tree->str = strdup(token->string);
    

// }

// Exptree* exptree_simpleTypeDet(Token *token) {

//     // switch (token->type) {
//     //     case TOKEN_TYPE_INT:      return exptree_init(TREE_TYPE_INT);
//     //     case TOKEN_TYPE_FLOAT:    return exptree_init(TREE_TYPE_FLOAT);
//     //     case TOKEN_TYPE_CHAR:     return exptree_init(TREE_TYPE_CHAR);
//     //     case TOKEN_TYPE_VARIABLE: return exptree_init(TREE_TYPE_VARIABLE);
//     //     case TOKEN_TYPE_STRING:   return exptree_init(TREE_TYPE_STRING);
//     //     default:                  return NULL;
//     // }

//     return NULL;
// }



// Exptree* exptree_apply(Exptree *tree, Exptree *arg) {

//     assert(arg != NULL);
//     arg->
//     if (!tree) return arg;

//     Exptree* new = exptree_initApply();
    

//     return NULL;
// }

//? ###########################################################################
//? ###########################################################################

//? ###########################################################################
//? ###########################################################################

int reduceOptack(Opstack *stack, Opstack *output) {

    while (!opstack_isEmpty(stack)) {
        MES("-----------------------------")
        Exptree* tree = opstack_pop(stack);
        opstack_push(output, tree);
    }

    return 0;
}

//? ###########################################################################
//? ###########################################################################

int isFuncArg(Token *token) {
    
    switch (token->type) {
        case TOKEN_TYPE_INT:
        case TOKEN_TYPE_FLOAT:
        case TOKEN_TYPE_CHAR:
        case TOKEN_TYPE_STRING:
        case TOKEN_TYPE_FUNCTION:
        case TOKEN_TYPE_ACUTED_OPERATOR:
        case TOKEN_TYPE_VARIABLE:
        case TOKEN_TYPE_PARENTHESES_BRACKET_OPEN:

            return TRUE;
        default:
            return FALSE;
    }
}

Exptree* eatFunction(Token **token_ptr, Scope *scope) {

    Token *token = *token_ptr;

    Exptree *func = exptree_initSimple(token, scope);
    assert(func != NULL);
    assert(func->type == EXPTREE_TYPE_FUNCTION);
    token = token->next;

    int argCount = ((Function*)(func->content))->argsCount;
    for (int q = 0; q < argCount; ++q) {

        DOT
        if (isFuncArg(token)) {
            Exptree *tree = NULL;
            if (token->type == TOKEN_TYPE_PARENTHESES_BRACKET_OPEN) {
                token = token->next; 
                tree = expression(&token, scope);
                token = token->prev;
                // printf("--> %s\n", token->string);
                // exptree_printToFile(tree, "PPPPPPPPPPPPPPP.txt");
                // system("dot -Tsvg PPPPPPPPPPPPPPP.txt -o pppppppp.svg");
                // CAP
            } else {
                tree = exptree_initSimple(token, scope);
            }
            assert(tree != NULL);
            // printf("--> %s\n", typeTree_convertToString(func->elemType));
            // printf("--> %s\n", typeTree_convertToString(tree->elemType));
            // CAP
            DOT
            if (typeTree_isEqual(tree->elemType, typeTree_getLast(func->elemType))) {
                DOT
                func = exptree_initApply(func, tree);
                DOT
                token = token->next;
            } else {
                DOT
                break;
            }            
        } else if (token->type == TOKEN_TYPE_PARENTHESES_BRACKET_CLOSE) {
            break;
        } else {
            DOT
            break;
        }

    }

    // if (token) *token_ptr = token;
    *token_ptr = token;
    DOT

    return func;
}

//? ###########################################################################
//? ###########################################################################

int isConstType(Token *token) {

    switch (token->type) {
        case TOKEN_TYPE_INT:
        case TOKEN_TYPE_FLOAT:
        case TOKEN_TYPE_CHAR:
        case TOKEN_TYPE_STRING:
            return TRUE;
        default:
            return FALSE;
    }
}

int isVarType(Token *token) {

    switch (token->type) {
        case TOKEN_TYPE_VARIABLE:
            return TRUE;
        default:
            return FALSE;
    }
}

int isOperType(Token *token) {

    switch (token->type) {
        case TOKEN_TYPE_OPERATOR:
        case TOKEN_TYPE_ACUTED_FUNCTION:
            return TRUE;
        default:
            return FALSE;
    }
}

int isFuncType(Token *token) {

    switch (token->type) {
        case TOKEN_TYPE_FUNCTION:
        case TOKEN_TYPE_ACUTED_OPERATOR:
            return TRUE;
        default:
            return FALSE;
    }
}

Opstack* reorder(Token **token_ptr, Scope *scope) {

    Token *token = *token_ptr;

    Opstack *stack  = opstack_init();
    Opstack *output = opstack_init();

    DOT

    while (token) {

        if (isOperType(token)) {
            // scope_isInScope(token->string);
            printf("--> %s\n", token->string);
            DOT
            Function* fn = scope_getFunction(scope, token->string);
                // FILE *file = fopen("HHHHHHHHHHHHHHHHHHHH.txt", "w+"); // deb
                // scope_dump(scope, file); // deb
                // fclose(file); // deb
                // printf("--> %s\n", fn->name); // deb

            DOT
            assert(fn != NULL); // dev
            // function_isOper(fn);
            assert(function_isOper(fn) != 0); // dev
            Exptree *fnTree = exptree_initSimple(token, scope);
            assert(fnTree != NULL);
            DOT
            if (opstack_getPriority(stack) < fn->priority) {
                DOT
                opstack_push(stack, fnTree);
            } else {
                DOT
                reduceOptack(stack, output);
                opstack_push(stack, fnTree);
            }
            DOT
        } else if (token->type == TOKEN_TYPE_PARENTHESES_BRACKET_OPEN) {
            DOT
            // CAP
            Exptree *tree = expression(&(token->next), scope);
            assert(tree != NULL);
            // exptree_printToFile(tree, "PPPPPPPPPPPPPPP.txt");
            // CAP
            if (typeTree_getHeigh(tree->elemType) > 1) {
                opstack_push(stack, tree);
            } else {
                // MES("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^out");
                // opstack_dump(output, stdout);
                // MES("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^sta");
                // opstack_dump(stack, stdout);
                // MES("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
                opstack_push(output, tree);
            }
            // opstack_dump(stack, stdout);
            // MES("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
            // opstack_dump(output, stdout);
            // MES("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
            
        } else if (token->type == TOKEN_TYPE_PARENTHESES_BRACKET_CLOSE) {
            break;
        } else if (isConstType(token) || isFuncType(token) || isVarType(token)){
            DOT
            opstack_push(output, exptree_initSimple(token, scope));
        } else {
            printf("--1> |%s|\n", token->string);
            CAP // dev
        }

        token = token->next;
    }

DOT
    // opstack_dump(stack, stdout);
    // opstack_dump(output, stdout);
    reduceOptack(stack, output);
    opstack_dump(output, stdout);
    DOT

    if (token) *token_ptr = token->next;

    return output;
}

//? ###########################################################################
//? ###########################################################################

Exptree* opstackToExptree(Opstack *stack) {

    assert(stack != NULL); // dev
    assert(opstack_isEmpty(stack) != TRUE); // dev

    Exptree *tree = opstack_pop(stack);

    if (tree->type == EXPTREE_TYPE_ELEM || 
        tree->type == EXPTREE_TYPE_APPLY
    ) {
        return tree;
    } else if (tree->type == EXPTREE_TYPE_OPERATOR) {

        Exptree *arg1 = opstackToExptree(stack);
        assert(arg1 != NULL);
        Exptree *arg2 = opstackToExptree(stack);
        assert(arg2 != NULL);

        tree = exptree_initApply(tree, arg2);
        assert(tree != NULL);
        tree = exptree_initApply(tree, arg1);
        assert(tree != NULL);

        return tree;
    }

    return NULL;
}

Exptree* weakexpression(Token **token_ptr, Scope *scope) {

    Opstack *output = reorder(token_ptr, scope);
    Exptree *tree = opstackToExptree(output);
    if (!opstack_isEmpty(output)) { CAP } // dev // It`is an error obviously

    return tree;
}

//? ###########################################################################
//? ###########################################################################

Exptree* expression(Token **token_ptr, Scope *scope) {

    DOT

    Token *token = *token_ptr;

    Exptree *tree = NULL;
    if (token->type == TOKEN_TYPE_FUNCTION ||
        token->type == TOKEN_TYPE_ACUTED_OPERATOR
    ) {
        tree = eatFunction(token_ptr, scope);
        DOT
        Token *token = *token_ptr;
        // printf("--> %s\n", token->string);
        // CAP
        DOT
        if (token && isOperType(token)) {
            *token_ptr = token->next;
            Exptree *arg2 = expression(token_ptr, scope);
            Exptree *func = exptree_initSimple(token, scope);
            assert(func != NULL);
            func = exptree_initApply(func, tree);
            func = exptree_initApply(func, arg2);
            tree = func;
        }
        DOT
    } else if (!strcmp(token->string, "if")) {
        // *token_ptr = (*token_ptr)->next;
        // Exptree *cond = expression(token_ptr, scope);
        // *token_ptr = (*token_ptr)->next;
        // // exptree_printToFile(cond, "PPPPPPPPPPPPPPP.txt");
        // // system("dot -Tsvg PPPPPPPPPPPPPPP.txt -o pppppppp.svg");
        // printf("--> %s\n", (*token_ptr)->string);
        // Exptree *arg1 = expression(token_ptr, scope);
        CAP
    } else {
        tree = weakexpression(token_ptr, scope);

    }

    // if (token->type)


    assert(tree != NULL);

    DOT
    exptree_printToFile(tree, "PPPPPPPPPPPPPPP.txt");
    system("dot -Tsvg PPPPPPPPPPPPPPP.txt -o pppppppp.svg");



    return tree;
}

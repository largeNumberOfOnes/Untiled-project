

#include <assert.h>

#include "createFence.h"

#include "scanner_structs.h"


#include "../../globalDefinitions.h"

Fence* fence_createFenceFromTokens(Token *token) {

    assert(token != NULL);

    Fence* head     = NULL;
    Fence* curFence = head;
    int isHeadEmpty = 0;

    while (token) {
        // DOT
        if (isSovereign(token)) {
            DOT
            if (!isHeadEmpty) {
                head = fence_init(token);
                curFence = head;
                isHeadEmpty = 1;
            } else {
                assert(token->prev);
                token->prev->next = NULL;
                token->prev = NULL;
                fence_append(curFence, token);
                curFence = curFence->next;
            }
        }

        token = token->next;
    }




    return head;
}
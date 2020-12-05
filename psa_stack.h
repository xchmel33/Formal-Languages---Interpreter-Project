//
// Created by Matúš on 26. 11. 2020.
//

#ifndef IFJSTACK_PSA_STACK_H
#define IFJSTACK_PSA_STACK_H
#include "scanner.h"

typedef struct
{
    Token E;
    struct Stack_item *lptr;
    struct Stack_item *rptr;
}Stack_item;



typedef struct
{
    Stack_item *top;
    Stack_item *active;

} Psa_stack;

Psa_stack *s_init();
void s_destroy(Psa_stack *stack);
Token* s_pop (Psa_stack *stack);
int s_push (Psa_stack *stack,Token *token);
void s_print(Psa_stack* stack,char* mame);
//Token* s_search_tokens(Psa_stack* stack, TokenType rangeBegin, TokenType rangeEnd);



#endif //IFJSTACK_PSA_STACK_H

/**
 * Implementation of imperative language IFJ2020 compiler
 * @file psa_stack.h
 * @author Lukáš Chmelo (xchmel33), Matúš Nosko (xnosko06)
 * @brief Stack for precedence analysis
 */

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
void s_print(Psa_stack* stack, char* name);
//Token* s_search_tokens(Psa_stack* stack, TokenType rangeBegin, TokenType rangeEnd);



#endif //IFJSTACK_PSA_STACK_H

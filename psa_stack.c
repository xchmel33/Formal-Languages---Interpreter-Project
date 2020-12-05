#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "psa_stack.h"
#include "scanner.h"



Psa_stack *s_init()
{
    Psa_stack *new_stack = malloc(sizeof(Psa_stack));

    if (new_stack == NULL)
    {
        return NULL;
    }

    new_stack->active = NULL;
    new_stack->top = NULL;

    return new_stack;
}

void s_destroy(Psa_stack* stack)
{
    Stack_item* X = stack->top;
    while (X != NULL) {
        X = stack->top->lptr;
        free(stack->top);
        stack->top = X;
    }
    X = stack->active;
    while (X != NULL) {
        X = stack->active->lptr;
        free(stack->active);
        stack->active = X;
    }
}

int s_push(Psa_stack *stack, Token *token)
{
    Stack_item *new_i = malloc(sizeof(Stack_item));
    if (new_i == NULL)
    {
        return ERR_PARSER;
    }

    new_i->E = *token;

    new_i->lptr = (struct Stack_item *) stack->top;
    new_i->rptr = NULL;

    if (stack->top != NULL)
    {
        stack->top->rptr = (struct Stack_item *) new_i;
    }
    stack->top = new_i;
    return ERR_OK;
}

Token* s_pop(Psa_stack *stack)
{
    if (stack->top != NULL)
    {
        Stack_item *tmp = stack->top;
        stack->top = (Stack_item *) tmp->lptr;
        return &tmp->E;
    }
}

void s_print(Psa_stack *stack, char* name)
{
    printf("START OF %s STACK:\n", name);
    stack->active = stack->top;
    while (stack->active != NULL) {
        
        printToken(&stack->active->E);
        stack->active = stack->active->lptr;
    }
    printf("END OF %s STACK\n", name);
}

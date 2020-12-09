/**
 * Implementation of imperative language IFJ2020 compiler
 * @file psa_stack.c
 * @author Lukáš Chmelo (xchmel33), Matúš Nosko (xnosko06)
 * @brief Stack for precedence analysis
 */
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "psa_stack.h"
#include "scanner.h"



Psa_stack* s_init()                                 //stack initialization
{
    Psa_stack* new_stack = malloc(sizeof(Psa_stack));           //allocate memory for stack

    if (new_stack == NULL)
    {
        return NULL;
    }

    new_stack->active = NULL;
    new_stack->top = NULL;

    return new_stack;
}

void s_destroy(Psa_stack* stack)            //free allocated memory for stack
{
    free(stack);
}

int s_push(Psa_stack* stack, Token *token)      //add new element on top of stack
{
    Stack_item* new_i = malloc(sizeof(Stack_item));
    if (new_i == NULL)
    {
        return ERR_PARSER;
    }
    new_i->E = *initToken();
    new_i->E.type = token->type;
    if (token->attribute.integer != UNDEFINED_TOKEN_ATTRIBUTE) {
        new_i->E.attribute.integer = token->attribute.integer;
    }
    else if (token->attribute.decimal != UNDEFINED_TOKEN_ATTRIBUTE) {
        new_i->E.attribute.decimal = token->attribute.decimal;
    }
    else if (token->attribute.string->length != 0) {
        strCopyString(new_i->E.attribute.string, token->attribute.string);
    }

    new_i->lptr = (struct Stack_item*) stack->top;
    new_i->rptr = NULL;

    if (stack->top != NULL)
    {
        stack->top->rptr = (struct Stack_item*) new_i;
    }
    stack->top = new_i;
    return ERR_OK;
}

Token* s_pop(Psa_stack* stack)              //remove element on top of stack
{
    if (stack->top != NULL)
    {
        Stack_item* tmp = stack->top;
        stack->top = (Stack_item*)tmp->lptr;
        return &tmp->E;
    }
    return NULL;
}

void s_print(Psa_stack* stack, char* name)          //print stack
{
    printf("START OF %s STACK:\n", name);
    stack->active = stack->top;
    while (stack->active != NULL) {

        printToken(&stack->active->E);
        stack->active = stack->active->lptr;
    }
    printf("END OF %s STACK\n\n", name);
}

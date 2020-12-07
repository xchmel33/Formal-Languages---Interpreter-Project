#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"
#include "parser.h"
#include "psa_stack.h"
#include "scanner.h"
#include "dstring.h"

#define T_S (7)
#define Error (-1)

Psa_stack* ActiveStack;
Token* EMPTY_TOKEN;

const char prec_table[T_S][T_S] = {
    //      *			  +-   */  <!=   (	   )     i	  $ *//
    /*  	+-	    */ { '>', '<', '>', '<',  '>',  '<', '>'},
    /*  	*&/	    */ { '>', '>', '>', '<',  '>',  '<', '>'},
    /*    <...!=    */ { '<', '<', '>', '<',  '>',  '<', '>'},
    /*	    (	    */ { '<', '<', '<', '<',  '=',  '<', ' '},
    /*	    )	    */ { '>', '>', '>', ' ',  '>',  ' ', '>'},
    /*	    i	    */ { '>', '>', '>', ' ',  '>',  ' ', '>'},
    /*	    $	    */ { '<', '<', '<', '<',  ' ',  '<', ' '},
};
void ActivateResources() {
    ActiveStack = s_init();
    EMPTY_TOKEN = initToken();
    strAddChar(EMPTY_TOKEN->attribute.string, '$');
    EMPTY_TOKEN->type = TT_EMPTY;
}

void FreeResources() {
    s_destroy(ActiveStack);
}

int getIndex(Token* T) {

    //pozicia v tabulke
    switch (T->type) {
    case TT_ADD:
    case TT_SUB:
        return 0;
    case TT_MUL:
    case TT_DIV:
        return 1;
    case TT_LESS_THAN:
    case TT_MORE_THAN:
    case TT_EQUAL:
    case TT_MORE_OR_EQUAL:
    case TT_LESS_OR_EQUAL:
    case TT_NOT_EQUAL:
        return 2;
    case(TT_L_BRACKET):
        return 3;
    case(TT_R_BRACKET):
        return 4;
    case TT_IDENTIFIER:
    case TT_INTEGER:
    case TT_DECIMAL:
    case TT_STRING:
        return 5;
    case TT_EMPTY:
    case TT_SEMICOLON:
    case TT_COMMA:
    case TT_EOL:
        return 6;
    default:
        return -1;
    }
}

Token* getSymbol(Token* A, Token* B) {

    int x, y;
    x = getIndex(A);
    y = getIndex(B);
    if ((x == -1) || (y == -1)) {
        return NULL;
    }
    Token* sym = initToken();
    sym->type = TT_TABLESYM;
    strAddChar(sym->attribute.string, prec_table[x][y]);
    return sym;
}

Token* getNextToken(Token* actual_token, Token* next_token) {

    if (actual_token->type == next_token->type) {
        GetToken(next_token);
        return next_token;
    }
    else {
        return next_token;
    }
}

Token* topTerm(Psa_stack* stack) {

    stack->active = (struct Stack_item*) stack->top;
    while (stack->active != NULL) {
        if (stack->active->E.type >= TT_ADD && stack->active->E.type <= TT_EMPTY) {
            return &stack->active->E;
        }
        stack->active = stack->active->lptr;
    }
}

Token* checkRule(Psa_stack* Rulestack, HashTable *table) { //codegen required
    
    Token* operand1 = s_pop(Rulestack);
    Token* operator,* operand2;

    if (operand1->type == TT_EXPRESSION) {
        operator = s_pop(Rulestack);
        operand2 = s_pop(Rulestack);
        switch (operator->type)
        {
        // E -> E+E
        case(TT_ADD):
            break;
        // E -> E-E
        case(TT_SUB):
            break;
        // E -> E*E
        case(TT_MUL):
            break;
        // E -> E/E
        case(TT_DIV):
            break;
        // E -> E<E
        case(TT_LESS_THAN):
            break;
        // E -> E>E
        case(TT_MORE_THAN):
            break;
        // E -> E==E
        case(TT_EQUAL):
            break;
        // E -> E>=E
        case(TT_LESS_OR_EQUAL):
            break;
        // E -> E<=E
        case(TT_MORE_OR_EQUAL):
            break;
        // E -> E!=E
        case(TT_NOT_EQUAL):
            break;
        }
    }
    // E -> i
    else if (operand1->type == TT_IDENTIFIER){ 
        //WIP -> hash table
        operand1->type = TT_EXPRESSION;
        return operand1;
    }
    else if (operand1->type >= TT_INTEGER && operand1->type <= TT_STRING) {
        operand1->type = TT_EXPRESSION;
        return operand1;
    }

    // E -> (E)
    else if (operand1->type == TT_L_BRACKET) {
        operand1 = s_pop(Rulestack);
        if (s_pop(Rulestack)->type == TT_R_BRACKET && operand1->type == TT_EXPRESSION) {
            return operand1;
        }
    }

    //unknown rule
    return EMPTY_TOKEN;
}


int expression(HashTable *table, Token* prev_token, Token* act_token) {

    //moze ist do main
    ActivateResources();

    Token* A, * B, * Y, *S;
    Psa_stack* RuleStack;
    B = prev_token;

    if (s_push(ActiveStack, EMPTY_TOKEN) != ERR_OK) {
        return Error;
    }

    do {
        A = topTerm(ActiveStack);
        S = getSymbol(A, B);
        if (B->type != TT_EMPTY || A->type != TT_EMPTY) {
            switch (S->attribute.string->str[0])
            {
            case '=':
                s_push(ActiveStack, B);
                B = getNextToken(B, act_token);
                break;
            case '<':
                if (ActiveStack->top->E.type == TT_EXPRESSION) {
                    prev_token = s_pop(ActiveStack);
                    s_push(ActiveStack, S);
                    s_push(ActiveStack, prev_token);
                    s_push(ActiveStack, B);
                }
                else {
                    s_push(ActiveStack, S);
                    s_push(ActiveStack, B);
                }
                B = getNextToken(B, act_token);
                break;
            case '>':
                RuleStack = s_init();
                Y = s_pop(ActiveStack);
                while (Y->type != TT_TABLESYM) {
                    s_push(RuleStack, Y);
                    Y = s_pop(ActiveStack);
                }
                //s_push(RuleStack, Y);
                s_print(RuleStack, "RuleStack");
                s_push(ActiveStack, checkRule(RuleStack,table));
                if (ActiveStack->top->E.type == TT_EMPTY) {
                    printf("Unknown rule -> rules are WIP\n");
                    return Error;
                }
                s_destroy(RuleStack);
                break;
            case ' ':
                printf("Error");
                return Error;
                break;
            }
        }
        s_print(ActiveStack, "ActiveStack");
        if (B->type == TT_EOL || B->type == TT_EOF) {
            B = EMPTY_TOKEN;
        }
    } while (B->type != TT_EMPTY || A->type != TT_EMPTY);
    printf("OK\n");
    FreeResources;
}

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"
#include "parser.h"
#include "symtable.h"
#include "psa_stack.h"
#include "scanner.h"

#define T_S (7)
#define Error (-1)

Psa_stack ActiveStack;
Token EMPTY_TOKEN;

char prec_table[T_S][T_S] = {
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
    ActiveStack = *s_init();
    EMPTY_TOKEN.attribute.string[0] = '$';
    EMPTY_TOKEN.type = TT_EMPTY;
}

void FreeResources() {
    s_destroy(&ActiveStack);
}

int getIndex(Token T) {

    //pozicia v tabulke
    switch (T.type) {
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

Token* getSymbol(Token A, Token B) {

    int x, y;
    x = getIndex(A);
    y = getIndex(B);
    if ((x == -1) || (y == -1)) {
        return NULL;
    }
    Token* sym = malloc(sizeof(Token));
    sym->type = TT_TABLESYM;
    sym->attribute.string[0] = prec_table[x][y];
    return sym;
}

void getNextToken(Token* token, Token prev_token, Token actual_token) {

    if (token->type == prev_token.type) {
        *token = actual_token;
    }
    else {
        GetToken(token);
    }
}

Token topTerm(Psa_stack* stack) {

    stack->active = (struct Stack_item*) stack->top;
    while (stack->active != NULL){
        if (stack->active->E.type >= TT_ADD && stack->active->E.type <= TT_EMPTY) {
            return stack->active->E;
        }
        stack->active = stack->active->lptr;
    }
}

int checkRule(Psa_stack* Rulestack) {
    
    // E -> E+E 
    // E -> E-E
    // E -> E*E
    // E -> E/E
    // E -> E<E
    // E -> E>E
    // E -> E==E
    // E -> E>=E
    // E -> E<=E
    // E -> E!=E
    // E -> (E)
    // E -> i
    // E -> integer
    // E -> double
    // E -> string

    return 0;
}


int expression(HashTable table, Token prev_token, Token act_token) {

    //moze ist do main
    ActivateResources();

    Token A, B, * S, Y;
    Psa_stack RuleStack;

    if (s_push(&ActiveStack, EMPTY_TOKEN) != ERR_OK) {
        return Error;
    }
    B = prev_token;
    do {
        A = topTerm(&ActiveStack);
        S = (Token*)getSymbol(A, B);
        if (S == NULL) {
            printf("error");
            return Error;
        }
        switch (S->attribute.string[0])
        {
        case '=':
            s_push(&ActiveStack, B);
            getNextToken(&B, prev_token, act_token);
            break;
        case '<':
            s_push(&ActiveStack, *S);
            s_push(&ActiveStack, B);
            getNextToken(&B, prev_token, act_token);
            break;
        case '>':
            RuleStack = *s_init();
            Y = s_pop(&ActiveStack);
            while (Y.type != TT_TABLESYM) {
                s_push(&RuleStack, Y);
                Y = s_pop(&ActiveStack);
            }
            s_push(&RuleStack, Y);
            printf("Rule stack:\n");
            s_print(&RuleStack);
            printf("END OF Rule stack:\n");
            s_destroy(&RuleStack);
            break;
        }
        printf("A:");
        printToken(&A);
        printf("B:");
        printToken(&B);
        printf("Active stack:\n");
        s_print(&ActiveStack);
        printf("End of Active stack:\n");
    } while (B.type != TT_EMPTY || A.type != TT_EMPTY);
}
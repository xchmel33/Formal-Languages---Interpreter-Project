
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"
#include "parser.h"
#include "symtable.h"
#include "psa_stack.h"

#define T_S (7)
#define Error (-1)

Psa_stack ActiveStack;

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
    x = getIndex(B);
    y = getIndex(A);
    if ((x == -1) && (y == -1)) {
        return NULL;
    }
    Token* sym = malloc(sizeof(Token));
    sym->type = TT_SYM;
    sym->attribute.string[0] = prec_table[x][y];
    return sym;
}

void getnexttoken(Token* token, Token prev_token, Token actual_token) {

    if (token == &prev_token) {
        token = &actual_token;
    }
    else {
        GetToken(token);
    }
}

int expresion(HashTable table, Token actual_token, Token prev_token) {

    //moze ist do main
    ActivateResources();

    Token A, B, $, *S;
    $.type = TT_EMPTY;
    strcpy($.attribute.string,"$");
    if (s_push(&ActiveStack, $) != ERR_OK) {
        return Error;
        B = prev_token;
        do {
            A = ActiveStack.top->E;
            S = (Token*)getSymbol(A, B);
            if (S->type != TT_SYM) {
                return Error;
                switch (S->attribute.string[0])
                {
                case '=':
                    s_push(&ActiveStack, B);
                    getnexttoken(&B, prev_token, actual_token);
                    break;
                case '<':
                    s_push(&ActiveStack, *S);
                    s_push(&ActiveStack, B);
                    getnexttoken(&B, prev_token, actual_token);
                    break;
                case '>':
                    if (ActiveStack.top->E.type >= TT_IDENTIFIER && ActiveStack.top->E.type <= TT_STRING) {
                        s_pop(&ActiveStack);
                        if (ActiveStack.top->E.type >= TT_SYM) {

                        }
                    }
                    break;
                }

            }


        } while (B.type != TT_EMPTY && A.type != TT_EMPTY);
    }
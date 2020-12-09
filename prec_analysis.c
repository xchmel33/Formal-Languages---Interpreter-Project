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
#include "code_generator.h"

#define T_S (7)
#define EXP_OK (0)

Psa_stack* ActiveStack;
Token* EMPTY_TOKEN;
HashTable* table;

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
void ActivateResources(HashTable* ptable) {
    ActiveStack = s_init();
    EMPTY_TOKEN = initToken();
    strAddChar(EMPTY_TOKEN->attribute.string, '$');
    EMPTY_TOKEN->type = TT_EMPTY;
    table = ptable;
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
Datatype getType(Token *token) {

    
    if (token->attribute.integer != UNDEFINED_TOKEN_ATTRIBUTE) {
        return INT;
    }
    else if (token->attribute.decimal = UNDEFINED_TOKEN_ATTRIBUTE) {
        return FLOAT64;
    }
    else if (htSearch(table, token->attribute.string->str) != NULL) {
        return IDENTIFIER;
    }
    else {
        return STRING;
    }
}

bool cg_stack_p(Token* token) {
    
    if (token->type == TT_STRING) {
        cg_stack_push_string(token->attribute.string->str);
    }
    else if (token->type == TT_INTEGER) {
        cg_stack_push_string(token->attribute.integer);
    }
    else if (token->type == TT_IDENTIFIER) {
        cg_stack_push_string(token->attribute.string->str);
    }
    else if (token->type == TT_DECIMAL) {
        cg_stack_push_double(token->attribute.decimal);
    }
    return true;
}

int checkTypes(Token* operand1,Token* operand2) {

    Datatype op1_type = getType(operand1);
    Datatype op2_type = getType(operand2);
    switch (op1_type)
    {
    case INT:
        if (op2_type == INT) {
            break;
        }
        else if (op2_type == STRING || op2_type == FLOAT64) {
            return ERR_MATH_TYPE;
        }
        else if (op2_type == IDENTIFIER) {
            if (htSearch(table, operand2->attribute.string->str)->data.type == T_INT) {
                break;
            }
            else {
                return ERR_MATH_TYPE;
            }
        }
    case FLOAT64:
        if (op2_type == STRING || op2_type == INT) {
            return ERR_MATH_TYPE;
        }
        else if (op2_type == FLOAT64) {
            break;
        }
        else if (op2_type == IDENTIFIER) {
            if (htSearch(table, operand2->attribute.string->str)->data.type == T_DOUBLE) {
                break;
            }
            else {
                return ERR_MATH_TYPE;
            }
        }
    case STRING:
        if (op2_type == INT || op2_type == FLOAT64) {
            return ERR_MATH_TYPE;
        }
        else if (op2_type == STRING) {
            break;
        }
        else if (op2_type == IDENTIFIER) {
            if (htSearch(table, operand2->attribute.string->str)->data.type == T_STRING) {
                break;
            }
            else {
                return ERR_MATH_TYPE;
            }
        }
    case IDENTIFIER:
        switch (htSearch(table, operand1->attribute.string->str)->data.type)
        {
        case T_INT:
            if (op2_type == INT) {
                break;
            }
            else if (op2_type == STRING || op2_type == FLOAT64) {
                return ERR_MATH_TYPE;
            }
            else if (op2_type == IDENTIFIER) {
                if (htSearch(table, operand2->attribute.string->str)->data.type == T_INT) {
                    break;
                }
                else {
                    return ERR_MATH_TYPE;
                }
            }
        case T_DOUBLE:
            if (op2_type == STRING || op2_type == INT) {
                return ERR_MATH_TYPE;
            }
            else if (op2_type == FLOAT64) {
                break;
            }
            else if (op2_type == IDENTIFIER) {
                if (htSearch(table, operand2->attribute.string->str)->data.type == T_DOUBLE) {
                    break;
                }
                else {
                    return ERR_MATH_TYPE;
                }
            }
        case T_STRING:
            if (op2_type == INT || op2_type == FLOAT64) {
                return ERR_MATH_TYPE;
            }
            else if (op2_type == STRING) {
                break;
            }
            else if (op2_type == IDENTIFIER) {
                if (htSearch(table, operand2->attribute.string->str)->data.type == T_STRING) {
                    break;
                }
                else {
                    return ERR_MATH_TYPE;
                }
            }
        }
    }
    return EXP_OK;
}

Token* checkRule(Psa_stack* Rulestack,int *error_code) { //codegen required
    
    Token* operand1 = s_pop(Rulestack);
    Token* operator,* operand2;
    TableItem* ID;

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
    if (operand1->type == TT_EXPRESSION) {
        operator = s_pop(Rulestack);
        operand2 = s_pop(Rulestack);

        //check matching types
        int pom = checkTypes(operand1, operand2);
        if (pom != EXP_OK) {
            *error_code = pom;
            return EMPTY_TOKEN;
        }

        //zero division
        if (operator->type == TT_DIV && operand2->attribute.integer == 0 || operand2->attribute.decimal == 0) {
            *error_code = ERR_DIV_ZERO;
            return EMPTY_TOKEN;
        }

        //operation
        //cg_stack_p(operand2);
        //cg_stack_p(operand1);
        //cg_operation(operator->type);
         
        //return token with operation datatype
        *error_code = EXP_OK;
        return operand1;
    }
    // E -> i
    else if (operand1->type == TT_IDENTIFIER){ 
        ID = htSearch(table, operand1->attribute.string->str);
        if (ID == NULL) {
            return ERR_DEF;
        }
        else {
            if (ID->data.var) {
                operand1->type = TT_EXPRESSION;
                *error_code = EXP_OK;
                return operand1;
            }
            else {
                //function call
            }
        }
        
    }
    else if (operand1->type >= TT_INTEGER && operand1->type <= TT_STRING) {
        operand1->type = TT_EXPRESSION;
        *error_code = EXP_OK;
        return operand1;
    }

    // E -> (E)
    else if (operand1->type == TT_L_BRACKET) {
        operand1 = s_pop(Rulestack);
        if (s_pop(Rulestack)->type == TT_R_BRACKET && operand1->type == TT_EXPRESSION) {
            *error_code = EXP_OK;
            return operand1;
        }
    }
    else {
        *error_code = ERR_INTERNAL;
        return EMPTY_TOKEN;
    }
}


int expression(Token* prev_token, Token* act_token) {

    Token* A, * B, * Y, *S;
    Psa_stack* RuleStack;
    B = prev_token;
    int pom;

    if (s_push(ActiveStack, EMPTY_TOKEN) != ERR_OK) {
        return ERR_INTERNAL;
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
                s_push(ActiveStack, checkRule(RuleStack, &pom));
                if (ActiveStack->top->E.type == TT_EMPTY) {
                    return pom;
                }
                s_destroy(RuleStack);
                break;
            case ' ':
                return ERR_PARSER;
            }
        }
        s_print(ActiveStack, "ActiveStack");
        if (B->type == TT_EOL || B->type == TT_EOF) {
            B = EMPTY_TOKEN;
        }
    } while (B->type != TT_EMPTY || A->type != TT_EMPTY);
    act_token = s_pop(ActiveStack);
    FreeResources();
    return pom;
}

/**
 * Implementation of imperative language IFJ2020 compiler
 * @file prec_analysis.c
 * @author Lukáš Chmelo (xchmel33)
 * @brief expression parser implementation
 */

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

#define T_S (7) //table size
#define EXP_OK (0) //return if ok

//global variables
Psa_stack* ActiveStack;
Token* EMPTY_TOKEN;
HashTable* table;

//prec table implemented as 2 dimensional array
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

    //table possition
    switch (T->type) {
        case TT_ADD:
        case TT_SUB: 
            return 0; //+-
        case TT_MUL:
        case TT_DIV:
            return 1; //*/
        case TT_LESS_THAN:
        case TT_MORE_THAN:
        case TT_EQUAL:
        case TT_MORE_OR_EQUAL:
        case TT_LESS_OR_EQUAL:
        case TT_NOT_EQUAL:
            return 2; // <...!=
        case(TT_L_BRACKET):
            return 3; //(
        case(TT_R_BRACKET):
            return 4; //)
        case TT_IDENTIFIER:
        case TT_INTEGER:
        case TT_DECIMAL:
        case TT_STRING:
            return 5; // i
        case TT_EMPTY:
        case TT_SEMICOLON:
        case TT_COMMA:
        case TT_EOL:
            return 6; //$
        default:
            return -1; //error
    }
}

Token* getSymbol(Token* A, Token* B) {

    //get prec table symbol 
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

Token* getNextToken(Token* actual_token, Token* next_token, int *error_code) {

    //decide whenever get the next token or use current
    if (actual_token->type == next_token->type) {
        *error_code = GetToken(next_token);
        return next_token;
    }
    else {
        return next_token;
    }
}

Token* topTerm(Psa_stack* stack) {

    //cycle while terminal nearest to stack top is found
    stack->active = (struct Stack_item*) stack->top;
    while (stack->active != NULL) {
        if (stack->active->E.type >= TT_ADD && stack->active->E.type <= TT_EMPTY) {
            return &stack->active->E;
        }
        stack->active = stack->active->lptr;
    }
    return NULL;
}
Datatype getType(Token *token) {

    /*token type will be lost by TT_EXPRESION, 
    so it has to be restored by initialized attribute*/
    if (token->attribute.integer != UNDEFINED_TOKEN_ATTRIBUTE) {
        return INT;
    }
    else if (token->attribute.decimal != UNDEFINED_TOKEN_ATTRIBUTE) {
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

    Datatype t = getType(token);
    if (t== STRING) {
        cg_stack_push_string(token->attribute.string->str);
        return true;
    }
    else if (t == INT) {
        cg_stack_push_int(token->attribute.integer);
        return true;
    }
    else if (t == IDENTIFIER) {
        cg_stack_push_identifier(token->attribute.string->str,table);
        return true;
    }
    else if (t == STRING) {
        cg_stack_push_double(token->attribute.decimal);
        return true;
    }
    return false;
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
                if (htSearch(table, operand2->attribute.string->str)->data.type == T_FLOAT64) {
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
                case T_FLOAT64:
                    if (op2_type == STRING || op2_type == INT) {
                        return ERR_MATH_TYPE;
                    }
                    else if (op2_type == FLOAT64) {
                        break;
                    }
                    else if (op2_type == IDENTIFIER) {
                        if (htSearch(table, operand2->attribute.string->str)->data.type == T_FLOAT64) {
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

Token* funcCall(TableItem *ID,int* error_code) {

    Token* token = initToken();
    Token* argument = initToken();
    GetToken(token); //left bracket or error
    if (token->type != TT_L_BRACKET) {
        *error_code = ERR_PARSER;
        return EMPTY_TOKEN;
    }
    GetToken(argument); //function argument type check
    if (ID->data.param[0].type != argument->type) {
        *error_code = ERR_FUNC;
        return EMPTY_TOKEN;
    }
    GetToken(token); //right bracket or error
    if (token->type != TT_R_BRACKET) {
        *error_code = ERR_PARSER;
        return EMPTY_TOKEN;
    }
    if (ID->data.return_type == T_NONE) {
        *error_code = ERR_MATH_TYPE;
        return EMPTY_TOKEN;
    }
    else if (ID->data.return_type == T_INT) {
        *error_code = EXP_OK;
        //token will be used to return integer value for further use
        token->type = TT_EXPRESSION;
        strClear(token->attribute.string);
        token->attribute.integer = 0;
        return token;
    }
    else if (ID->data.return_type == T_FLOAT64) {
        *error_code = EXP_OK;
        //token will be used to return float value for further use
        token->type = TT_EXPRESSION;
        strClear(token->attribute.string);
        token->attribute.decimal = 0;
        return token;
    }
    else if (ID->data.return_type == T_STRING) {
        *error_code = EXP_OK;
        //token will be used to return string value for further use
        token->type = TT_EXPRESSION;
        return token;
    }
    *error_code = ERR_FUNC;
    return EMPTY_TOKEN;
}

Token* checkRule(Psa_stack* Rulestack,int *error_code) { //codegen required

    Token *operand1 = s_pop(Rulestack);
    Token *operator = initToken();
    Token *operand2 = initToken();
    TableItem* ID; //for function or variable search

    
    // E -> E + ... != E --> all operation rules implementation
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

            //codegen operations
        //push to stack
        cg_stack_p(operand2);
        cg_stack_p(operand1);

        //string concatanation
        if (getType(operand1) == STRING && operator->type == TT_ADD && getType(operand2) == STRING) {
            cg_concat();
        }
        //all other operations
        cg_other_operations(operator->type);

        //return token with operation datatype
        *error_code = EXP_OK;
        return operand1;
    }

    // E -> i --> identifier
    else if (operand1->type == TT_IDENTIFIER){
        ID = htSearch(table, operand1->attribute.string->str);
        if (ID == NULL) {
            //variable or function not found
            *error_code = ERR_DEF;
            return EMPTY_TOKEN;
        }
        else {
            if (ID->data.var) {
                //change type to avoid being returned by getTerm function
                operand1->type = TT_EXPRESSION;
                *error_code = EXP_OK;
                return operand1;
            }
            else {
                //function call & return type stored inside token
                return funcCall(ID, error_code);
            }
        }

    }

    // E -> i --> integer, float, string
    else if (operand1->type >= TT_INTEGER && operand1->type <= TT_STRING) {
        operand1->type = TT_EXPRESSION;
        *error_code = EXP_OK;
        return operand1;
    }

    // E -> (E)
    else if (operand1->type == TT_L_BRACKET) {
        
        operand1 = s_pop(Rulestack);

        //return only if previus rule is applied 
        if (s_pop(Rulestack)->type == TT_R_BRACKET && operand1->type == TT_EXPRESSION) {
            *error_code = EXP_OK;
            return operand1;
        }
    }

    //no rule found
    *error_code = ERR_INTERNAL;
    return EMPTY_TOKEN;
}


int expression(Token* prev_token, Token* act_token) {

    Token* A, * B, * Y, * S; //complementary tokens
    Psa_stack* RuleStack; //temporary stack used to apply operation rules to tokens
    int error = 0; 

    //push $ to stack bottom
    if (s_push(ActiveStack, EMPTY_TOKEN) != ERR_OK) {
        return ERR_INTERNAL;
    }

    B = prev_token;
    do {
        A = topTerm(ActiveStack); //terminal
        S = getSymbol(A, B); //prec table symbol
        if (S == NULL) {
            return ERR_PARSER;
        }
        if (B->type != TT_EMPTY || A->type != TT_EMPTY) {
            switch (S->attribute.string->str[0])
            {
            case '=': //push only token
                s_push(ActiveStack, B);
                B = getNextToken(B, act_token, &error);
                break;
            case '<': //push token & symbol
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
                B = getNextToken(B, act_token, &error);
                break;
            case '>': //push tokens to rulestack
                RuleStack = s_init();
                Y = s_pop(ActiveStack);
                while (Y->type != TT_TABLESYM) {
                    s_push(RuleStack, Y);
                    Y = s_pop(ActiveStack);
                }
                //push result type stored in token to main stack 
                s_push(ActiveStack, checkRule(RuleStack, &error)); 
                if (ActiveStack->top->E.type == TT_EMPTY) {
                    return error;
                }
                s_destroy(RuleStack);
                break;
            case ' ':
                return ERR_PARSER;
            }
            if (error != 0) {
                return error;
            }
        }

        //end of expression
        if (B->type == TT_EOL || B->type == TT_EOF){
            B = EMPTY_TOKEN;
        }

        //end of expression when inside statement
        else if (B->type == TT_BLOCK_BEGIN) {
            s_push(ActiveStack, B);
            B = EMPTY_TOKEN;
        }
    } while (B->type != TT_EMPTY || A->type != TT_EMPTY);

    //store result type inside active token
    act_token = s_pop(ActiveStack); 
    
    FreeResources();
    return error; 
}
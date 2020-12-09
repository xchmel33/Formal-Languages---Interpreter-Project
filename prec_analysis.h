/**
 * Implementation of imperative language IFJ2020 compiler
 * @file prec_analysis.h
 * @author Lukáš Chmelo (xchmel33)
 * @brief expression parser header implementation
 */


#ifndef IFJ_FINAL_PREC_ANALYSIS_H
#define IFJ_FINAL_PREC_ANALYSIS_H

#include "scanner.h"
#include "psa_stack.h"
#include "symtable.h"

 /**
  * @brief Function for global variables initialization
  *
  * @param ptable, symtable to be initialized
  *
  */
void ActivateResources(HashTable* ptable);

/**
 * @brief Function for destroing stack
 */
void FreeResources();

/**
 * @brief Function for returning the index of terminal from prec table
 *
 * @param T, terminal
 *
 * @return integer, index of terminal
 */
int getIndex(Token* T);

/**
 * @brief Function for returning the symbol from prec table
 *
 * @param A, top terminal
 * @param B, current token
 *
 * @return token, symbol from prec table
 */
Token* getSymbol(Token* A, Token* B);

/**
 * @brief Function for returning the required token
 *
 * @param actual_token, current token
 * @param next token 
 *
 * @return token
 */
Token* getNextToken(Token* actual_token, Token* next_token, int* error_code);

/**
 * @brief Function for returning terminal nearest to stack top
 *
 * @param stack
 *
 * @return token, which stores the terminal
 */
Token* topTerm(Psa_stack* stack);

/**
 * @brief Function for comparing types of operands
 *
 * @param operand1
 * @param operand2
 *
 * @return 0, in case of error, number of error
 */
int checkTypes(Token* operand1, Token* operand2);

/**
 * @brief Function for evaluating type of result from operation as well as applying rules
 *
 * @param Rulestack, stores tokens for operations
 * @param result_type, type of result from operation
 *
 * @return 0, in case of error, number of error
 */
int checkRule(Psa_stack* Rulestack, Token *result_type);

/**
 * @brief Function for analyzing lost datatypes
 *
 * @param token, which is analyzed
 *
 * @return INT, FLOAT64, STRING or IDENTIFIER
 */
Datatype getType(Token* token);

/**
 * @brief Main function for processing expressions
 *
 * @param prev_token, input from parser
 * @param act_token, input from parser
 *
 * @return 0, in case of error, number of error
 */
int expression(Token* prev_token, Token* act_token);

/**
 * @brief Function for pushing operands to stack
 *
 * @param token, oprand which will be pushed to stack
 *
 * @return true, if type was not found false
 */
bool cg_stack_p(Token* token);

#endif //IFJ_FINAL_PARSER_H

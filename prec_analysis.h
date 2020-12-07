#ifndef IFJ_FINAL_PREC_ANALYSIS_H
#define IFJ_FINAL_PREC_ANALYSIS_H

#include "scanner.h"
#include "psa_stack.h"
#include "symtable.h"

void ActivateResources();
void FreeResources();
int getIndex(Token* T);
Token* getSymbol(Token* A, Token* B);
Token* getNextToken(Token* prev_token, Token* actual_token);
Token* topTerm(Psa_stack* stack);
Token* checkRule(Psa_stack* Rulestack, HashTable *table);
int expression(HashTable table, Token* prev_token, Token* act_token);

#endif //IFJ_FINAL_PARSER_H
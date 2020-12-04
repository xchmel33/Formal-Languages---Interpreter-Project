#ifndef IFJ_FINAL_PREC_ANALYSIS_H
#define IFJ_FINAL_PREC_ANALYSIS_H

#include "scanner.h"
#include "psa_stack.h"
#include "symtable.h"

void ActivateResources();
void FreeResources();
int getIndex(Token T);
Token* getSymbol(Token A, Token B);
void getNextToken(Token* token, Token prev_token, Token actual_token);
Token topTerm(Psa_stack* stack);
int checkRule(Psa_stack* Rulestack);
int expression(HashTable table, Token prev_token, Token act_token);

#endif //IFJ_FINAL_PARSER_H
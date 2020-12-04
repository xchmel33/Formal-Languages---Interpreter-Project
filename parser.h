//
// Created by Matúš on 30. 11. 2020.
//

#ifndef IFJ_FINAL_PARSER_H
#define IFJ_FINAL_PARSER_H

#include "scanner.h"
#include "symtable.h"

#define GET_TOKEN \
    GetToken(act_token)

void set_active_token(Token* token);
void set_active_table(HashTable* table);
int base_cond(Token* token);
int body();

int def_func();
int params(TableItem* func);
int statements();
int statement ();

int blockBeginEOL_check ();
int IfblockEnd_check();

int init ();
int value();
#endif //IFJ_FINAL_PARSER_H

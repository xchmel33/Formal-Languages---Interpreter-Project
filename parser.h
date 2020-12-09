//
// Created by Matúš on 30. 11. 2020.
//

#ifndef IFJ_FINAL_PARSER_H
#define IFJ_FINAL_PARSER_H

#include "scanner.h"
#include "symtable.h"

#define GET_TOKEN \
    error_code = GetToken(act_token); \
    if (error_code != 0) return error_code

void set_active_token(Token* token);
void set_active_table(HashTable* table);
int base_cond(Token* token);
int body();

int def_func();
int params(TableItem* func);
int statements();
int statement ();

int blockBeginEOL_check ();
int blockEnd_EOL_check ();
int IfblockEnd_check();

int init ();
int assign(Token* left_id);
int value();
int terms ();
#endif //IFJ_FINAL_PARSER_H

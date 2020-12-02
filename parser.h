//
// Created by Matúš on 30. 11. 2020.
//

#ifndef IFJ_FINAL_PARSER_H
#define IFJ_FINAL_PARSER_H

#include "scanner.h"
#include "symtable.h"

void set_active_token(Token* token);
void set_active_table(HashTable* table);
int base_cond(Token* token);
int body();

int def_func();
int params(TableItem* func);
int statements(TableItem* func);
int blockBeginEOL_check ();
#endif //IFJ_FINAL_PARSER_H

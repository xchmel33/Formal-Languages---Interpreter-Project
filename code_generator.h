//
// Created by Matúš on 6. 12. 2020.
//

#ifndef IFJ_FINAL_CODE_GENERATOR_H
#define IFJ_FINAL_CODE_GENERATOR_H

#include "dstring.h"
#include "parser.h"

#define ADD_CODE(_code) \
    if (!strAddString(&code,(_code))) return false

#define ADD_INSTR(_instr) \
    if (!strAddString(&code,(_instr "\n"))) return false

#define ADD_CODE_INT(_code) \
    do{ \
        char str[MAX_DIGITS]; \
        sprintf(str, "%d", _code);\
        ADD_CODE(str); \
    } while (0)

#define ADD_CODE_DOUBLE(_code) \
    do { \
        char str[MAX_DIGITS]; \
        sprintf(str,"%a",_code); \
        ADD_CODE(str); \
    } while (0)

#define MAX_DIGITS 40

bool cg_main();
bool cg_main_end();
bool cg_func_start(char* id);
bool cg_func_end(char* id);
bool cg_func_call(char* id);
bool cg_stack_push_int(int value);
bool cg_stack_push_double(double value);
bool cg_stack_push_string(char* value);
bool cg_stack_push_global(char* key);
bool cg_stack_push_identifier(char* key, HashTable globaltable);
bool cg_operation(TokenType operation);
bool cg_var_declare(char* id);
bool cg_def_val_var(DataType value);
bool cg_var_to_default_val(char* id, DataType value);
bool cg_var_val(Token token);
bool cg_var_to_any_val(char* id, Token token);
bool cg_print_id(TableItem* data);
bool cg_print_value(char* val, DataType type);
int code_to_stdout();

#endif //IFJ_FINAL_CODE_GENERATOR_H

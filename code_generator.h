//
// Created by Matúš on 6. 12. 2020.
//

#ifndef IFJ_FINAL_CODE_GENERATOR_H
#define IFJ_FINAL_CODE_GENERATOR_H

#include "dstring.h"

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

#define MAX_DIGITS 40

bool cg_main();
bool cg_main_end();
bool cg_func_start(char* id);
bool cg_func_end(char* id);
bool cg_func_call(char* id);

bool cg_stack_p_int(int i);

int code_to_stdout();

#endif //IFJ_FINAL_CODE_GENERATOR_H

//
// Created by Matúš on 6. 12. 2020.
//
#include <stdbool.h>
#include <stdio.h>

#include "code_generator.h"
#include "symtable.h"

#include "prec_analysis.h"
#include "parser.h"


dstring code;
HashTable globalHt;

bool cg_main(){
    ADD_INSTR("\n# Zaciatok main"); //ADD_INSTR aj odriadkuje !
    ADD_INSTR("LABEL $$main");
    ADD_INSTR("CREATEFRAME");
    ADD_INSTR("PUSHFRAME");

    return true;
}

bool cg_main_end(){
    ADD_INSTR("# End of main");

    ADD_INSTR("POPFRAME");
    ADD_INSTR("CLEARS");
    return true;
}

bool cg_func_start(char* id){
    ADD_CODE("\n# Function begin "); //ADD_CODE neodriadkuje
    ADD_CODE(id);
    ADD_CODE("\n");
    ADD_CODE("LABEL $"); ADD_CODE(id); ADD_CODE("\n");

    ADD_INSTR("PUSHFRAME");

    return true;
}

bool cg_func_end(char* id){
    ADD_CODE("# Function end ");
    ADD_CODE(id);
    ADD_CODE("\n");
    ADD_INSTR("POPFRAME");
    ADD_INSTR("RETURN");
    return true;
}

bool cg_func_call(char* id)
{
    ADD_CODE("CALL $");
    ADD_CODE(id);
    ADD_CODE("\n");

    return true;
}

bool cg_stack_p_int(int i)
{
    ADD_CODE("PUSHS int@");
    ADD_CODE_INT(i);
    ADD_CODE("\n");
    return true;
}



int code_to_stdout(){
    fprintf(stdout, "%s", code.str);
    strFree(&code);
}
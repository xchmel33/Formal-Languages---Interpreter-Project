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

bool cg_code_header()
{
    ADD_INSTR(".IFJcode20");
    ADD_INSTR("#Program start");

    ADD_INSTR("DEFVAR GF@$$expr_result");
    ADD_INSTR("DEFVAR GF@$$op1");
    ADD_INSTR("DEFVAR GF@$$op2");
    //ADD_INSTR("DEFVAR GF@$$typ_op_1");
    ADD_INSTR("DEFVAR GF@$$concat");
    //ADD_INSTR("DEFVAR GF@$$typ_op_2");
    //ADD_INSTR("DEFVAR GF@$$expr_result_type");

    ADD_INST("JUMP $$main");

    return true;
}

bool cg_main()
{
    ADD_INSTR("\n# Main begin"); //ADD_INSTR aj odriadkuje !
    ADD_INSTR("LABEL $$main");
    ADD_INSTR("CREATEFRAME");
    ADD_INSTR("PUSHFRAME");

    return true;
}

bool cg_main_end()
{
    ADD_INSTR("# End of main");

    ADD_INSTR("POPFRAME");
    ADD_INSTR("CLEARS");
    return true;
}

bool cg_func_start(char* id)
{
    ADD_CODE("\n# Function begin "); //ADD_CODE neodriadkuje
    ADD_CODE(id);
    ADD_CODE("\n");
    ADD_CODE("LABEL $"); ADD_CODE(id); ADD_CODE("\n");

    ADD_INSTR("PUSHFRAME");

    return true;
}

bool cg_func_end(char* id)
{
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

bool cg_label(char* name, int id)
{
    ADD_CODE("LABEL $"); 
    ADD_CODE(name); 
    ADD_CODE_INT(id); 
    ADD_CODE("\n");
    return true;
}

bool cg_clear_stack() {
    ADD_INSTR("CLEARS");
    return true;
}

int code_to_stdout()
{
    fprintf(stdout, "%s", code.str);
    strFree(&code);
}

bool cg_stack_push_operation(char* sym) 
{
    ADD_CODE("PUSHS ");
    ADD_CODE("GF@");
    ADD_CODE(sym);
    ADD_CODE("\n");
    return true;
}

bool cg_operation(TokenType operation) 
{
    switch (operation)
    {    
    case(TT_ADD):
        ADD_INSTR("ADDS");
        return true;
    case(TT_SUB):
        ADD_INSTR("SUBS");
        return true;
    case(TT_MUL):
        ADD_INSTR("MULS");
        return true;
    case(TT_DIV):
        ADD_INSTR("DIVS");
        return true;
    case(TT_LESS_THAN):
        ADD_INSTR("LTS");
        return true;
    case(TT_MORE_THAN):
        ADD_INSTR("GTS");
        return true;
    case(TT_EQUAL):
        ADD_INSTR("EQS");
        return true;
    case(TT_LESS_OR_EQUAL):
        ADD_INSTR("LTS");
        cg_stack_push_operation("$$op2");
        cg_stack_push_operation("$$op1");
        ADD_INSTR("EQS");
        ADD_INSTR("ORS");            
        return true;
    case(TT_MORE_OR_EQUAL):
        ADD_INSTR("GTS");
        cg_stack_push_operation("$$op2");
        cg_stack_push_operation("$$op1");
        ADD_INSTR("EQS");
        ADD_INSTR("ORS");
        return true;
    case(TT_NOT_EQUAL):
        ADD_INSTR("EQS");
        ADD_INSTR("NOTS");            
        return true;
}

Token* cg_count() 
{

}
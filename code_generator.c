#define _CRT_SECURE_NO_WARNINGS

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

bool cg_header()
{
    ADD_INSTR("#Program start");
    ADD_INSTR(".IFJcode20");

    ADD_INSTR("#Def basic vars");
    ADD_INSTR("DEFVAR GF@$$expr_result");
    ADD_INSTR("DEFVAR GF@$$op1");
    ADD_INSTR("DEFVAR GF@$$op2");
    ADD_INSTR("JUMP $$main");

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

bool cg_label(char* name, int id)
{
    ADD_CODE("LABEL $"); 
    ADD_CODE(name); 
    ADD_CODE_INT(id); 
    ADD_CODE("\n");
    return true;
}

bool cg_clear_stack() 
{
    ADD_INSTR("CLEARS");
    return true;
}

void code_to_stdout()
{
    fprintf(stdout, "%s", code.str);
    strFree(&code);
}

bool cg_stack_push_int(int value)
{
    ADD_CODE("PUSHS int@");
    ADD_CODE_INT(value); 
    ADD_CODE("\n");
    return true;
}

bool cg_stack_push_double(double value) 
{
    ADD_CODE("PUSHS float@");
    ADD_CODE_DOUBLE(value); 
    ADD_CODE("\n");
    return true;
}

bool cg_stack_push_string(char* value)
{
    ADD_CODE("PUSHS string@");
    ADD_CODE(value);
    ADD_CODE("\n");
    return true;
}

bool cg_stack_push_global(char* key) 
{
    ADD_CODE("PUSHS ");
    ADD_CODE("GF@");
    ADD_CODE(key);
    ADD_CODE("\n");
    return true;
}

bool cg_stack_pop_id(char* var, bool local) 
{
    ADD_CODE("POPS ");
    if (local) 
        ADD_CODE("LF@");
    else 
        ADD_CODE("GF@");
    ADD_CODE(var); 
    ADD_CODE("\n");
    return true;
}

bool cg_stack_push_identifier(char* key, HashTable *globaltable)
{
    ADD_CODE("PUSHS ");
    if (htSearch(globaltable, key) == NULL) {
        ADD_CODE("GF@");
    }
    else {
        ADD_CODE("LF@");
    }
    ADD_CODE(key);
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
        cg_stack_push_global("$$op2");
        cg_stack_push_global("$$op1");
        ADD_INSTR("EQS");
        ADD_INSTR("ORS");
        return true;
    case(TT_MORE_OR_EQUAL):
        ADD_INSTR("GTS");
        cg_stack_push_global("$$op2");
        cg_stack_push_global("$$op1");
        ADD_INSTR("EQS");
        ADD_INSTR("ORS");
        return true;
    case(TT_NOT_EQUAL):
        ADD_INSTR("EQS");
        ADD_INSTR("NOTS");
        return true;
    }
    return false;
}

bool cg_var_declare(char* id) 
{
    ADD_CODE("DEFVAR LF@");
    ADD_CODE(id);
    ADD_CODE("\n");
    return true;
}

bool cg_def_val_var(DataType value)
{
    switch (value) {

    case T_INT:
        ADD_CODE("int@0");
        break;
    case T_FLOAT64:
        ADD_CODE("float@0");
        break;
    case T_STRING:
        ADD_CODE("string@");
        break;

    default:
        return false;
    }
    return true;
}

bool cg_var_to_default_val(char* id, DataType value)
{
    ADD_CODE("MOVE LF@");
    ADD_CODE(id);
    ADD_CODE(" "); // space ! MOVE LF@id int@x
    if (cg_def_val_var(value) == 1)
    {
        ADD_CODE("\n");
        return true;
    }
    return false;
}

bool cg_var_val(Token token)
{
    char val_string[MAX_DIGITS];

    switch (token.type) {

    case TT_INTEGER:
        sprintf(val_string, "%d", token.attribute.integer);
        ADD_CODE("int@");
        ADD_CODE(val_string);

        break;
    case TT_DECIMAL:
        sprintf(val_string, "%a", token.attribute.decimal);
        ADD_CODE("float@");
        ADD_CODE(val_string);

        break;
    case TT_STRING:
        break;

    default:
        return false;
    }
    return true;
}

bool cg_var_to_any_val(char* id, Token token)
{
    ADD_CODE("MOVE LF@");
    ADD_CODE(id);
    ADD_CODE(" "); // space ! MOVE LF@id int@x
    if (cg_var_val(token) == 0)
    {
        return false;
    }
    return true;
}

bool cg_print_id(TableItem* data)
{
    ADD_INSTR("\n # Print id");
    ADD_CODE("WRITE");
    ADD_CODE(" ");
    ADD_CODE(data->key);
    ADD_CODE("\n");
    return true;
}

bool cg_print_value(char* val, DataType type)
{
    ADD_INSTR("\n# Print value");

    switch (type) {

        case T_INT:
            ADD_CODE("WRITE int@");
            ADD_CODE(val);
            ADD_CODE("\n");
            return true;

        case T_FLOAT64:
            ADD_CODE("WRITE float@");
            ADD_CODE(val);
            ADD_CODE("\n");
            return true;
        case T_STRING:
            ADD_CODE("WRITE string@");
            return true;

        default:
            return false;
    }
}

bool cg_find_type(Datatype data_t)
{
    switch (data_t) {

        case INT:
            ADD_CODE("int");
            return true;

        case FLOAT64:
            ADD_CODE("float");
            return true;
        case STRING:
            ADD_CODE("string");
            return true;
        default:
            return false;
    }
}

bool cg_variants_of_input(Datatype data_t)
{
    ADD_INSTR("\n # CG func input");
    ADD_CODE("READ GF@$$expr_result");
    if (cg_find_type(data_t) == true)
    {
        ADD_CODE("\n");
        return true;
    }
    else
    {
        return false;
    }
}
bool cg_concat() 
{   
    cg_stack_pop_id("$$op_2", false);
    cg_stack_pop_id("$$op_1", true);
    cg_stack_push_global("$$expr_result");
    ADD_CODE("CONCAT GF@$$expr_result GF@$$op_1 GF@$$op_2\n");
    ADD_CODE("\n");
    return true;
}

bool cg_other_operations(TokenType operation) 
{
    cg_stack_push_global("$$expr_result");
    cg_operation(operation);
    ADD_CODE("\n");
    return true;
}
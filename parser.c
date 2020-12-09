#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"
#include "parser.h"
#include "prec_analysis.h"
#include "code_generator.h"

#define DEBUG 1

#define PRINT_DEBUG(txt) \
    if (DEBUG) \
        printf(txt)



Token* act_token;
HashTable* act_table; //Global
HashTable* local_table; //Local
int error_code = 0;
int iteration_count = 0;


unsigned param = 0;

dstring code;

void set_active_token(Token* token)
{
    act_token = token;
}
void set_active_table(HashTable* table)
{
    act_table = table;
}




int base_cond(Token* token)
{

    if (token->attribute.keyword == PACKAGE)
    {
        GetToken(token);
        if (strcmp(token->attribute.string->str, "main") == 0)
        {
            PRINT_DEBUG("Package main ok \n ");
            return ERR_OK;
        }
        if (token->type == TT_EOF)
        {
            PRINT_DEBUG("End of program \n");
            return ERR_OK;
        }
    }
    else
    {
        return ERR_PARSER;
    }
    return ERR_PARSER;
}


int body() {
    PRINT_DEBUG("Body start \n");

    GET_TOKEN; if (error_code != 0) return error_code;
    PRINT_DEBUG("AFTER fisrt GET TOKEN \n");
    if (act_token->type == TT_EOL)
    {
        do {
            GET_TOKEN; if (error_code != 0) return error_code;
        } while (act_token->type == TT_EOL);
    }
    iteration_count++;
    if (iteration_count == 1)
    {
        local_table = (HashTable*)malloc(sizeof(HashTable));
        strInit(&code);
        cg_header();
        htInit(local_table);
        base_cond(act_token);

        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->type != TT_EOL)
        {
            return ERR_PARSER;
        }
        else
        {
            GET_TOKEN; if (error_code != 0) return error_code;
            if (act_token->type == TT_EOL)
            {
                do {
                    GET_TOKEN; if (error_code != 0) return error_code;
                } while (act_token->type == TT_EOL);
            }
            if (act_token->type == TT_IDENTIFIER)
            {
                return ERR_PARSER;
            }
        }
    }
    if (act_token->attribute.keyword == FUNC) // ID "FUNC" -> "ID"
    {
        error_code = def_func(); //"ID"
        if (error_code != 0)
        {
            PRINT_DEBUG("Error guard def func ! \n");
            return error_code;
        }
    }
    if (act_token->type == TT_IDENTIFIER)
    {
        error_code = statement();
        if (error_code != 0)
        {
            return error_code;
        }
    }


    if (act_token->type == TT_EOF)
    {
        if (htSearch(act_table, "main") == NULL)
        {
            return ERR_PARSER;
        }
        cg_main_end();
        return ERR_OK;
    }
    body();
}

int def_func()
{
    PRINT_DEBUG("Def func \n");


    GET_TOKEN; if (error_code != 0) return error_code;

    if (act_token->type == TT_IDENTIFIER)
    {
        if (strcmp(act_token->attribute.string->str, "print") == 0)
        {
            PRINT_DEBUG("Function pre declared print \n");
            GET_TOKEN; if (error_code != 0) return error_code;
            if (act_token->type == TT_L_BRACKET)
            {
                int error_pom = terms();
                if (error_pom != 0)
                {
                    return error_pom;
                }
                return ERR_OK;
            }
        }
        if (strcmp(act_token->attribute.string->str, "inputi") == 0)
        {
            PRINT_DEBUG("INPUTI \n");
            GET_TOKEN; if (error_code != 0) return error_code;
            if (act_token->type == TT_L_BRACKET)
            {
                GET_TOKEN; if (error_code != 0) return error_code;
                if (act_token->type == TT_R_BRACKET)
                {
                    if (cg_variants_of_input(INT) == true)
                    {
                        return ERR_OK;
                    }
                    else
                    {
                        return ERR_INTERNAL;
                    }
                }
            }
            else
            {
                return ERR_PARSER;
            }
        }
        if (strcmp(act_token->attribute.string->str, "inputf") == 0)
        {
            PRINT_DEBUG("INPUTI \n");
            GET_TOKEN; if (error_code != 0) return error_code;
            if (act_token->type == TT_L_BRACKET)
            {
                GET_TOKEN; if (error_code != 0) return error_code;
                if (act_token->type == TT_R_BRACKET)
                {
                    if (cg_variants_of_input(FLOAT64) == true)
                    {
                        return ERR_OK;
                    }
                    else
                    {
                        return ERR_INTERNAL;
                    }
                }
            }
            else
            {
                return ERR_PARSER;
            }
        }
        TableItem func;
        if (strcmp(act_token->attribute.string->str, "main") == 0)
        {
            cg_main();
        }
        else
        {

            cg_func_start(act_token->attribute.string->str);
        }
        func.next_item = NULL;
        size_t len = strlen(act_token->attribute.string->str);
        func.key = malloc(len * sizeof(char)); //sizeof(char) always 1
        strcpy(func.key, act_token->attribute.string->str);
        func.data.type = T_FUNC;
        PRINT_DEBUG("Table check \n");
        htInsert(act_table, func.key, func.data);
        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->type == TT_L_BRACKET)
        {
            error_code = params(&func);
            if (error_code == 0)
            {
                if (blockBeginEOL_check() != 0)
                {
                    PRINT_DEBUG("Error block begin eol \n");
                    return ERR_PARSER;
                }
                int error_pom = statements(&func); // pomocna aby sme vedeli kt. error vracat
                if (error_pom != 0)
                {
                    return error_pom;
                    PRINT_DEBUG("Statements error \n");
                }
                else
                {
                    cg_func_end(func.key);
                }
            }
            else
            {
                return error_code;
            }
        }
        return ERR_OK;
    }
    else
    {
        PRINT_DEBUG("Chyba definiecie funkcie \n");
        return ERR_DEF;
    }
}

int params(TableItem* func)
{
    TableData* data = (TableData*)malloc(sizeof(TableData));

    int i = 0;
    int param_counter = 0;
    while (1)
    {

        if (act_token->type == TT_L_BRACKET) {
            GET_TOKEN; if (error_code != 0) return error_code;
        }

        if (act_token->type == TT_IDENTIFIER)
        {

            data->param[param].identifier = malloc(sizeof(char));
            strcpy(data->param[param].identifier, act_token->attribute.string->str);
            param_counter++;
            GET_TOKEN; if (error_code != 0) return error_code;
            if (!(act_token->type == TT_INTEGER || act_token->type == TT_STRING || act_token->type == TT_DECIMAL))
            {
                return ERR_PARSER;
            }
        }
        if (act_token->type == TT_DATATYPE) {
            switch (act_token->attribute.datatype) {
                case INT:
                    data->param[param].type = 0;
                    GET_TOKEN; if (error_code != 0) return error_code;
                    if (act_token->type != TT_IDENTIFIER)
                    {
                        return ERR_PARSER;
                    }

                    break;
                case FLOAT64:
                    data->param[param].type = 1;
                    GET_TOKEN; if (error_code != 0) return error_code;
                    if (act_token->type != TT_IDENTIFIER)
                    {
                        return ERR_PARSER;
                    }
                    break;
                case STRING:
                    data->param[param].type = 2;
                    GET_TOKEN; if (error_code != 0) return error_code;
                    if (act_token->type != TT_IDENTIFIER)
                    {
                        return ERR_PARSER;
                    }
                    break;
                default:
                    return ERR_PARSER;
            }
        }

        /*if (act_token->type == TT_DATATYPE)
        {
            return TT_DATATYPE;
        }*/
        if (act_token->type == TT_COMMA) {
            param++; //indexovanie parametrov
            GET_TOKEN; if (error_code != 0) return error_code;

            i++;
        }
        else
        {
            if (act_token->type == TT_R_BRACKET)
            {
                GET_TOKEN; if (error_code != 0) return error_code;

                if (act_token->type == TT_BLOCK_BEGIN)
                {
                    data->type = T_FUNC;
                    data->number_params = param_counter;
                    data->return_type = T_NONE;
                    data->var = false;
                    htInsert(act_table, func->key, *data);
                    if (param_counter != 0)
                    {
                        switch (data->param[param].type) {

                            case INT:
                                data->param->type = INT;
                                break;
                            case FLOAT64:
                                data->param->type = FLOAT64;
                                break;
                            case STRING:
                                data->param->type = STRING;
                                break;
                            default:
                                return ERR_DEF_TYPE;

                        }
                        data->var = true;
                        htInsert(local_table, data->param[param].identifier, *data);
                    }
                    return ERR_OK;
                }

                if (act_token->type == TT_L_BRACKET) {
                    GET_TOKEN; if (error_code != 0) return error_code;
                }
                else
                {
                    PRINT_DEBUG("Error def in params \n");
                    return ERR_PARSER;
                }
                do {
                    if (act_token->type == TT_R_BRACKET) {
                        data->type = T_FUNC;
                        data->number_params = param_counter;
                        data->return_type = T_NONE;
                        data->var = false;
                        htInsert(act_table, func->key, *data);
                        return ERR_OK;
                    }
                    if (act_token->type == TT_DATATYPE) {
                        switch (act_token->attribute.datatype) {
                            case INT:
                                data->return_type = 0;
                                GET_TOKEN; if (error_code != 0) return error_code;
                                break;
                            case FLOAT64:
                                data->return_type = 1;
                                GET_TOKEN; if (error_code != 0) return error_code;
                                break;
                            case STRING:
                                data->return_type = 2;
                                GET_TOKEN; if (error_code != 0) return error_code;
                                break;
                            default:
                                return ERR_PARSER;
                        }
                    }
                    if (act_token->type == TT_COMMA) {
                        param++; //indexovanie parametrov
                        GET_TOKEN; if (error_code != 0) return error_code;

                    }
                    else
                    {
                        if (act_token->type == TT_R_BRACKET)
                        {
                            data->type = T_FUNC;
                            data->number_params = param_counter;
                            data->var = false;
                            htInsert(act_table, func->key, *data);
                            param = 0; //resetovanie indexovania parametrov
                            return ERR_OK;
                        }
                        else {
                            PRINT_DEBUG("ERROR def func in params \n");
                            return ERR_DEF;
                        }
                    }
                } while (1);
            }
            else
            {
                PRINT_DEBUG("ERROR def func in params ! \n");
                return ERR_PARSER;
            }
        }

    }
}

int statements()
{

    while (act_token->type != TT_BLOCK_BEGIN && act_token->type != TT_BLOCK_END)
    {
        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->type == TT_BLOCK_BEGIN || act_token->type == TT_BLOCK_END)
        {
            break;
        }

        switch (act_token->attribute.keyword)
        {
            case IF:
                PRINT_DEBUG("Statements IF \n");
                int error_pom = statement(); // pomocna aby sme vedeli kt. error vracat
                return error_pom;
            case FOR:
                PRINT_DEBUG("Statements FOR \n");
                // for -> init -> ; -> expres -> ; -> assign -> { eol statements... eol }
                if (init(act_token) != 0)
                {
                    return ERR_PARSER;
                }
                GET_TOKEN; if (error_code != 0) return error_code;
                if (act_token->type == TT_SEMICOLON)
                {
                    PRINT_DEBUG("For 1. semicolon ok \n");
                    GET_TOKEN; if (error_code != 0) return error_code;
                }
                else
                {
                    return ERR_PARSER;
                }
                // assing teba dorobit neviem ako bude vyzerat value();!!!!!!!!!!!!!!!!


                break;
            case FUNC:
                PRINT_DEBUG("Statements FUNC \n");
                int error = def_func();
                if (error != 0)
                {
                    return error;
                }
                else
                {
                    return ERR_OK;
                }

            case PACKAGE:
                PRINT_DEBUG("Statements PACKAGE \n");
                break;
            case RETURN:
                PRINT_DEBUG("Statements RETURN \n");
                break;
            default:
                break;
        }
        if (act_token->type == TT_IDENTIFIER)
        {
            error_code = statement();
            if (error_code != 0)
            {
                return error_code;
            }
        }
        if (act_token->type == TT_INIT || act_token->type == TT_ASSIGN)
        {
            return ERR_PARSER;
        }
    }
    if (act_token->type == TT_BLOCK_END)
    {
        PRINT_DEBUG("Block end of func ! \n");
        if (blockEnd_EOL_check() == 0)
        {
            return ERR_OK;
        }
        else
        {
            return ERR_PARSER;
        }
    }
    if (act_token->type == TT_EOF)
    {
        return ERR_OK;
    }
    return ERR_OK;
}
int statement()
{
    Token* prev_token1 = initToken();
    Token* prev_token2 = initToken();
    TableData* data = (TableData*)malloc(sizeof(TableData));
    TableItem* temporary_ht;

    if (act_token->type == TT_BLOCK_END) {
        return ERR_OK;
    }

    //if -> else start
    if (act_token->attribute.keyword == IF)
    {
        error_code = GetToken(prev_token2);
        if (error_code != 0) return error_code;

        error_code = GetToken(prev_token1);
        if (error_code != 0) return error_code;

        //var op
        if (prev_token1->type >= TT_ADD && prev_token1->type <= TT_R_BRACKET)
        {
            ActivateResources(local_table);
            int error_pom = expression(prev_token2, prev_token1);
            if (error_pom != 0) {
                return error_pom;
            }
            act_token = prev_token1;
        }
        else {
            //bracket var op
            GET_TOKEN; if (error_code != 0) return error_code;
            if (act_token->type >= TT_ADD && act_token->type <= TT_R_BRACKET)
            {
                ActivateResources(local_table);
                int error_pom = expression(prev_token1, act_token);
                if (error_pom != 0) {
                    return error_pom;
                }
            }
        }

        //{
        error_code = blockBeginEOL_check();
        if (error_code != 0) return error_code;

        GET_TOKEN; if (error_code != 0) return error_code;
        error_code = statement();
        if (error_code != 0) return error_code;

        //else
        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->attribute.keyword == ELSE) {

            //{
            GET_TOKEN; if (error_code != 0) return error_code;
            error_code = blockBeginEOL_check();
            if (error_code != 0) return error_code;

            GET_TOKEN; if (error_code != 0) return error_code;
            error_code = statement();
            if (error_code != 0) return error_code;

            //}
            error_code = blockEnd_EOL_check();
            if (error_code != 0) return error_code;
            //if -> else finished
            return ERR_OK;
        }
        else {
            return ERR_PARSER;
        }

    }
    if (act_token->type == TT_IDENTIFIER)
    {
        if (strcmp(act_token->attribute.string->str, "print") == 0) {
            PRINT_DEBUG("Function pre declared print \n");
            GET_TOKEN;
            if (error_code != 0) return error_code;
            if (act_token->type == TT_L_BRACKET) {
                int error_pom = terms();
                if (error_pom != 0) {
                    return error_pom;
                }
                return ERR_OK;
            }
        }
        if (strcmp(act_token->attribute.string->str, "inputi") == 0) {
            PRINT_DEBUG("INPUTI \n");
            GET_TOKEN;
            if (error_code != 0) return error_code;
            if (act_token->type == TT_L_BRACKET) {
                GET_TOKEN;
                if (error_code != 0) return error_code;
                if (act_token->type == TT_R_BRACKET) {
                    if (cg_variants_of_input(INT) == true) {
                        return ERR_OK;
                    }
                    else {
                        return ERR_INTERNAL;
                    }
                }
            }
            else {
                return ERR_PARSER;
            }
        }
        if (strcmp(act_token->attribute.string->str, "inputf") == 0) {
            PRINT_DEBUG("INPUTI \n");
            GET_TOKEN;
            if (error_code != 0) return error_code;
            if (act_token->type == TT_L_BRACKET) {
                GET_TOKEN;
                if (error_code != 0) return error_code;
                if (act_token->type == TT_R_BRACKET) {
                    if (cg_variants_of_input(FLOAT64) == true) {
                        return ERR_OK;
                    }
                    else {
                        return ERR_INTERNAL;
                    }
                }
            }
            else {
                return ERR_PARSER;
            }
        }
    }
    if (htSearch(act_table, act_token->attribute.string->str) != NULL && act_token->attribute.datatype == T_FUNC)
    {
        PRINT_DEBUG("Volanie definovanej funkcie ! \n ");
    }

    prev_token1 = act_token;
    size_t len = strlen(prev_token1->attribute.string->str);
    char* name_of_id = malloc(sizeof(len));
    strcpy(name_of_id, prev_token1->attribute.string->str);

    if (htSearch(local_table, act_token->attribute.string->str) == NULL)
    {
        //nebol este inicializovany => next token musi byt :=
        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->type != TT_INIT)
        {
            return ERR_DEF;
        }
        else
        {
            PRINT_DEBUG("INIT \n");

            error_code = init(prev_token1);
            if (error_code != 0)
            {
                return error_code;
            }
            htInsert(local_table, name_of_id, *data);
        }
    }
    else
    {
        PRINT_DEBUG("ID sa uz nachadza v localhash table ! \n");
        prev_token1 = act_token;
        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->type == TT_ASSIGN)
        {
            error_code = assign(prev_token1);
            if (error_code != 0)
            {
                return error_code;
            }
        }


    }

    if (act_token->type == TT_COMMA)
    {

        int id_counter = 0;
        PRINT_DEBUG("ID, ID, IDn ... \n");
        while (1) {
            if (act_token->type == TT_ASSIGN || act_token->type == TT_INIT) {
                statements();
            }
            GET_TOKEN; if (error_code != 0) return error_code;
            if (act_token->type != TT_IDENTIFIER) {
                return ERR_PARSER;
            }
            data->var = true;
            htInsert(local_table, act_token->attribute.string->str, *data);
            GET_TOKEN; if (error_code != 0) return error_code;
            id_counter++;
        }
    }
    return ERR_OK;
}


int blockBeginEOL_check()
{
    if (act_token->type == TT_R_BRACKET)
    {
        GET_TOKEN; if (error_code != 0) return error_code;
    }

    if (act_token->type != TT_BLOCK_BEGIN)
    {
        PRINT_DEBUG("Block begin + EOL ERROR ! \n");
        return ERR_PARSER;
    }
    else
    {
        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->type == TT_EOL)
        {
            PRINT_DEBUG("Block begin + EOL OK ! \n");
            return ERR_OK;
        }
        else
        {
            return ERR_PARSER;
        }
    }
}
int blockEnd_EOL_check()
{
    if (act_token->type != TT_BLOCK_END)
    {
        PRINT_DEBUG("Block begin + EOL ERROR ! \n");
        return ERR_PARSER;
    }
    else
    {
        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->type == TT_EOL)
        {
            PRINT_DEBUG("Block END + EOL OK ! \n");
            return ERR_OK;
        }
        else
        {
            return ERR_PARSER;
        }
    }
}
int IfblockEnd_check()
{
    Token* prev_token = malloc(sizeof(Token));
    if (act_token->type == TT_BLOCK_END)
    {
        prev_token->type = act_token->type;
        GET_TOKEN; if (error_code != 0) return error_code;
    }
    else
    {
        return ERR_PARSER;
    }

    if (act_token->attribute.keyword == ELSE && prev_token->type == TT_BLOCK_END)
    {
        GET_TOKEN; if (error_code != 0) return error_code;
        return ERR_OK;
    }
    else
    {
        return ERR_PARSER;
    }
}

int init(Token* left_id) {

    Token* prev_token = initToken();

    if (act_token->attribute.keyword == FOR)
    {
        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->type == TT_IDENTIFIER)
        {
            left_id = act_token;
        }
        else
        {
            return ERR_PARSER;
        }
        GET_TOKEN; if (error_code != 0) return error_code;
        if (act_token->type != TT_INIT)
        {
            return ERR_PARSER;
        }
        init(prev_token);
        GET_TOKEN; if (error_code != 0) return error_code;
    }
    if (act_token->type == TT_INIT)
    {
        error_code = GetToken(prev_token);
        if (error_code != 0) return error_code;

        GET_TOKEN; if (error_code != 0) return error_code;

        // operator ID

        if (act_token->type >= TT_ADD && act_token->type <= TT_R_BRACKET)
        {
            ActivateResources(local_table);
            int error_pom = expression(prev_token, act_token);
            if (error_pom != 0)
            {
                return error_pom;
            }
            return ERR_OK;
        }
        else
        {
            if (act_token->type == TT_EOL)
            {
                if (prev_token->type == TT_INTEGER) {
                    left_id->attribute.datatype = INT;
                    left_id->attribute.integer = prev_token->attribute.integer;
                }
                else if (prev_token->type == TT_DECIMAL) {
                    left_id->attribute.datatype = FLOAT64;
                    left_id->attribute.decimal = prev_token->attribute.decimal;
                }
                else if (prev_token->type == TT_STRING) {
                    left_id->attribute.datatype = STRING;
                    strCopyString(left_id->attribute.string, prev_token->attribute.string);
                }

                return ERR_OK;
            }
        }

    }
    return ERR_PARSER;
}

int assign(Token* left_id)
{
    Token* prev_token = initToken();
    if (act_token->type == TT_ASSIGN)
    {
        GetToken(prev_token);
    }
    if (prev_token->type == TT_IDENTIFIER)
    {
        if (strcmp(prev_token->attribute.string->str,"print") == 0)
        {
            return ERR_SEM_OTHR;
        }

        if (act_token->attribute.datatype == T_FUNC)
        {
            PRINT_DEBUG("Assign func \n");
        }
        GET_TOKEN; if (error_code != 0) return error_code;

        // operator ID

        if (act_token->type >= TT_ADD && act_token->type <= TT_R_BRACKET)
        {
            ActivateResources(local_table);
            int error_pom = expression(prev_token, act_token);
            if (error_pom != 0)
            {
                return error_pom;
            }
            return ERR_OK;
        }
        else
        {
            if (act_token->type == TT_EOL)
            {
                if (prev_token->type == TT_INTEGER) {
                    left_id->attribute.datatype = INT;
                    left_id->attribute.integer = prev_token->attribute.integer;
                }
                else if (prev_token->type == TT_DECIMAL) {
                    left_id->attribute.datatype = FLOAT64;
                    left_id->attribute.decimal = prev_token->attribute.decimal;
                }
                else if (prev_token->type == TT_STRING) {
                    left_id->attribute.datatype = STRING;
                    strCopyString(left_id->attribute.string, prev_token->attribute.string);
                }

                return ERR_OK;
            }
        }
        return ERR_PARSER;
    }
}

int terms()
{
    PRINT_DEBUG("Func Terms printing \n");
    GET_TOKEN; if (error_code != 0) return error_code;
    TableItem* data;
    switch (act_token->type) {

        case TT_IDENTIFIER:

            data = htSearch(act_table, act_token->attribute.string->str);
            if (data != NULL)
            {
                if (cg_print_id(data) == false)
                {
                    return ERR_INTERNAL;
                }
            }
            else if ((data = htSearch(local_table, act_token->attribute.string->str)) != NULL)
            {
                if (cg_print_id(data) == false)
                {
                    return ERR_INTERNAL;
                }
            }
            else
            {
                return ERR_INTERNAL;
            }
            break;
        case TT_INTEGER:
        {
            char pom[50];
            sprintf(pom, "%d", act_token->attribute.integer); //nalepenie cisla na sting resp. konverzia int na string
            if (cg_print_value(pom, T_INT) == false) {
                return ERR_INTERNAL;
            }
            break;
        }
        case TT_DECIMAL:
        {
            char pom[50];
            sprintf(pom, "%f", act_token->attribute.decimal); //nalepenie cisla na sting resp. konverzia int na string
            if (cg_print_value(pom, T_FLOAT64) == false) {
                return ERR_INTERNAL;
            }
            break;
        }
        case TT_STRING:

            break;
        case TT_R_BRACKET:
            ADD_CODE("\n");
            return ERR_OK;
        default:
            return ERR_PARSER;
    }

    GET_TOKEN; if (error_code != 0) return error_code;

    switch (act_token->type) {
        case TT_R_BRACKET:
            ADD_CODE("\n");
            return ERR_OK;
        case TT_COMMA:
            ADD_CODE(" ");
            return terms();
        default:
            return ERR_PARSER;
    }
}
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"
#include "parser.h"
#include "prec_analysis.h"
#include "dstring.h"
#include "scanner.h"
#include "symtable.h"
#include "code_generator.h"

#define DEBUG 1

#define PRINT_DEBUG(txt) \
    if (DEBUG) \
        printf(txt)



Token* act_token;
HashTable* act_table; //Global
HashTable* local_table; //Local

int depth_level = 0;  //level zanorenia do funkcie
bool func_flag = 0; //flag funkcie 0 - 1
int iteration_count = 0; //pocet iteracii
int error_guard = 0;
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
            return ERR_OK; //Chybaju err stavy
        }
        if (token->type == TT_EOF)
        {
            PRINT_DEBUG("End of program \n");
            return 0;
        }
    }
    else
    {
        return ERR_PARSER;
    }
}


int body() {
    PRINT_DEBUG("Body start \n");

    GET_TOKEN;
    if (act_token->type == TT_EOL)
    {
        do {
            GET_TOKEN;
        }while (act_token->type == TT_EOL);
    }
    iteration_count++;
    if (iteration_count == 1)
    {
        local_table = (HashTable*)malloc(sizeof(HashTable));
        strInit(&code);
        htInit(local_table);
        base_cond(act_token);
        GET_TOKEN;
    }
    if (act_token->attribute.keyword == FUNC) // ID "FUNC" -> "ID"
    {
        error_guard = def_func(); //"ID"
        if (error_guard != 0)
        {
            PRINT_DEBUG("Error guard def func ! \n");
            return error_guard;
        }
    }
    if (act_token->type == TT_IDENTIFIER)
    {
        statement();
    }


    if (act_token->type == TT_EOF )
    {
        cg_main_end();
        return ERR_OK;
    }
    body();
}

int def_func()
{
    PRINT_DEBUG("Def func \n");
    func_flag = 1;
    depth_level++;
    GET_TOKEN;

    if (act_token->type == TT_IDENTIFIER)
    {
        if (strcmp(act_token->attribute.string->str,"print") == 0)
        {
            PRINT_DEBUG("Function pre declared print \n");
            GET_TOKEN;
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
        TableItem func;
        if (strcmp(act_token->attribute.string->str,"main") == 0)
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
        strcpy(func.key,act_token->attribute.string->str);
        func.data.type = T_FUNC;
        PRINT_DEBUG("Table check \n");
        htInsert(act_table, func.key, func.data);
        GET_TOKEN;
        if (act_token->type == TT_L_BRACKET)
        {

            if (!params(&func))
            {
                if (blockBeginEOL_check() !=0)
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
                return ERR_DEF;
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
    TableData *data = (TableData *) malloc(sizeof(TableData));
    int pom = TT_COMMA;
    int i = 0;
    int param_counter = 0;
    while (pom == TT_COMMA)
    {
        size_t len = 0;
        if (act_token->type == TT_L_BRACKET) {
            GET_TOKEN;
        }
        /*if (act_token->type == TT_R_BRACKET) {
            GET_TOKEN;
        }*/
        if (act_token->type == TT_IDENTIFIER) {
            len = strlen(act_token->attribute.string->str);
            data->param[param].identifier = malloc(sizeof(len * sizeof(char)));
            strcpy(data->param[param].identifier, act_token->attribute.string->str);
            param_counter++;
            GET_TOKEN;
        }
        if (act_token->type == TT_DATATYPE) {
            switch (act_token->attribute.datatype) {
                case INT:
                    data->param[param].type = 0;
                    GET_TOKEN;
                    break;
                case FLOAT64:
                    data->param[param].type = 1;
                    GET_TOKEN;
                    break;
                case STRING:
                    data->param[param].type = 2;
                    GET_TOKEN;
                    break;
            }
        }

        /*if (act_token->type == TT_DATATYPE)
        {
            return TT_DATATYPE;
        }*/
        if (act_token->type == TT_COMMA) {
            param++; //indexovanie parametrov
            GET_TOKEN;
            pom = TT_COMMA;
            i++;
        }
        else
        {
            if (act_token->type == TT_R_BRACKET)
            {
                GET_TOKEN;

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
                                break;
                        }
                        data->var = true;
                        htInsert(local_table, data->param[param].identifier, *data);
                    }
                    return ERR_OK;
                }

                if (act_token->type == TT_L_BRACKET) {
                    GET_TOKEN;
                }
                else
                {
                    PRINT_DEBUG("Error def in params \n");
                    return ERR_DEF;
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
                                GET_TOKEN;
                                break;
                            case FLOAT64:
                                data->return_type = 1;
                                GET_TOKEN;
                                break;
                            case STRING:
                                data->return_type = 2;
                                GET_TOKEN;
                                break;
                        }
                    }
                    if (act_token->type == TT_COMMA) {
                        param++; //indexovanie parametrov
                        GET_TOKEN;
                        pom = TT_COMMA;
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
                } while (pom == TT_COMMA);
            }
            else
            {
                PRINT_DEBUG("ERROR def func in params ! \n");
                return ERR_DEF;
            }
        }

    }
}

int statements()
{
    Token* prev_token;
    while (act_token->type != TT_BLOCK_BEGIN && act_token->type != TT_BLOCK_END)
    {
        GET_TOKEN;
        if (act_token->type == TT_BLOCK_BEGIN || act_token->type == TT_BLOCK_END)
        {
            break;
        }

        switch (act_token->attribute.keyword)
        {
            case IF:
                PRINT_DEBUG("Statements IF \n");
                int error_pom = statement(); // pomocna aby sme vedeli kt. error vracat
                if (error_pom != 0)
                {
                    return error_pom;
                }
                if (blockBeginEOL_check() != 0)
                {
                    return ERR_PARSER;
                }
                GET_TOKEN; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! umela uprava !!!!!!
                if (act_token->type == TT_BLOCK_END)
                {
                    if (IfblockEnd_check() != 0)
                    {
                        return ERR_PARSER;
                    }
                    else
                    {
                        if (blockBeginEOL_check() != 0)
                        {
                            return ERR_PARSER;
                        }
                    }
                }
                break;
            case FOR:
                PRINT_DEBUG("Statements FOR \n");
                // for -> init -> ; -> expres -> ; -> assign -> { eol statements... eol }
                if (init(act_token) != 0)
                {
                    return ERR_PARSER;
                }
                GET_TOKEN;
                if (act_token->type == TT_SEMICOLON)
                {
                    PRINT_DEBUG("For 1. semicolon ok \n");
                    GET_TOKEN;
                }
                else
                {
                    return ERR_PARSER;
                }
                // assing teba dorobit neviem ako bude vyzerat value();!!!!!!!!!!!!!!!!


                break;
            case FUNC:
                PRINT_DEBUG("Statements FUNC \n");
                def_func();
                break;
            case PACKAGE:
                PRINT_DEBUG("Statements PACKAGE \n");
                break;
            case RETURN:
                PRINT_DEBUG("Statements RETURN \n");
                break;
            default:
                break;
        }
        if (act_token->type == TT_IDENTIFIER) {
            if (statement() != 0)
            {
                return ERR_DEF;
            }
            /*
            prev_token = act_token; //v pripade ak by som ho v buducnosti potreboval
            GET_TOKEN;
            switch (act_token->type) {
                case TT_IDENTIFIER:
                    return ERR_PARSER; //ID ID x not ok

                case TT_INIT:
                    PRINT_DEBUG("Statements init ! \n");
                    init();
                    break;
                case TT_ASSIGN:
                    PRINT_DEBUG("Statements assign \n");
                    GET_TOKEN;
                    value();
                    break;
                case TT_INTEGER:
                case TT_STRING:
                case TT_DECIMAL:
                    PRINT_DEBUG("Statements int string decimal alias <value>\n");
                    prev_token = act_token;
                    GET_TOKEN;
                    break;
                case TT_COMMA:
                    break;

            }*/
        }
    }
    if (act_token->type == TT_BLOCK_END)
    {
        PRINT_DEBUG("Block end of func ! \n");
        if (IfblockEnd_check() == 0 )
        {
            return ERR_OK;
        }
        else
        {
            return ERR_PARSER;
        }
    }
}
int statement ()
{
    TableData *data = (TableData *) malloc(sizeof(TableData));
    TableItem* temporary_ht;
    Token *prev_token;
    if (act_token->attribute.keyword == IF)
    {
        GET_TOKEN;
        if (act_token->type != TT_IDENTIFIER)
        {
            return ERR_PARSER;
        }
        else
        {
            temporary_ht = htSearch(local_table,act_token->attribute.string->str);
            if(temporary_ht != NULL)
            {
                PRINT_DEBUG("IF ID found in local hash table \n");

                //check if datatypes are same

                switch (temporary_ht->data.type) {

                    case T_INT:
                        if (act_token->type != TT_INTEGER)
                        {
                            return ERR_DEF_TYPE;
                        }
                        break;
                    case T_FLOAT64:

                        if (act_token->type != TT_DECIMAL)
                        {
                            return ERR_DEF_TYPE;
                        }
                        break;
                    case T_STRING:
                        if (act_token->type != TT_STRING)
                        {
                            return ERR_DEF_TYPE;
                        }
                        break;
                    default:
                        return ERR_PARSER;
                }

                return ERR_OK; // zatial work in progress
            }
            else
            {
                PRINT_DEBUG("IF ID not found in local hash table \n");
            }
        }
    }
    if (act_token->type == TT_IDENTIFIER)
    {
        if (htSearch(act_table,act_token->attribute.string->str) != NULL && act_token->attribute.datatype == T_FUNC)
        {
            PRINT_DEBUG("Volanie definovanej funkcie ! \n ");
        }

        prev_token = act_token;
        size_t len = strlen(prev_token->attribute.string->str);
        char* name_of_id = malloc(sizeof(len));
        strcpy(name_of_id,prev_token->attribute.string->str);

        if (htSearch(local_table,act_token->attribute.string->str) == NULL)
        {
            //nebol este inicializovany => next token musi byt :=
            GET_TOKEN;
            if (act_token->type != TT_INIT)
            {
                return ERR_DEF;
            }
            else
            {
                PRINT_DEBUG("INIT \n");
                init(prev_token);

                data->var = true;
                switch (prev_token->attribute.datatype) {

                    case INT:
                        data->type = T_INT;
                        data->value.integer_value = prev_token->attribute.integer;
                        break;
                    case FLOAT64:
                        data->type = T_FLOAT64;
                        data->value.float_value = prev_token->attribute.decimal;
                        break;
                    case STRING:
                        data->type = T_STRING;
                        strcpy(data->value.string_value, prev_token->attribute.string->str);
                        break;
                    default:
                        return ERR_PARSER;
                }
                htInsert(local_table, name_of_id, *data);
            }
        }
        else
        {
            PRINT_DEBUG("ID sa uz nachadza v localhash table ! \n");
            prev_token = act_token;
            GET_TOKEN;
            if (act_token->type == TT_ASSIGN)
            {
                if (assign(prev_token) != 0)
                {
                    return ERR_PARSER;
                }
            }


        }
    }
    if (act_token->type == TT_COMMA) {
        prev_token = act_token; //v pripade ak by som ho v buducnosti potreboval
        int id_counter = 0;
        PRINT_DEBUG("ID, ID, IDn ... \n");
        while (1) {
            if (act_token->type == TT_ASSIGN || act_token->type == TT_INIT) {
                statements();
            }
            GET_TOKEN;
            if (act_token->type != TT_IDENTIFIER) {
                return ERR_PARSER;
            }
            data->var = true;
            htInsert(local_table, act_token->attribute.string->str, *data);
            GET_TOKEN;
            id_counter++;
        }
    }
    return ERR_OK;
}


int blockBeginEOL_check ()
{
    if (act_token->type == TT_R_BRACKET)
    {
        GET_TOKEN;
    }

    if (act_token->type != TT_BLOCK_BEGIN)
    {
        PRINT_DEBUG("Block begin + EOL ERROR ! \n");
        return ERR_PARSER;
    }
    else
    {
        GET_TOKEN;
        if (act_token->type == TT_EOL)
        {
            PRINT_DEBUG("Block begin + EOL OK ! \n");
            return ERR_OK;
        }
    }
}

int IfblockEnd_check()
{
    Token* prev_token = malloc(sizeof(Token));
    if (act_token->type == TT_BLOCK_END)
    {
        prev_token->type = act_token->type;
        GET_TOKEN;
    }
    else
    {
        return ERR_PARSER;
    }

    if (act_token->attribute.keyword == ELSE && prev_token->type == TT_BLOCK_END)
    {
        GET_TOKEN;
        return ERR_OK;
    }
}

int init (Token* left_id) {
    Token *prev_token = initToken();
    if (act_token->attribute.keyword == FOR)
    {
        GET_TOKEN;
        if (act_token->type == TT_IDENTIFIER)
        {
            left_id = act_token;
        }
        else
        {
            return ERR_OK;
        }
        GET_TOKEN;
        if(act_token->type != TT_INIT)
        {
            return ERR_PARSER;
        }
        GET_TOKEN;
    }
    else
    {
        GET_TOKEN;
    }

    /*if (act_token->type == TT_IDENTIFIER)
    {
        prev_token = act_token;
        GET_TOKEN;
        switch (act_token->type)
        {
            case TT_ADD:
            case TT_SUB:
            case TT_MUL:
            case TT_DIV:
                PRINT_DEBUG("Arithmetical operator calling expression analyser !\n");
        }
    }*/
    if (act_token->type == TT_INTEGER) {
        left_id->attribute.datatype = INT;
        left_id->attribute.integer = act_token->attribute.integer;
    }
    if (act_token->type == TT_DECIMAL) {
        left_id->attribute.datatype = FLOAT64;
        left_id->attribute.decimal = act_token->attribute.decimal;
    }
    return ERR_OK;
}

int assign(Token* left_id)
{
    Token *prev_token = initToken();
    if (act_token->type == TT_ASSIGN)
    {
        GetToken(prev_token);
    }
    if (prev_token->type == TT_IDENTIFIER)
    {
        if (act_token->attribute.datatype == T_FUNC)
        {
            PRINT_DEBUG("Assign func \n");
        }
        GET_TOKEN;

        // operator ID

        if (act_token->type >= TT_ADD && act_token->type <= TT_R_BRACKET)
        {
            ActivateResources(local_table);
            expression(prev_token,act_token);
        }
    }
}

int terms ()
{
    PRINT_DEBUG("Func Terms printing \n");
    GET_TOKEN;

    switch (act_token->type) {

        case TT_IDENTIFIER:
            TableItem* data;
            data = htSearch(act_table,act_token->attribute.string->str);
            if (data != NULL)
            {
                if (cg_print_id(data) == false)
                {
                    return ERR_INTERNAL;
                }
            }
            else if ((data = htSearch(local_table,act_token->attribute.string->str) ) != NULL)
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

        default:
            return ERR_PARSER;
    }

    GET_TOKEN;

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





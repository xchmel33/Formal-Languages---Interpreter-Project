#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"
#include "parser.h"
#include "scanner.h"
#include "symtable.h"

#define DEBUG 1

#define PRINT_DEBUG(txt) \
    if (DEBUG) \
        printf(txt)



Token* act_token;
HashTable* act_table;
int depth_level = 0;  //level zanorenia do funkcie
bool func_flag = 0; //flag funkcie 0 - 1
int iteration_count = 0; //pocet iteracii
int error_guard = 0;
unsigned param = 0;



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
    if (token->type == TT_EOF)
    {
        PRINT_DEBUG("End of program");
        return 0;
    }
    if (token->attribute.keyword == PACKAGE)
    {
        GetToken(token);
        if (strcmp(token->attribute.string, "main") == 0)
        {
            PRINT_DEBUG("Package main ok \n ");
            return 0; //Chybaju err stavy
        }

    }
    else
    {
        return 3;
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
        statements();
    }


    if (act_token->type == TT_EOF )
    {
        return 0;
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
        TableItem func;

        func.next_item = NULL;
        size_t len = strlen(act_token->attribute.string);
        func.key = malloc(len * sizeof(char)); //sizeof(char) always 1
        strcpy(func.key,act_token->attribute.string);
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
            }
            else
            {
                return ERR_DEF;
            }
        }
        return 0;
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
            len = strlen(act_token->attribute.string);
            data->param[param].identifier = malloc(sizeof(len * sizeof(char)));
            strcpy(data->param[param].identifier, act_token->attribute.string);
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
        switch (act_token->attribute.keyword)
        {
            case IF:
                PRINT_DEBUG("Statemnts IF \n");
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
                if (init() != 0)
                {
                    return ERR_PARSER;
                }
                GET_TOKEN;
                if (act_token->type == TT_SEMICOLON)
                {
                    PRINT_DEBUG("For 1. semicolon ok");
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
                break;
            case PACKAGE:
                PRINT_DEBUG("Statements PACKAGE \n");
                break;
            case RETURN:
                PRINT_DEBUG("Statements RETURN \n");
                break;
        }
        if (act_token->type == TT_IDENTIFIER) {
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

            }
        }
        if (act_token->type == TT_COMMA) {
            prev_token = act_token; //v pripade ak by som ho v buducnosti potreboval
            int id_counter = 0;
            PRINT_DEBUG("ID, ID, IDn ... \n");
            while (1) {
                if (act_token->type == TT_ASSIGN || act_token->type == TT_INIT)
                {
                    statements();
                }
                GET_TOKEN;
                if (act_token->type != TT_IDENTIFIER) {
                    return ERR_PARSER;
                }
                GET_TOKEN;
                id_counter++;
            }
        }
    }
    return ERR_OK;
}
int statement ()
{
    TableItem* temporary_ht;
    if (act_token->attribute.keyword == IF)
    {
        GET_TOKEN;
        if (act_token->type != TT_IDENTIFIER)
        {
            return ERR_PARSER;
        }
        else
        {
            temporary_ht = htSearch(act_table,act_token->attribute.string);
            if(temporary_ht != NULL)
            {
                PRINT_DEBUG("IF -> FUNC found in global hash table \n");
                GET_TOKEN; //overime ci sedi pocet parametrov
                if (act_token->type == TT_L_BRACKET)
                {
                    //ok zatvorka sedi
                    GET_TOKEN;
                    int counter = 0;
                    while (act_token->type == TT_IDENTIFIER)
                    {
                        counter++;
                        GET_TOKEN;
                        if (act_token->type == TT_R_BRACKET)
                        {
                            break;
                        }
                        if (act_token->type != TT_COMMA)
                        {
                            return ERR_PARSER;
                        }
                        GET_TOKEN;
                    }

                    if (temporary_ht->data.number_params != counter)
                    {
                        PRINT_DEBUG("Nesedi pocet parametrov ! \n ");
                        return ERR_FUNC;
                    }

                }
                return ERR_OK; // zatial work in progress
            }
            else
            {
                PRINT_DEBUG("IF -> FUNC not found in global hash table \n");
            }
        }
    }
}


int blockBeginEOL_check ()
{
    if (act_token->type== TT_R_BRACKET)
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

int init ()
{
    Token* prev_token = malloc(sizeof(Token));
    GET_TOKEN;
    if (act_token->type == TT_IDENTIFIER)
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
    }
    else
    {
        PRINT_DEBUG("error missing ID (func -> init)");
        return ERR_PARSER;
    }
    return ERR_OK; //umela uprava
}

int value()
{

}
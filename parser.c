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

#define GET_TOKEN \
    GetToken(act_token)


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
                if (statements() != 0)
                {
                    PRINT_DEBUG("Statements error \n");
                    return ERR_PARSER;
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
                    else {
                        if (act_token->type == TT_R_BRACKET) {
                            data->type = T_FUNC;
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
    GET_TOKEN;
    if (act_token->type != TT_BLOCK_BEGIN)
    {
        return ERR_PARSER;
    }
    else
    {
        GET_TOKEN;
        return ERR_OK;
    }

}
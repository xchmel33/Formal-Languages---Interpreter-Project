#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"
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

int body()
{
    PRINT_DEBUG("Body start \n");
    GET_TOKEN;
    base_cond(act_token);
    GET_TOKEN;
    if (act_token->type == TT_KEYWORD && (strcmp(act_token->attribute.string, "func") != 0))
    {
        PRINT_DEBUG("Def func \n");
        func_flag = 1;
        depth_level++;
        GET_TOKEN;
        if (act_token->type == TT_IDENTIFIER)
        {
            TableItem func;
            func.next_item = NULL;
            func.key = act_token->attribute.string;
            func.data.type = (DataType)act_token->type;


        }

    }


}
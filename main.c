/**
 * Implementation of imperative language IFJ2020 compiler
 * @file main.c
 * @author Lukáš Chmelo (xchmel33), Matúš Nosko(xnosko06), Tomáš Čechvala(xcechv03)
 * @brief main function implementation
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "parser.h"
#include "errno.h"
#include "error.h"
#include "code_generator.h"

int main() {

    SetSource(stdin);
    Token* token;
    token = initToken();
    HashTable* pTable = (HashTable*)malloc(sizeof(HashTable));
    htInit(pTable);
    set_active_token(token);
    set_active_table(pTable);

    int error = body();
    if (error != 0)
    {
        printf("%d \n",error);
        return error;
    }
    htPrintTable(pTable);
    code_to_stdout();
    free(token);
    htClearAll(pTable);
    return 0;
}

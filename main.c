#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "parser.h"
#include "errno.h"
#include "error.h"
#include "code_generator.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        // argv[0] == interpret
        // argv[1] == filename
        // and nothing else!
        return ERR_INTERNAL;
    }
    FILE* f = fopen(argv[1], "r");
    if (f == NULL) {
        return ERR_INTERNAL;
    }
    SetSource(f);
    Token* token;
    token = initToken();
    HashTable* pTable = (HashTable*)malloc(sizeof(HashTable));
    htInit(pTable);
    set_active_token(token);
    set_active_table(pTable);

    int error = body();
    if (error != 0)
    {
        return error;
    }
    htPrintTable(pTable);
    code_to_stdout();
    fclose(f);
    free(token);
    htClearAll(pTable);
    fclose(f);
    return 0;
}

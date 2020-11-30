#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "parser.h"
#include "errno.h"

int main() {
    FILE* f;
    Token* token = malloc(sizeof(Token));
    f = fopen("code.txt", "r");
    if (f == NULL)
    {
        printf("File opening error \n"); //zmazat neskor !!!!!!!!!!!!!!!!!!!!
        return 0;
    }
    HashTable* pTable = (HashTable*)malloc(sizeof(HashTable));
    htInit(pTable);
    SetSource(f);
    set_active_token(token);
    set_active_table(pTable);
    body();
    htPrintTable(pTable);
    /*while (token->type != TT_EOF) {
        if (GetToken(&*token)) {
            printf("Lexical Error\n");
            fclose(f);
            free(token);
            return 0;
        }
        else {
            switch (token->type)
            {
            case(TT_EOL):
                printf("Token: EOL, %s\n", token->attribute.string);
                break;
            case(TT_EOF):
                printf("Token: EOF, %s\n", token->attribute.string);
                break;
            case(TT_IDENTIFIER):
                printf("Token: IDENTIFIER, %s\n", token->attribute.string);
                break;
            case(TT_KEYWORD):
                printf("Token: KEYWORD, ");
                switch (token->attribute.keyword)
                {
                case(ELSE):
                    printf("else\n");
                    break;
                case(FOR):
                    printf("for\n");
                    break;
                case(FUNC):
                    printf("func\n");
                    break;
                case(IF):
                    printf("if\n");
                    break;
                case(PACKAGE):
                    printf("package\n");
                    break;
                case(RETURN):
                    printf("return\n");
                    break;
                }
                break;
            case(TT_DATATYPE):
                printf("Token: DATATYPE, ");
                switch (token->attribute.datatype)
                {
                case(INT):
                    printf("int\n");
                    break;
                case(FLOAT64):
                    printf("float64\n");
                    break;
                case(STRING):
                    printf("string\n");
                    break;
                }
                break;
            case(TT_INTEGER):
                printf("Token: INTEGER, %d\n", token->attribute.integer);
                break;
            case(TT_DECIMAL):
                printf("Token: DECIMAL, %d\n", token->attribute.decimal);
                break;
            case(TT_STRING):
                printf("Token: STRING, %s\n", token->attribute.string);
                break;
            case(TT_EQUAL):
                printf("Token: EQUAL, %s\n", token->attribute.string);
                break;
            case(TT_NOT_EQUAL):
                printf("Token: NOT EQUAL, %s\n", token->attribute.string);
                break;
            case(TT_LESS_THAN):
                printf("Token: LESS THAN, %s\n", token->attribute.string);
                break;
            case(TT_LESS_OR_EQUAL):
                printf("Token: LESS OR EQUAL, %s\n", token->attribute.string);
                break;
            case(TT_MORE_THAN):
                printf("Token: MORE THAN, %s\n", token->attribute.string);
                break;
            case(TT_MORE_OR_EQUAL):
                printf("Token: MORE OR EQUAL, %s\n", token->attribute.string);
                break;
            case(TT_ASSIGN):
                printf("Token: ASSIGN, %s\n", token->attribute.string);
                break;
            case(TT_INIT):
                printf("Token: INIT, %s\n", token->attribute.string);
                break;
            case(TT_ADD):
                printf("Token: ADD, %s\n", token->attribute.string);
                break;
            case(TT_SUB):
                printf("Token: SUB, %s\n", token->attribute.string);
                break;
            case(TT_MUL):
                printf("Token: MUL, %s\n", token->attribute.string);
                break;
            case(TT_DIV):
                printf("Token: DIV, %s\n", token->attribute.string);
                break;
            case(TT_L_BRACKET):
                printf("Token: LEFT BRACKET, %s\n", token->attribute.string);
                break;
            case(TT_R_BRACKET):
                printf("Token: RIGHT BRACKET, %s\n", token->attribute.string);
                break;
            case(TT_BLOCK_BEGIN):
                printf("Token: BLOCK BEGIN, %s\n", token->attribute.string);
                break;
            case(TT_BLOCK_END):
                printf("Token: BLOCK END, %s\n", token->attribute.string);
                break;
            case(TT_COMMA):
                printf("Token: COMMA, %s\n", token->attribute.string);
                break;
            case(TT_SEMICOLON):
                printf("Token: SEMICOLON, %s\n", token->attribute.string);
                break;
            }
        }
    }*/
    fclose(f);
    free(token);
    htClearAll(pTable);
    return 0;
}

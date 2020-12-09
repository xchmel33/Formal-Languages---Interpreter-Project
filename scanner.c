#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "dstring.h"

FILE* source;

void procces_id_key_data(dstring *S_Attribute, Token* token) {

    token->type = TT_KEYWORD;
    if (!strcmp(S_Attribute->str, "else")) token->attribute.keyword = ELSE;
    else if (!strcmp(S_Attribute->str, "for")) token->attribute.keyword = FOR;
    else if (!strcmp(S_Attribute->str, "func")) token->attribute.keyword = FUNC;
    else if (!strcmp(S_Attribute->str, "if")) token->attribute.keyword = IF;
    else if (!strcmp(S_Attribute->str, "package")) token->attribute.keyword = PACKAGE;
    else if (!strcmp(S_Attribute->str, "return"))  token->attribute.keyword = RETURN;
    else if (!strcmp(S_Attribute->str, "int")) {
        token->type = TT_DATATYPE;
        token->attribute.datatype = INT;
    }
    else if (!strcmp(S_Attribute, "float64")) {
        token->type = TT_DATATYPE;
        token->attribute.datatype = FLOAT64;
    }
    else if (!strcmp(S_Attribute, "string")) {
        token->type = TT_DATATYPE;
        token->attribute.datatype = STRING;
    }
    else {
        token->type = TT_IDENTIFIER;
        strCopyString(token->attribute.string, S_Attribute);
    }
}

void procces_decimal(dstring* S_Attribute, Token* token) {

    char* end;
    double value = strtod(S_Attribute->str, &end);
    if (*end) {
        return;
    }
    token->type = TT_DECIMAL;
    token->attribute.decimal = value;
}

void SetSource(FILE* f) {
    source = f;
}
Token* initToken() {

    Token* token = malloc(sizeof(Token));
    token->attribute.string = malloc(sizeof(dstring));
    strInit(token->attribute.string);
    token->attribute.datatype = UNDEFINED_TOKEN_ATTRIBUTE;
    token->attribute.keyword = UNDEFINED_TOKEN_ATTRIBUTE;
    token->attribute.integer = UNDEFINED_TOKEN_ATTRIBUTE;
    token->attribute.decimal = UNDEFINED_TOKEN_ATTRIBUTE;
    return token;
}

int GetToken(Token* token) {

    token->attribute.datatype = UNDEFINED_TOKEN_ATTRIBUTE;
    token->attribute.keyword = UNDEFINED_TOKEN_ATTRIBUTE;
    token->attribute.integer = UNDEFINED_TOKEN_ATTRIBUTE;
    token->attribute.decimal = UNDEFINED_TOKEN_ATTRIBUTE;

    token->type = TT_EMPTY;
    strClear(token->attribute.string);
    ScannerState state = SS_START;
    char c = { 0 };
    dstring* S_Attribute = malloc(sizeof(dstring));
    strInit(S_Attribute);
    char strhex[2];

    while (state != SS_FINISHED) {
        c = getc(source);
        switch (state)
        {
        case(SS_START):
            strClear(S_Attribute);
                if (c == '\n' || c == '\r') {
                    token->type = TT_EOL;
                    strAddString(token->attribute.string,"NULL");
                    state = SS_FINISHED;
                }
                else if (isdigit(c)) {
                    strAddChar(S_Attribute,c);
                    state = SS_NUMBER;
                }
                else if (isspace(c) || c == '\t') {
                    state = SS_START;
                }
                else if (isalpha(c) || c == '_') {
                    strAddChar(S_Attribute,c);
                    state = SS_ID_KEY_DATA;
                }
                else if (c == EOF) {
                    token->type = TT_EOF;
                    strAddString(token->attribute.string,"NULL");
                    state = SS_FINISHED;
                }
                else if (c == ',') {
                    token->type = TT_COMMA;
                    strAddChar(token->attribute.string,c);
                    state = SS_FINISHED;
                }
                else if (c == ';') {
                    token->type = TT_SEMICOLON;
                    strAddChar(token->attribute.string,c);
                    state = SS_FINISHED;
                }
                else if (c == '(') {
                    token->type = TT_L_BRACKET;
                    strAddChar(token->attribute.string,c);
                    state = SS_FINISHED;
                }
                else if (c == ')') {
                    token->type = TT_R_BRACKET;
                    strAddChar(token->attribute.string,c);
                    state = SS_FINISHED;
                }
                else if (c == '{') {
                    token->type = TT_BLOCK_BEGIN;
                    strAddChar(token->attribute.string,c);
                    state = SS_FINISHED;
                }
                else if (c == '}') {
                    token->type = TT_BLOCK_END;
                    strAddChar(token->attribute.string,c);
                    state = SS_FINISHED;
                }
                else if (c == '+') {
                    token->type = TT_ADD;
                    strAddChar(token->attribute.string,c);
                    state = SS_FINISHED;
                }
                else if (c == '-') {
                    token->type = TT_SUB;
                    strAddChar(token->attribute.string,c);
                    state = SS_FINISHED;
                }
                else if (c == '*') {
                    token->type = TT_MUL;
                    strAddChar(token->attribute.string,c);
                    state = SS_FINISHED;
                }
                else if (c == '/') {
                    state = SS_BACKSLASH;
                }
                else if (c == '<') {
                    strAddChar(S_Attribute,c);
                    state = SS_LESS_THAN;
                }
                else if (c == '>') {
                    strAddChar(S_Attribute,c);
                    state = SS_MORE_THAN;
                }
                else if (c == '=') {
                    strAddChar(S_Attribute,c);
                    state = SS_EQUAL;
                }
                else if (c == '!') {
                    strAddChar(S_Attribute,c);
                    state = SS_NOT;
                }
                else if (c == ':') {
                    strAddChar(S_Attribute,c);
                    state = SS_INIT;
                }
                else if (c == '"') {
                    //strAddChar(S_Attribute,c);
                    state = SS_STRING;
                }
                else {
                    state = SS_ERROR;
                }
                break;
            case(SS_STRING):
                if (c == '"') {
                    //strAddChar(S_Attribute,c);
                    token->type = TT_STRING;
                    strCopyString(token->attribute.string,S_Attribute);
                    state = SS_FINISHED;
                }
                else if (c < 32) {
                    ungetc(c, source);
                    state = SS_ERROR;
                }
                else if (c == '\\') {
                    state = SS_ESCAPE_SEQUENCE;
                }
                else {
                    strAddChar(S_Attribute,c);
                }
                break;
            case(SS_ESCAPE_SEQUENCE):
                if (c == 'n') {
                    c = '\n';
                    strAddChar(S_Attribute,c);
                    state = SS_STRING;
                }
                else if (c == 't') {
                    c = '\t';
                    strAddChar(S_Attribute,c);
                    state = SS_STRING;
                }
                else if (c == 'x') {
                    state = SS_ESCAPE_SEQUENCE_HEX;
                }
                else if (c == '"') {
                    c = '\"';
                    strAddChar(S_Attribute,c);
                    state = SS_STRING;
                }
                else if (c == '\\') {
                    strAddChar(S_Attribute,c);
                    state = SS_STRING;
                }
                break;
            case(SS_ESCAPE_SEQUENCE_HEX):
                if ((isdigit(c)) || (tolower(c) >= 97 && tolower(c) <= 102)) {
                    strhex[0] = c;
                    state = SS_ESCAPE_SEQUENCE_HEX_2;
                }
                else {
                    state = SS_ERROR;
                }
                break;
            case(SS_ESCAPE_SEQUENCE_HEX_2):
                if ((isdigit(c)) || (tolower(c) >= 97 && tolower(c) <= 102)) {
                    strhex[1] = c;
                    c = (char)strtol(strhex, NULL, 16);
                    strAddChar(S_Attribute,c);
                    state = SS_STRING;
                }
                else {
                    state = SS_ERROR;
                }
                break;
            case(SS_INIT):
                if (c == '=') {
                    strAddChar(S_Attribute,c);
                    token->type = TT_INIT;
                    strCopyString(token->attribute.string, S_Attribute);
                    state = SS_FINISHED;
                }
                else {
                    state = SS_ERROR;
                }
                break;
            case(SS_LESS_THAN):
                if (c == '=') {
                    strAddChar(S_Attribute,c);
                    token->type = TT_LESS_OR_EQUAL;
                    strCopyString(token->attribute.string, S_Attribute);
                    state = SS_FINISHED;
                }
                else {
                    ungetc(c, source);
                    token->type = TT_LESS_THAN;
                    strCopyString(token->attribute.string, S_Attribute);
                    state = SS_FINISHED;
                }
                break;
            case(SS_MORE_THAN):
                if (c == '=') {
                    strAddChar(S_Attribute,c);
                    token->type = TT_MORE_OR_EQUAL;
                    strCopyString(token->attribute.string, S_Attribute);
                    state = SS_FINISHED;
                }
                else {
                    ungetc(c, source);
                    token->type = TT_MORE_THAN;
                    strCopyString(token->attribute.string, S_Attribute);
                    state = SS_FINISHED;
                }
                break;
            case(SS_NOT):
                if (c == '=') {
                    strAddChar(S_Attribute,c);
                    token->type = TT_NOT_EQUAL;
                    strCopyString(token->attribute.string, S_Attribute);
                    state = SS_FINISHED;
                }
                else {
                    state = SS_ERROR;
                }
                break;
            case(SS_EQUAL):
                if (c == '=') {
                    strAddChar(S_Attribute,c);
                    token->type = TT_EQUAL;
                    strCopyString(token->attribute.string, S_Attribute);
                    state = SS_FINISHED;
                }
                else {
                    ungetc(c, source);
                    token->type = TT_ASSIGN;
                    strCopyString(token->attribute.string, S_Attribute);
                    state = SS_FINISHED;
                }
                break;
            case(SS_BACKSLASH):
                if (c == '/') {
                    state = SS_LINE_COMMENTARY;
                }
                else if (c == '*') {
                    state = SS_BLOCK_COMMENTARY;
                }
                else {
                    ungetc(c, source);
                    strAddChar(S_Attribute, '/');
                    token->type = TT_DIV;
                    strCopyString(token->attribute.string, S_Attribute);
                    state = SS_FINISHED;
                }
                break;
            case(SS_LINE_COMMENTARY):
                if (c == '\n' || c == '\r') {
                    token->type = TT_EOL;
                    strAddString(token->attribute.string,"NULL");
                    state = SS_FINISHED;
                }
                else if (c == EOF) {
                    token->type = TT_EOF;
                    strAddString(token->attribute.string, "NULL");
                    state = SS_FINISHED;
                }
                break;
            case(SS_BLOCK_COMMENTARY):
                strcpy(S_Attribute, "");
                while (strcmp(S_Attribute, "*/") != 0) {
                    if (c == '*' || c == '/') {
                        strAddChar(S_Attribute,c);
                    }
                    c = getc(source);
                }
                state = SS_START;
                break;
            case(SS_ID_KEY_DATA):
                if ((isalpha(c) || isdigit(c) || c == '_')) {
                    strAddChar(S_Attribute,c);
                }
                else {
                    ungetc(c, source);
                    procces_id_key_data(S_Attribute, token);
                    state = SS_FINISHED;
                }
                break;
            case(SS_NUMBER):
                if (isdigit(c)) {
                    strAddChar(S_Attribute,c);
                }
                else if (c == '.') {
                    strAddChar(S_Attribute,c);
                    state = SS_NUMBER_DECIMAL;
                }
                else if (tolower(c) == 'e') {
                    strAddChar(S_Attribute,c);
                    state = SS_NUMBER_EXPONENT;
                }
                else {
                    ungetc(c, source);
                    token->type = TT_INTEGER;
                    token->attribute.integer = atoi(S_Attribute->str);
                    state = SS_FINISHED;
                }
                break;
            case(SS_NUMBER_DECIMAL):
                if (isdigit(c)) {
                    strAddChar(S_Attribute,c);
                }
                else if (tolower(c) == 'e') {
                    strAddChar(S_Attribute,c);
                    state = SS_NUMBER_EXPONENT;
                }
                else {
                    ungetc(c, source);
                    procces_decimal(S_Attribute, token);
                    state = SS_FINISHED;
                }
                break;
            case(SS_NUMBER_EXPONENT):
                if (isdigit(c)) {
                    strAddChar(S_Attribute,c);
                }
                else if (c == '-') {
                    strAddChar(S_Attribute, c);
                    state = SS_NUMBER_EXPONENT_SIGN;
                }
                else if (c == '+') {
                    state = SS_NUMBER_EXPONENT_SIGN;
                }
                else {
                    ungetc(c, source);
                    procces_decimal(S_Attribute, token);
                    state = SS_FINISHED;
                }
                break;
            case(SS_NUMBER_EXPONENT_SIGN):
                if (isdigit(c)) {
                    strAddChar(S_Attribute, c);
                }
                else {
                    ungetc(c, source);
                    procces_decimal(S_Attribute, token);
                    state = SS_FINISHED;
                }
                break;
            case(SS_ERROR):
                return 1;
                break;
        }
    }
    return token->type == TT_EMPTY ? -1 : 0;

}

void printToken(Token* token) {

    switch (token->type)
    {
        case(TT_EOL):
            printf("Token: EOL, %s\n", token->attribute.string->str);
            break;
        case(TT_EOF):
            printf("Token: EOF, %s\n", token->attribute.string->str);
            break;
        case(TT_IDENTIFIER):
            printf("Token: IDENTIFIER, %s\n", token->attribute.string->str);
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
            printf("Token: DECIMAL, %f\n", token->attribute.decimal);
            break;
        case(TT_STRING):
            printf("Token: STRING, %s\n", token->attribute.string->str);
            break;
        case(TT_EQUAL):
            printf("Token: EQUAL, %s\n", token->attribute.string->str);
            break;
        case(TT_NOT_EQUAL):
            printf("Token: NOT EQUAL, %s\n", token->attribute.string->str);
            break;
        case(TT_LESS_THAN):
            printf("Token: LESS THAN, %s\n", token->attribute.string->str);
            break;
        case(TT_LESS_OR_EQUAL):
            printf("Token: LESS OR EQUAL, %s\n", token->attribute.string->str);
            break;
        case(TT_MORE_THAN):
            printf("Token: MORE THAN, %s\n", token->attribute.string->str);
            break;
        case(TT_MORE_OR_EQUAL):
            printf("Token: MORE OR EQUAL, %s\n", token->attribute.string->str);
            break;
        case(TT_ASSIGN):
            printf("Token: ASSIGN, %s\n", token->attribute.string->str);
            break;
        case(TT_INIT):
            printf("Token: INIT, %s\n", token->attribute.string->str);
            break;
        case(TT_ADD):
            printf("Token: ADD, %s\n", token->attribute.string->str);
            break;
        case(TT_SUB):
            printf("Token: SUB, %s\n", token->attribute.string->str);
            break;
        case(TT_MUL):
            printf("Token: MUL, %s\n", token->attribute.string->str);
            break;
        case(TT_DIV):
            printf("Token: DIV, %s\n", token->attribute.string->str);
            break;
        case(TT_L_BRACKET):
            printf("Token: LEFT BRACKET, %s\n", token->attribute.string->str);
            break;
        case(TT_R_BRACKET):
            printf("Token: RIGHT BRACKET, %s\n", token->attribute.string->str);
            break;
        case(TT_BLOCK_BEGIN):
            printf("Token: BLOCK BEGIN, %s\n", token->attribute.string->str);
            break;
        case(TT_BLOCK_END):
            printf("Token: BLOCK END, %s\n", token->attribute.string->str);
            break;
        case(TT_COMMA):
            printf("Token: COMMA, %s\n", token->attribute.string->str);
            break;
        case(TT_SEMICOLON):
            printf("Token: SEMICOLON, %s\n", token->attribute.string->str);
            break;
        case(TT_TABLESYM):
            printf("Token: TABLESYM, %s\n", token->attribute.string->str);
            break;
        case(TT_EMPTY):
            printf("Token: EMPTY, %s\n", token->attribute.string->str);
            break;
        case(TT_EXPRESSION):
            if (token->attribute.integer != UNDEFINED_TOKEN_ATTRIBUTE) {
                printf("Token: EXPRESSION, %d\n", token->attribute.integer);
            }
            else if (token->attribute.decimal != UNDEFINED_TOKEN_ATTRIBUTE) {
                printf("Token: EXPRESSION, %f\n", token->attribute.decimal);
            }
            else if (token->attribute.string->length != 0) {
                printf("Token: EXPRESSION, %s\n", token->attribute.string->str);
            }
            else if (token->attribute.datatype != UNDEFINED_TOKEN_ATTRIBUTE) {
                printf("Token: EXPRESSION,");
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
            }
            else if (token->attribute.keyword != UNDEFINED_TOKEN_ATTRIBUTE) {
                printf("Token: EXPRESSION,");
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
            }
            break;
    }
}
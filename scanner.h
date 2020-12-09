/**
 * Implementation of imperative language IFJ2020 compiler
 * @file scanner.h
 * @author Lukáš Chmelo (xchmel33), Tomáš Čechvala (xcechv03)
 * @brief Scanner header implementation
 */


#ifndef SCANNER_H
#define SCANNER_H
#include "dstring.h"
#define UNDEFINED_TOKEN_ATTRIBUTE -999999
typedef enum {
    ELSE,
    FOR,
    FUNC,
    IF,
    PACKAGE,
    RETURN,
}Keyword;

typedef enum {
    INT,
    FLOAT64,
    STRING,
    IDENTIFIER,
}Datatype;

typedef enum {
    //term
    TT_ADD,
    TT_SUB,
    TT_MUL,
    TT_DIV,
    TT_LESS_THAN,
    TT_MORE_THAN,
    TT_EQUAL,
    TT_MORE_OR_EQUAL,
    TT_LESS_OR_EQUAL,
    TT_NOT_EQUAL,
    TT_L_BRACKET,
    TT_R_BRACKET,
    TT_IDENTIFIER,
    TT_INTEGER,
    TT_DECIMAL,
    TT_STRING,
    TT_EMPTY,

    //other
    TT_TABLESYM,
    TT_EOL,
    TT_EOF,
    TT_KEYWORD,
    TT_DATATYPE,
    TT_ASSIGN,
    TT_INIT,
    TT_BLOCK_BEGIN,
    TT_BLOCK_END,
    TT_COMMA,
    TT_SEMICOLON,
    TT_EXPRESSION,
}TokenType;

typedef struct {
    Keyword keyword;
    Datatype datatype;
    int integer;
    double decimal;
    dstring* string;
}TokenAttribute;

typedef struct {
    TokenType type;
    TokenAttribute attribute;
}Token;

typedef enum {
    SS_START,
    SS_FINISHED,
    SS_ID_KEY_DATA,
    SS_NUMBER,
    SS_NUMBER_DECIMAL,
    SS_NUMBER_EXPONENT,
    SS_NUMBER_EXPONENT_SIGN,
    SS_BACKSLASH,
    SS_LINE_COMMENTARY,
    SS_BLOCK_COMMENTARY,
    SS_EQUAL,
    SS_NOT,
    SS_ERROR,
    SS_MORE_THAN,
    SS_LESS_THAN,
    SS_INIT,
    SS_STRING,
    SS_ESCAPE_SEQUENCE,
    SS_ESCAPE_SEQUENCE_HEX,
    SS_ESCAPE_SEQUENCE_HEX_2,
}ScannerState;

void procces_id_key_data(dstring* S_Attribute, Token* token);
void procces_decimal(dstring* S_Attribute, Token* token);
void SetSource(FILE* f);
int GetToken(Token* token);
Token* initToken();
void printToken(Token* token);

#endif //IFJ_FINAL_SCANNER_H
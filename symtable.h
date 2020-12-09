
#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "scanner.h"

typedef enum {
    T_FUNC,
    T_INT,
    T_FLOAT64,
    T_STRING,
    T_NONE,
}DataType;


typedef struct {
    Datatype type;
    char* identifier;
}Param;

typedef union {
    int integer_value;
    char* string_value;
    float  float_value;
}Value;

typedef struct {
    DataType type; //func/var
    bool defined;
    Value value; //na uchovanie hodnoty
    bool var; // true -> contains variable !
    DataType return_type;
    int number_params;
    Param param[100]; //Define max index ex. max number of arg !
}TableData;

typedef struct TableItem {
    char* key;
    TableData data;
    struct TableItem* next_item;
} TableItem;





typedef TableItem* HashTable[100];

int hashCode(char* key);

void htPrintTable(HashTable* ptrht);

void htInit(HashTable* htable);

TableItem* htSearch(HashTable* htable, char* key);

void htInsert(HashTable* htable, char* key, TableData data);

TableData* htRead(HashTable* htable, char* key);

void htDelete(HashTable* htable, char* key);

void htClearAll(HashTable* htable);

#endif

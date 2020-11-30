#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symtable.h"

TableItem* UNDEFPTR;

void htPrintTable( HashTable* ptrht)
{
    int maxlen = 0;
    int sumcnt = 0;

    printf("------------HASH TABLE--------------\n");
    for ( int i=0; i<100; i++ ) {
        printf ("%i:",i);
        int cnt = 0;
        TableItem* ptr = (*ptrht)[i];
        while ( ptr != NULL ) {
            printf(" (%s,%d)",ptr->key,ptr->data.type);
            if ( ptr != UNDEFPTR )
                cnt++;
            ptr = ptr->next_item;
        }
        printf ("\n");

        if (cnt > maxlen)
            maxlen = cnt;
        sumcnt+=cnt;
    }

    printf("------------------------------------\n");
    printf("Items count %i   The longest list %i\n",sumcnt,maxlen);
    printf("------------------------------------\n");
}


int hashCode(char* key) {

    int retval = 1;
    int keylen = strlen(key);
    for (int i = 0; i < keylen; i++)
        retval += key[i];
    return (retval % 100);
}

void htInit(HashTable* htable) {

    for (int i = 0; i < 100; i++) {
        (*htable)[i] = NULL;
    }
}

TableItem* htSearch(HashTable* htable, char* key) {

    TableItem* X = (*htable)[hashCode(key)];
    while (X) {
        if (strcmp(X->key, key) == 0) {
            return X;
        }
        X = X->next_item;
    }
    return NULL;
}

void htInsert(HashTable* htable, char* key, TableData data) {

    TableItem* X = htSearch(htable, key);
    if (X) {
        X->data = data;
    }
    else {
        int i = hashCode(key);
        TableItem* Y = (*htable)[i];

        X = malloc(sizeof(struct TableItem));
        if (!X) {
            return;
        }
        X->data = data;
        X->key = key;
        X->next_item = NULL;
        (*htable)[i] = X;
        X->next_item = Y;
    }
}

TableData* htRead(HashTable* htable, char* key) {

    TableItem* X = htSearch(htable, key);
    return (X != NULL) ? &X->data : NULL;
}

void htDelete(HashTable* htable, char* key) {

    int i = hashCode(key);
    TableItem* X = (*htable)[i];
    if (X == NULL) {
        return;
    }
    TableItem* Y = NULL;

    while (X != NULL)
    {
        if (strcmp(key, X->key) == 0) {
            if (Y == NULL) {
                (*htable)[i] = X->next_item;
            }
            else {
                Y->next_item = X->next_item;
            }
            free(X);
            return;
        }
        Y = X;
        X = X->next_item;
    }
}

void htClearAll(HashTable* htable) {

    for (int i = 0; i < 100; i++)
    {
        TableItem* X = (*htable)[i];
        TableItem* Y = NULL;
        while (X) {
            Y = X->next_item;
            free(X);
            X = Y;
        }
        (*htable)[i] = NULL;
        X = NULL;
    }
}

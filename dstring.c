/**
 * Implementation of imperative language IFJ2020 compiler
 * @file dstring.c
 * @author Tomáš Čechvala (xcechv03)
 * @brief Dynamic string implementation
 */

#define _CRT_SECURE_NO_WARNINGS     //VisualStudio requirement

#include "dstring.h"
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#define STR_LEN_INC 16             // amount of bits that we will use for allocation of memory
#define STR_FALSE   0
#define STR_TRUE 1

int strInit(dstring* s) {
//initialize dynamic string

    if (!(s->str = (char *) malloc(STR_LEN_INC)))
    {
        return STR_FALSE;
    }

    strClear(s);
    s->allocSize = STR_LEN_INC;

    return STR_TRUE;
}

void strFree(dstring* s) {
//free allocated memory

    free(s->str);

}

void strClear(dstring* s) {
//clear content of the dynamic string
    s->length = 0;
    s->str[0] = '\0';

}

int strAddChar(dstring* s1, char c) {
//add char at the end of the dynamic string

    if (s1->length + 1 >= s1->allocSize)
    {
        s1->str = (char*)realloc(s1->str, s1->length + STR_LEN_INC);
        if (s1->str == NULL)
            return STR_FALSE;
        s1->allocSize = s1->length + STR_LEN_INC;
    }
    s1->str[s1->length] = c;
    s1->length++;
    s1->str[s1->length] = '\0';
    return STR_TRUE;
}
int strAddString(dstring* s1, const char *s2){
//add constant string at the end of the dynamic string

    unsigned int len2 = (unsigned int) strlen(s2);

    if (s1->length + len2 + 1 >= s1->allocSize)
    {
        s1->str = (char*)realloc(s1->str, s1->length + len2 + 1);
        if ((s1->str == NULL))
            return STR_FALSE;
        s1->allocSize = s1->length + len2 + 1;
    }
    s1->length += len2;
    strcat(s1->str, s2);
    s1->str[s1->length] = '\0';
    return STR_TRUE;

}

int strCopyString(dstring* s1, dstring* s2) {
//copy string s2 into string s1

    int length2 = s2->length;
    if (length2 >= s1->allocSize)
    {
        
        if ((s1->str = (char*)realloc(s1->str, length2 + 1)) == NULL)
            return STR_FALSE;
        s1->allocSize = length2 + 1;
    }
        strcpy(s1->str, s2->str);
        s1->length = length2;
        return STR_TRUE;
    }

int strCmpString(dstring* s1, dstring* s2) {
//compare both dynamic strings
    return strcmp(s1->str, s2->str);
}

int strCmpConstStr(dstring* s1, char* s2) {
//compare dynamic string with a constant string
     return strcmp(s1->str, s2);
}

char* strGetStr(dstring* s) {
//retun text of the dynamic string
    return s->str;
}

int strGetLength(dstring* s) {
//return length of the dynamic string
    return s->length;

}

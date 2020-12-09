#define _CRT_SECURE_NO_WARNINGS

#include "dstring.h"
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#define STR_LEN_INC 16
#define STR_FALSE   0
#define STR_TRUE 1

int strInit(dstring* s) {

    if (!(s->str = (char *) malloc(STR_LEN_INC)))
    {
        return STR_FALSE;
    }

    strClear(s);
    s->allocSize = STR_LEN_INC;

    return STR_TRUE;
}

void strFree(dstring* s) {

    free(s->str);

}

void strClear(dstring* s) {

    s->length = 0;
    s->str[0] = '\0';

}

int strAddChar(dstring* s1, char c) {
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

    return strcmp(s1->str, s2->str);
}

int strCmpConstStr(dstring* s1, char* s2) {

     return strcmp(s1->str, s2);
}

char* strGetStr(dstring* s) {

    return s->str;
}

int strGetLength(dstring* s) {

    return s->length;

}

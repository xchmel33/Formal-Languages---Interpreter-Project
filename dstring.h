/**
 * Implementation of imperative language IFJ2020 compiler
 * @file dstring.h
 * @author Tomáš Čechvala (xcechv03)
 * @brief Dynamic string implementation
 */

#ifndef IFJ_FINAL_DSTRING_H
#define IFJ_FINAL_DSTRING_H


#define MAX_CHARACTERS (101)

typedef struct
{
    char* str;		// string ended by '\0' byte
    int length;		// length of dynamic string
    int allocSize;	// amount of chars allocated for dynamic string
} dstring;


int strInit(dstring* s);
void strFree(dstring* s);

void strClear(dstring* s);
int strAddChar(dstring* s1, char c);
int strCopyString(dstring* s1, dstring* s2);
int strCmpString(dstring* s1, dstring* s2);
int strCmpConstStr(dstring* s1, char* s2);

int strAddString(dstring* s1, const char *s2);


char* strGetStr(dstring* s);
int strGetLength(dstring* s);


#endif //IFJ_FINAL_DSTRING_H
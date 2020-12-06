
#ifndef IFJ_FINAL_DSTRING_H
#define IFJ_FINAL_DSTRING_H


#define MAX_CHARACTERS (101)

typedef struct
{
    char* str;		// misto pro dany retezec ukonceny znakem '\0'
    int length;		// skutecna delka retezce
    int allocSize;	// velikost alokovane pameti
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
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
}Datatype;

typedef enum {
	TEOL,
	TEOF,
	IDENTIFIER,
	KEYWORD,
	DATATYPE,

	INTEGER,
	DECIMAL,
	TSTRING,

	EQUAL,
	NOT_EQUAL,
	LESS_THAN,
	LESS_EQUAL,
	MORETHAN,
	MORE_EQUAL,

	ASSIGN,
	INIT,
	ADD,
	SUB,
	MUL,
	DIV,

	L_BRACKET,
	R_BRACKET,
	BLOCK_BEGIN,
	BLOC_KEND,
	COMMA,
	SEMICOLON,
	LINE_COMMENTARY,
	BLOCK_COMMENTARY,
}TokenType;

typedef union {
	char identifier[50];
	Keyword keyword;
	Datatype datatype;
	int integer;
	double decimal;
	char string[50];
}TokenAttribute;

typedef struct {
	TokenType tokentype;
	TokenAttribute attribute;
}Token;

typedef enum {
	START,
	FINISHED,
	IDENTIFIER_KEYWORD,
	NUMBER,
	NUMBER_DECIMAL,
	NUMBER_EXPONENT,
}ScannerState;


int main(void){
	
	FILE* source;
	Token *token;
	source = fopen("code.txt", "r");
	ScannerState state = START;
	char c = { 0 };
	char Sattribute[10] = "";

	while (state != FINISHED) {
		c = getc(source);
		switch (state)
		{
		case(START):
			if (isalpha(c)) {
				strncat(Sattribute, &c, 1);
				state = IDENTIFIER_KEYWORD;
			}
			else if (isdigit(c)) {
				strncat(Sattribute, &c, 1);
				state = NUMBER;
			}
			else if (isspace(c)) {
				state = START;
			}
			else if (c == '\n') {
				state = FINISHED;
			}
			break;
		case(IDENTIFIER_KEYWORD):
			if ((isalpha(c) || isdigit(c) || c == '_')) {
				strncat(Sattribute, &c, 1);
			}
			else {  
				state = FINISHED;
			}
			break;
		case(NUMBER):
			if (isdigit(c)) {
				strncat(Sattribute, &c, 1);
			}
			else if (c == '.') {
				strncat(Sattribute, &c, 1);
				state = NUMBER_DECIMAL;
			}
			else if (c == 'e') {
				strncat(Sattribute, &c, 1);
				state = NUMBER_EXPONENT;
			}
			else {
				state = FINISHED;
			}
			break;
		case(NUMBER_DECIMAL):
			if (isdigit(c)) {
				strncat(Sattribute, &c, 1);
			}
			else if (c == 'e') {
				strncat(Sattribute, &c, 1);
				state = NUMBER_EXPONENT;
			}
			else {
				state = FINISHED;
			}
			break;
		case(NUMBER_EXPONENT):
			if (isdigit(c) || c == '+' || c == '-') {
				strncat(Sattribute, &c, 1);
			}
			else {
				state = FINISHED;
			}
			break;
		}
		

	}
	printf("%s", Sattribute);
	fclose(source);
	return 0;
}
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
	BLOCK_END,
	COMMA,
	SEMICOLON,
}TokenType;

typedef union {
	char identifier[50];
	Keyword keyword;
	Datatype datatype;
	int integer;
	double decimal;
	char string[50];
	char operator[2];
	char other;
}TokenAttribute;

typedef struct {
	TokenType type;
	TokenAttribute attribute;
}Token;

typedef enum {
	START,
	FINISHED,
	ID_KEY_DATA,
	NUMBER,
	NUMBER_DECIMAL,
	NUMBER_EXPONENT,
	BACKSLASH,
	LINE_COMMENTARY,
	BLOCK_COMMENTARY,
}ScannerState;

void procces_id_key_data(char S_Attribute[10], Token* token) {

	token->type = KEYWORD;
	if (!strcmp(S_Attribute, "else")) token->attribute.keyword = ELSE;
	else if (!strcmp(S_Attribute, "for")) token->attribute.keyword = FOR;
	else if (!strcmp(S_Attribute, "func")) token->attribute.keyword = FUNC;
	else if (!strcmp(S_Attribute, "if")) token->attribute.keyword = IF;
	else if (!strcmp(S_Attribute, "package")) token->attribute.keyword = PACKAGE;
	else if (!strcmp(S_Attribute, "return"))  token->attribute.keyword = RETURN;
	else if (!strcmp(S_Attribute, "int")) {
		token->type = DATATYPE;
		token->attribute.datatype = INT;
	}
	else if (!strcmp(S_Attribute, "float64")) {
		token->type = DATATYPE;
		token->attribute.datatype = FLOAT64;
	}
	else if (!strcmp(S_Attribute, "string")) {
		token->type = DATATYPE;
		token->attribute.datatype = STRING;
	}
	else {
		token->type = IDENTIFIER;
		strcpy(token->attribute.identifier, S_Attribute);
	}
}

void procces_decimal(char S_Attribute[10], Token* token) {

	char* end;
	double value = strtod(S_Attribute, &end);
	if (*end) {
		return;
	}
	token->type = DECIMAL;
	token->attribute.decimal = value;
}

int main(void) {

	FILE* source;
	Token* token = malloc(sizeof(Token));
	source = fopen("code.txt", "r");
	ScannerState state = START;
	char c = { 0 };
	char S_Attribute[10] = "";

	while (state != FINISHED) {
		c = getc(source);
		switch (state)
		{
		case(START):
			strcpy(S_Attribute, "");
			if (isalpha(c)) {
				strncat(S_Attribute, &c, 1);
				state = ID_KEY_DATA;
			}
			else if (isdigit(c)) {
				strncat(S_Attribute, &c, 1);
				state = NUMBER;
			}
			else if (isspace(c)) {
				state = START;
			}
			else if (c == '\n') {
				token->type = TEOL;
				token->attribute.other = NULL;
				state = FINISHED;
			}
			else if (c == EOF) {
				token->type = TEOF;
				token->attribute.other = NULL;
				state = FINISHED;
			}
			else if (c == ',') {
				token->type = COMMA;
				token->attribute.other = c;
				state = FINISHED;
			}
			else if (c == ';') {
				token->type = SEMICOLON;
				token->attribute.other = c;
				state = FINISHED;
			}
			else if (c == '(') {
				token->type = L_BRACKET;
				token->attribute.other = c;
				state = FINISHED;
			}
			else if (c == ')') {
				token->type = R_BRACKET;
				token->attribute.other = c;
				state = FINISHED;
			}
			else if (c == '{') {
				token->type = BLOCK_BEGIN;
				token->attribute.other = c;
				state = FINISHED;
			}
			else if (c == '}') {
				token->type = BLOCK_END;
				token->attribute.other = c;
				state = FINISHED;
			}
			else if (c == '+') {
				token->type = ADD;
				token->attribute.other = c;
				state = FINISHED;
			}
			else if (c == '-') {
				token->type = SUB;
				token->attribute.other = c;
				state = FINISHED;
			}
			else if (c == '*') {
				token->type = MUL;
				token->attribute.other = c;
				state = FINISHED;
			}
			else if (c == '/') {
				state = BACKSLASH;
			}
			break;
		case(BACKSLASH):
			if (c == '/') {
				state = LINE_COMMENTARY;
			}
			else if (c == '*') {
				state = BLOCK_COMMENTARY;
			}
			else {
				ungetc(c, source);
				token->type = DIV;
				token->attribute.other = '/';
				state = FINISHED;
			}
			break;
		case(LINE_COMMENTARY):
			if (c == '\n' || c == EOF) {
				state = START;
			}
			break;
		case(BLOCK_COMMENTARY):
			strcpy(S_Attribute, "");
			while (strcmp(S_Attribute, "*/") != 0) {
				if (c == '*' || c == '/') {
					strncat(S_Attribute, &c, 1);
				}
				c = getc(source);
			}
			state = START;
			break;
		case(ID_KEY_DATA):
			if ((isalpha(c) || isdigit(c) || c == '_')) {
				strncat(S_Attribute, &c, 1);
			}
			else {
				procces_id_key_data(S_Attribute, token);
				state = FINISHED;
			}
			break;
		case(NUMBER):
			if (isdigit(c)) {
				strncat(S_Attribute, &c, 1);
			}
			else if (c == '.') {
				strncat(S_Attribute, &c, 1);
				state = NUMBER_DECIMAL;
			}
			else if (tolower(c) == 'e') {
				strncat(S_Attribute, &c, 1);
				state = NUMBER_EXPONENT;
			}
			else {
				token->type = INTEGER;
				token->attribute.integer = atoi(S_Attribute);
				state = FINISHED;
			}
			break;
		case(NUMBER_DECIMAL):
			if (isdigit(c)) {
				strncat(S_Attribute, &c, 1);
			}
			else if (tolower(c) == 'e') {
				strncat(S_Attribute, &c, 1);
				state = NUMBER_EXPONENT;
			}
			else {
				procces_decimal(S_Attribute, token);
				state = FINISHED;
			}
			break;
		case(NUMBER_EXPONENT):
			if (isdigit(c) || c == '+' || c == '-') {
				strncat(S_Attribute, &c, 1);
			}
			else {
				procces_decimal(S_Attribute, token);
				state = FINISHED;
			}
			break;
		}


	}

	printf("Read: %s\n", S_Attribute);
	fclose(source);
	free(token);
	return 0;
}
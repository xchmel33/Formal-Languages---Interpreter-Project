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
	TT_EMPTY,
	TT_EOL,
	TT_EOF,
	TT_IDENTIFIER,
	TT_KEYWORD,
	TT_DATATYPE,

	TT_INTEGER,
	TT_DECIMAL,
	TT_STRING,

	TT_EQUAL,
	TT_NOT_EQUAL,
	TT_LESS_THAN,
	TT_LESS_OR_EQUAL,
	TT_MORE_THAN,
	TT_MORE_OR_EQUAL,

	TT_ASSIGN,
	TT_INIT,
	TT_ADD,
	TT_SUB,
	TT_MUL,
	TT_DIV,

	TT_L_BRACKET,
	TT_R_BRACKET,
	TT_BLOCK_BEGIN,
	TT_BLOCK_END,
	TT_COMMA,
	TT_SEMICOLON,
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
	SS_START,
	SS_FINISHED,
	SS_ID_KEY_DATA,
	SS_NUMBER,
	SS_NUMBER_DECIMAL,
	SS_NUMBER_EXPONENT,
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

void procces_id_key_data(char S_Attribute[10], Token* token);
void procces_decimal(char S_Attribute[10], Token* token);
void SetSource(FILE* f);
int GetToken(Token* token);

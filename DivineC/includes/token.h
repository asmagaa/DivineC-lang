// tokens
#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_LET, TOKEN_THERE, TOKEN_BE, TOKEN_OF, TOKEN_BLESSED,
    TOKEN_FUNCTION, TOKEN_IF, TOKEN_ELSE, TOKEN_FOR, TOKEN_WHILE,
    TOKEN_SACRED, TOKEN_DIVINE, TOKEN_PRAY, TOKEN_FORGIVE,
    TOKEN_INT, TOKEN_FLOAT, TOKEN_STRING_TYPE, TOKEN_BOOL,
    TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_STRING, TOKEN_TRUE, TOKEN_FALSE,
    TOKEN_ASSIGN, TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE,
    TOKEN_EQUAL, TOKEN_NOT_EQUAL, TOKEN_LESS, TOKEN_GREATER,
    TOKEN_LESS_EQUAL, TOKEN_GREATER_EQUAL, TOKEN_AND, TOKEN_OR, TOKEN_NOT,
    TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_RETURN, TOKEN_PRINT,
    TOKEN_EOF, TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char *value;
    int line;
    int column;
} Token;

Token* token_create(TokenType type, char *value, int line, int column);
void token_free(Token *token);

#endif
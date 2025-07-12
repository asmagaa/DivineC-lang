// lexer
#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    char *source;
    int position;
    int line;
    int column;
    int length;
} Lexer;

Lexer* lexer_init(char *source);
void lexer_free(Lexer *lexer);
char lexer_current_char(Lexer *lexer);
void lexer_advance(Lexer *lexer);
void lexer_skip_whitespace(Lexer *lexer);
Token* lexer_get_next_token(Lexer *lexer);

TokenType get_keyword_type(char *word);
Token* lexer_read_identifier(Lexer *lexer);
Token* lexer_read_number(Lexer *lexer);
Token* lexer_read_string(Lexer *lexer);

#endif
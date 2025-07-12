// tokens
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token.h"

Token* token_create(TokenType type, char *value, int line, int column) {
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->type = value ? strdup(value) : NULL;
    token->line = line;
    token->column = column;
    return token;
}

void token_free(Token *token) {
    if (token) {
        if (token->value) {
            free(token->value);
        }
        free(token);
    }
}
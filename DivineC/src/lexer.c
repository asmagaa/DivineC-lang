// lexer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexer.h"

Lexer* lexer_init(char *source) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(source);
    return lexer;
}

void lexer_free(Lexer *lexer) {
    if (lexer) {
        free(lexer);
    }
}

char lexer_current_char(Lexer *lexer) {
    if (lexer->position >= lexer->length) return '\0';
    return lexer->source[lexer->position];
}

void lexer_advance(Lexer *lexer) {
    if (lexer->position < lexer->length) {
        if (lexer->source[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

void lexer_skip_whitespace(Lexer *lexer) {
    while (isspace(lexer_current_char(lexer))) {
        lexer_advance(lexer);
    }
}

TokenType get_keyword_type(char *word) {
    if (strcmp(word, "let") == 0) return TOKEN_LET;
    if (strcmp(word, "there") == 0) return TOKEN_THERE;
    if (strcmp(word, "be") == 0) return TOKEN_BE;
    if (strcmp(word, "of") == 0) return TOKEN_OF;
    if (strcmp(word, "blessed") == 0) return TOKEN_BLESSED;
    if (strcmp(word, "function") == 0) return TOKEN_FUNCTION;
    if (strcmp(word, "if") == 0) return TOKEN_IF;
    if (strcmp(word, "else") == 0) return TOKEN_ELSE;
    if (strcmp(word, "for") == 0) return TOKEN_FOR;
    if (strcmp(word, "while") == 0) return TOKEN_WHILE;
    if (strcmp(word, "sacred") == 0) return TOKEN_SACRED;
    if (strcmp(word, "divine") == 0) return TOKEN_DIVINE;
    if (strcmp(word, "pray") == 0) return TOKEN_PRAY;
    if (strcmp(word, "forgive") == 0) return TOKEN_FORGIVE;
    if (strcmp(word, "int") == 0) return TOKEN_INT;
    if (strcmp(word, "float") == 0) return TOKEN_FLOAT;
    if (strcmp(word, "string") == 0) return TOKEN_STRING_TYPE;
    if (strcmp(word, "bool") == 0) return TOKEN_BOOL;
    if (strcmp(word, "true") == 0) return TOKEN_TRUE;
    if (strcmp(word, "false") == 0) return TOKEN_FALSE;
    if (strcmp(word, "return") == 0) return TOKEN_RETURN;
    if (strcmp(word, "print") == 0) return TOKEN_PRINT;
    return TOKEN_IDENTIFIER;
}

Token* lexer_read_identifier(Lexer *lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    int start_pos = lexer->position;
    
    while (isalnum(lexer_current_char(lexer)) || lexer_current_char(lexer) == '_') {
        lexer_advance(lexer);
    }
    
    int length = lexer->position - start_pos;
    char *word = malloc(length + 1);
    strncpy(word, lexer->source + start_pos, length);
    word[length] = '\0';
    
    TokenType type = get_keyword_type(word);
    Token *token = token_create(type, word, start_line, start_column);
    free(word);
    return token;
}

Token* lexer_read_number(Lexer *lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    int start_pos = lexer->position;
    
    while (isdigit(lexer_current_char(lexer)) || lexer_current_char(lexer) == '.') {
        lexer_advance(lexer);
    }
    
    int length = lexer->position - start_pos;
    char *number = malloc(length + 1);
    strncpy(number, lexer->source + start_pos, length);
    number[length] = '\0';
    
    Token *token = token_create(TOKEN_NUMBER, number, start_line, start_column);
    free(number);
    return token;
}

Token* lexer_read_string(Lexer *lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    
    lexer_advance(lexer);
    int start_pos = lexer->position;
    
    while (lexer_current_char(lexer) != '"' && lexer_current_char(lexer) != '\0') {
        lexer_advance(lexer);
    }
    
    if (lexer_current_char(lexer) == '\0') {
        return NULL;
    }
    
    int length = lexer->position - start_pos;
    char *string = malloc(length + 1);
    strncpy(string, lexer->source + start_pos, length);
    string[length] = '\0';
    
    lexer_advance(lexer);
    
    Token *token = token_create(TOKEN_STRING, string, start_line, start_column);
    free(string);
    return token;
}

Token* lexer_get_next_token(Lexer *lexer) {
    lexer_skip_whitespace(lexer);
    
    if (lexer->position >= lexer->length) {
        return token_create(TOKEN_EOF, NULL, lexer->line, lexer->column);
    }
    
    char current = lexer_current_char(lexer);
    int line = lexer->line;
    int column = lexer->column;
    
    if (isalpha(current) || current == '_') {
        return lexer_read_identifier(lexer);
    }
    
    if (isdigit(current)) {
        return lexer_read_number(lexer);
    }
    
    if (current == '"') {
        return lexer_read_string(lexer);
    }
    
    switch (current) {
        case '=':
            lexer_advance(lexer);
            if (lexer_current_char(lexer) == '=') {
                lexer_advance(lexer);
                return token_create(TOKEN_EQUAL, "==", line, column);
            }
            return token_create(TOKEN_ASSIGN, "=", line, column);
        case '+':
            lexer_advance(lexer);
            return token_create(TOKEN_PLUS, "+", line, column);
        case '-':
            lexer_advance(lexer);
            return token_create(TOKEN_MINUS, "-", line, column);
        case '*':
            lexer_advance(lexer);
            return token_create(TOKEN_MULTIPLY, "*", line, column);
        case '/':
            lexer_advance(lexer);
            return token_create(TOKEN_DIVIDE, "/", line, column);
        case '!':
            lexer_advance(lexer);
            if (lexer_current_char(lexer) == '=') {
                lexer_advance(lexer);
                return token_create(TOKEN_NOT_EQUAL, "!=", line, column);
            }
            return token_create(TOKEN_NOT, "!", line, column);
        case '<':
            lexer_advance(lexer);
            if (lexer_current_char(lexer) == '=') {
                lexer_advance(lexer);
                return token_create(TOKEN_LESS_EQUAL, "<=", line, column);
            }
            return token_create(TOKEN_LESS, "<", line, column);
        case '>':
            lexer_advance(lexer);
            if (lexer_current_char(lexer) == '=') {
                lexer_advance(lexer);
                return token_create(TOKEN_GREATER_EQUAL, ">=", line, column);
            }
            return token_create(TOKEN_GREATER, ">", line, column);
        case '&':
            lexer_advance(lexer);
            if (lexer_current_char(lexer) == '&') {
                lexer_advance(lexer);
                return token_create(TOKEN_AND, "&&", line, column);
            }
            break;
        case '|':
            lexer_advance(lexer);
            if (lexer_current_char(lexer) == '|') {
                lexer_advance(lexer);
                return token_create(TOKEN_OR, "||", line, column);
            }
            break;
        case '(':
            lexer_advance(lexer);
            return token_create(TOKEN_LPAREN, "(", line, column);
        case ')':
            lexer_advance(lexer);
            return token_create(TOKEN_RPAREN, ")", line, column);
        case '{':
            lexer_advance(lexer);
            return token_create(TOKEN_LBRACE, "{", line, column);
        case '}':
            lexer_advance(lexer);
            return token_create(TOKEN_RBRACE, "}", line, column);
        case ';':
            lexer_advance(lexer);
            return token_create(TOKEN_SEMICOLON, ";", line, column);
        case ',':
            lexer_advance(lexer);
            return token_create(TOKEN_COMMA, ",", line, column);
    }
    
    char unknown[2] = {current, '\0'};
    lexer_advance(lexer);
    return token_create(TOKEN_UNKNOWN, unknown, line, column);
}
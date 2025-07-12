// parser
#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "lexer.h"
#include "ast.h"

typedef struct {
    Token *current_token;
    Lexer *lexer;
} Parser;

Parser* parser_init(Lexer *lexer);
void parser_free(Parser *parser);
void parser_advance(Parser *parser);
int parser_match(Parser *parser, TokenType type);
void parser_consume(Parser *parser, TokenType type);

ASTNode* parse_program(Parser *parser);
ASTNode* parse_statement(Parser *parser);
ASTNode* parse_expression(Parser *parser);
ASTNode* parse_primary(Parser *parser);
ASTNode* parse_variable_declaration(Parser *parser);
ASTNode* parse_print_statement(Parser *parser);

#endif
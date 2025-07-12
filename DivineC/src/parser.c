// parser
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"

Parser* parser_init(Lexer *lexer) {
    Parser *parser = malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    return parser;
}

void parser_free(Parser *parser) {
    if (parser) {
        if (parser->current_token) {
            token_free(parser->current_token);
        }
        free(parser);
    }
}

void parser_advance(Parser *parser) {
    if (parser->current_token) {
        token_free(parser->current_token);
    }
    parser->current_token = lexer_get_next_token(parse->lexer);
}

int parser_match(Parser *parser, TokenType type) {
    return parser->current_token->type == type;
}

void parser_consume(Parser *parser, TokenType type) {
    if (parser->current_token->type == type) {
        parser_advance(parser);
    }
}

ASTNode* parse_primary(Parser *parser) {
    ASTNode *node = NULL;
    
    if (parser_match(parser, TOKEN_NUMBER)) {
        node = ast_node_create(AST_NUMBER);
        node->data.number_value = atof(parser->current_token->value);
        parser_advance(parser);
    } else if (parser_match(parser, TOKEN_STRING)) {
        node = ast_node_create(AST_STRING);
        node->data.string_value = strdup(parser->current_token->value);
        parser_advance(parser);
    } else if (parser_match(parser, TOKEN_TRUE)) {
        node = ast_node_create(AST_BOOLEAN);
        node->data.boolean_value = 1;
        parser_advance(parser);
    } else if (parser_match(parser, TOKEN_FALSE)) {
        node = ast_node_create(AST_BOOLEAN);
        node->data.boolean_value = 0;
        parser_advance(parser);
    } else if (parser_match(parser, TOKEN_IDENTIFIER)) {
        node = ast_node_create(AST_IDENTIFIER);
        node->data.string_value = strdup(parser->current_token->value);
        parser_advance(parser);
    } else if (parser_match(parser, TOKEN_LPAREN)) {
        parser_advance(parser);
        node = parse_expression(parser);
        parser_consume(parser, TOKEN_RPAREN);
    }
    
    return node;
}

ASTNode* parse_expression(Parser *parser) {
    ASTNode *left = parse_primary(parser);
    
    while (parser_match(parser, TOKEN_PLUS) || parser_match(parser, TOKEN_MINUS) ||
    parser_match(parser, TOKEN_MULTIPLY) || parser_match(parser, TOKEN_DIVIDE) ||
    parser_match(parser, TOKEN_EQUAL) || parser_match(parser, TOKEN_NOT_EQUAL) ||
    parser_match(parser, TOKEN_LESS) || parser_match(parser, TOKEN_GREATER) ||
    parser_match(parser, TOKEN_LESS_EQUAL) || parser_match(parser, TOKEN_GREATER_EQUAL)) {
        
        ASTNode *binary_op = ast_node_create(AST_BINARY_OP);
        binary_op->data.binary_op.operator = strdup(parser->current_token->value);
        binary_op->data.binary_op.left = left;
        parser_advance(parser);
        binary_op->data.binary_op.right = parse_primary(parser);
        left = binary_op;
    }
    
    return left;
}

ASTNode* parse_variable_declaration(Parser *parser) {
    parser_consume(parser, TOKEN_LET);
    parser_consume(parser, TOKEN_THERE);
    parser_consume(parser, TOKEN_BE);
    
    ASTNode *node = ast_node_create(AST_VARIABLE_DECL);
    node->data.variable_decl.data_type = strdup(parser->current_token->value);
    parser_advance(parser);
    
    parser_consume(parser, TOKEN_OF);
    
    node->data.variable_decl.name = strdup(parser->current_token->value);
    parser_advance(parser);
    
    parser_consume(parser, TOKEN_ASSIGN);
    node->data.variable_decl.value = parse_expression(parser);
    
    return node;
}

ASTNode* parse_print_statement(Parser *parser) {
    parser_consume(parser, TOKEN_PRINT);
    
    ASTNode *node = ast_node_create(AST_PRINT_STMT);
    node->data.print_stmt.expression = parse_expression(parser);
    
    return node;
}

ASTNode* parse_statement(Parser *parser) {
    if (parser_match(parser, TOKEN_LET)) {
        return parse_variable_declaration(parser);
    } else if (parser_match(parser, TOKEN_PRINT)) {
        return parse_print_statement(parser);
    } else {
        return parse_expression(parser);
    }
}

ASTNode* parse_program(Parser *parser) {
    ASTNode *program = ast_node_create(AST_PROGRAM);
    program->data.program.statements = malloc(sizeof(ASTNode*) * 100);
    program->data.program.statement_count = 0;
    
    while (!parser_match(parser, TOKEN_EOF)) {
        ASTNode *stmt = parse_statement(parser);
        if (stmt) {
            program->data.program.statements[program->data.program.statement_count++] = stmt;
        }
        parser_consume(parser, TOKEN_SEMICOLON);
    }
    
    return program;
}
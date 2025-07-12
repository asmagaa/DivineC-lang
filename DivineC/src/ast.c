// ast definitions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ast.h"

ASTNode* ast_node_create(ASTNodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    memset(&node->data, 0, sizeof(node->data));
    return node;
}

void ast_node_free(ASTNode *node) {
    if (!node) return;
    switch (node->type) {
        case AST_PROGRAM:
            if (node->data.program.statements) {
                for (int i = 0; i < node->data.program.statement_count; i++) {
                    ast_node_free(node->data.program.statements[i]);
                }
                free(node->data.program.statements);
            }
            break;
        case AST_VARIABLE_DECL:
            if (node->data.variable_decl.data_type) free(node->data.variable_decl.data_type);
            if (node->data.variable_decl.name) free(node->data.variable_decl.name);
            ast_node_free(node->data.variable_decl.value);
            break;
        case AST_FUNCTION_DECL:
            if (node->data.function_decl.name) free(node->data.function_decl.name);
            if (node->data.function_decl.params) {
                for (int i = 0; i < node->data.function_decl.param_count; i++) {
                    free(node->data.function_decl.params[i]);
                }
                free(node->data.function_decl.params);
            }
            ast_node_free(node->data.function_decl.body);
            break;
        case AST_IF_STMT:
            ast_node_free(node->data.if_stmt.condition);
            ast_node_free(node->data.if_stmt.then_stmt);
            ast_node_free(node->data.if_stmt.else_stmt);
            break;
        case AST_WHILE_STMT:
            ast_node_free(node->data.while_stmt.condition);
            ast_node_free(node->data.while_stmt.body);
            break;
        case AST_FOR_STMT:
            ast_node_free(node->data.for_stmt.init);
            ast_node_free(node->data.for_stmt.condition);
            ast_node_free(node->data.for_stmt.update);
            ast_node_free(node->data.for_stmt.body);
            break;
        case AST_RETURN_STMT:
            ast_node_free(node->data.return_stmt.expression);
            break;
        case AST_PRINT_STMT:
            ast_node_free(node->data.print_stmt.expression);
            break;
        case AST_BLOCK:
            if (node->data.block.statements) {
                for (int i = 0; i < node->data.block.statement_count; i++) {
                    ast_node_free(node->data.block.statements[i]);
                }
                free(node->data.block.statements);
            }
            break;
        case AST_BINARY_OP:
            if (node->data.binary_op.operator) free(node->data.binary_op.operator);
            ast_node_free(node->data.binary_op.left);
            ast_node_free(node->data.binary_op.right);
            break;
        case AST_UNARY_OP:
            if (node->data.unary_op.operator) free(node->data.unary_op.operator);
            ast_node_free(node->data.unary_op.operand);
            break;
        case AST_FUNCTION_CALL:
            if (node->data.function_call.name) free(node->data.function_call.name);
            if (node->data.function_call.args) {
                for (int i = 0; i < node->data.function_call.arg_count; i++) {
                    ast_node_free(node->data.function_call.args[i]);
                }
                free(node->data.function_call.args);
            }
            break;
        case AST_ASSIGNMENT:
            if (node->data.assignment.name) free(node->data.assignment.name);
            ast_node_free(node->data.assignment.value);
            break;
        case AST_IDENTIFIER:
        case AST_STRING:
            if (node->data.string_value) free(node->data.string_value);
            break;
        default:
            break;
    }
    free(node);
}
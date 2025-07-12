// ast definitions
#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,
    AST_VARIABLE_DECL,
    AST_FUNCTION_DECL,
    AST_IF_STMT,
    AST_WHILE_STMT,
    AST_FOR_STMT,
    AST_RETURN_STMT,
    AST_PRINT_STMT,
    AST_EXPRESSION_STMT,
    AST_BLOCK,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_FUNCTION_CALL,
    AST_IDENTIFIER,
    AST_NUMBER,
    AST_STRING,
    AST_BOOLEAN,
    AST_ASSIGNMENT,
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        struct {
            struct ASTNode **statements;
            int statement_count;
        } program;

        struct {
            char *data_type;
            char *name;
            struct ASTNode *value;
        } variable_decl;

        struct {
            char *name;
            char **params;
            int param_count;
            struct ASTNode *body;
        } function_decl;

        struct {
            struct ASTNode *condition;
            struct ASTNode *then_stmt;
            struct ASTNode *else_stmt;
        } if_stmt;

        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_stmt;

        struct {
            struct ASTNode *init;
            struct ASTNode *condition;
            struct ASTNode *update;
            struct ASTNode *body;
        } for_stmt;

        struct {
            struct ASTNode *expression;
        } return_stmt;

        struct {
            struct ASTNode *expression;
        } print_stmt;

        struct {
            struct ASTNode **statements;
            int statement_count;
        } block;

        struct {
            char *operator;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;

        struct {
            char *operator;
            struct ASTNode *operand;
        } unary_op;

        struct {
            char *name;
            struct ASTNode **args;
            int arg_count;
        } function_call;

        struct {
            char *name;
            struct ASTNode *value;
        } assignment;

        char *string_value;
        double number_value;
        int boolean_value;
    } data;
} ASTNode;

ASTNode* ast_node_create(ASTNodeType type);
void ast_node_free(ASTNode *node);

#endif
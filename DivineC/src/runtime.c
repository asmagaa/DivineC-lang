// runtime
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/runtime.h"

SymbolTable* symbol_table_create(void) {
    SymbolTable *table = malloc(sizeof(SymbolTable));
    table->symbols = NULL;
    return table;
}

void symbol_table_free(SymbolTable *table) {
    if (!table) return;
    
    Symbol *current = table->symbols;
    while (current) {
        Symbol *next = current->next;
        if (current->name) free(current->name);
        value_free(&current->value);
        free(current);
        current = next;
    }
    free(table);
}

void symbol_table_set(SymbolTable *table, char *name, Value value) {
    Symbol *symbol = malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->value = value;
    symbol->next = table->symbols;
    table->symbols = symbol;
}

Value* symbol_table_get(SymbolTable *table, char *name) {
    Symbol *current = table->symbols;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return &current->value;
        }
        current = current->next;
    }
    return NULL;
}

Interpreter* interpreter_init(void) {
    Interpreter *interpreter = malloc(sizeof(Interpreter));
    interpreter->global_table = symbol_table_create();
    interpreter->local_table = NULL;
    interpreter->error_occurred = 0;
    memset(interpreter->error_message, 0, sizeof(interpreter->error_message));
    return interpreter;
}

void interpreter_free(Interpreter *interpreter) {
    if (interpreter) {
        symbol_table_free(interpreter->global_table);
        if (interpreter->local_table) {
            symbol_table_free(interpreter->local_table);
        }
        free(interpreter);
    }
}

Value value_create_int(int val) {
    Value value;
    value.type = VALUE_INT;
    value.data.int_val = val;
    return value;
}

Value value_create_float(double val) {
    Value value;
    value.type = VALUE_FLOAT;
    value.data.float_val = val;
    return value;
}

Value value_create_string(char *val) {
    Value value;
    value.type = VALUE_STRING;
    value.data.string_val = strdup(val);
    return value;
}

Value value_create_bool(int val) {
    Value value;
    value.type = VALUE_BOOL;
    value.data.bool_val = val;
    return value;
}

void value_free(Value *value) {
    if (value && value->type == VALUE_STRING && value->data.string_val) {
        free(value->data.string_val);
        value->data.string_val = NULL;
    }
}

void print_value(Value value) {
    switch (value.type) {
        case VALUE_INT:
            printf("%d", value.data.int_val);
            break;
        case VALUE_FLOAT:
            printf("%.2f", value.data.float_val);
            break;
        case VALUE_STRING:
            printf("%s", value.data.string_val);
            break;
        case VALUE_BOOL:
            printf("%s", value.data.bool_val ? "true" : "false");
            break;
        case VALUE_NULL:
            printf("null");
            break;
    }
}

Value evaluate_expression(Interpreter *interpreter, ASTNode *node) {
    Value result;
    result.type = VALUE_NULL;
    
    if (!node) return result;
    
    switch (node->type) {
        case AST_NUMBER: {
            double val = node->data.number_value;
            if (val == (int)val) {
                result = value_create_int((int)val);
            } else {
                result = value_create_float(val);
            }
            break;
        }
        case AST_STRING:
            result = value_create_string(node->data.string_value);
            break;
        case AST_BOOLEAN:
            result = value_create_bool(node->data.boolean_value);
            break;
        case AST_IDENTIFIER: {
            Value *val = symbol_table_get(interpreter->global_table, node->data.string_value);
            if (val) {
                result = *val;
            }
            break;
        }
        case AST_BINARY_OP: {
            Value left = evaluate_expression(interpreter, node->data.binary_op.left);
            Value right = evaluate_expression(interpreter, node->data.binary_op.right);
            
            if (strcmp(node->data.binary_op.operator, "+") == 0) {
                if (left.type == VALUE_INT && right.type == VALUE_INT) {
                    result = value_create_int(left.data.int_val + right.data.int_val);
                } else if (left.type == VALUE_FLOAT || right.type == VALUE_FLOAT) {
                    double l = (left.type == VALUE_INT) ? left.data.int_val : left.data.float_val;
                    double r = (right.type == VALUE_INT) ? right.data.int_val : right.data.float_val;
                    result = value_create_float(l + r);
                }
            } else if (strcmp(node->data.binary_op.operator, "-") == 0) {
                if (left.type == VALUE_INT && right.type == VALUE_INT) {
                    result = value_create_int(left.data.int_val - right.data.int_val);
                } else if (left.type == VALUE_FLOAT || right.type == VALUE_FLOAT) {
                    double l = (left.type == VALUE_INT) ? left.data.int_val : left.data.float_val;
                    double r = (right.type == VALUE_INT) ? right.data.int_val : right.data.float_val;
                    result = value_create_float(l - r);
                }
            } else if (strcmp(node->data.binary_op.operator, "*") == 0) {
                if (left.type == VALUE_INT && right.type == VALUE_INT) {
                    result = value_create_int(left.data.int_val * right.data.int_val);
                } else if (left.type == VALUE_FLOAT || right.type == VALUE_FLOAT) {
                    double l = (left.type == VALUE_INT) ? left.data.int_val : left.data.float_val;
                    double r = (right.type == VALUE_INT) ? right.data.int_val : right.data.float_val;
                    result = value_create_float(l * r);
                }
            } else if (strcmp(node->data.binary_op.operator, "/") == 0) {
                double l = (left.type == VALUE_INT) ? left.data.int_val : left.data.float_val;
                double r = (right.type == VALUE_INT) ? right.data.int_val : right.data.float_val;
                if (r != 0) {
                    result = value_create_float(l / r);
                }
            }
            break;
        }
    }
    
    return result;
}

void execute_statement(Interpreter *interpreter, ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_VARIABLE_DECL: {
            Value value = evaluate_expression(interpreter, node->data.variable_decl.value);
            symbol_table_set(interpreter->global_table, node->data.variable_decl.name, value);
            break;
        }
        case AST_PRINT_STMT: {
            Value value = evaluate_expression(interpreter, node->data.print_stmt.expression);
            print_value(value);
            printf("\n");
            break;
        }
        case AST_PROGRAM: {
            for (int i = 0; i < node->data.program.statement_count; i++) {
                execute_statement(interpreter, node->data.program.statements[i]);
            }
            break;
        }
    }
}
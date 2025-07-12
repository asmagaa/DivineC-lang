// runtime
#ifndef RUNTIME_H
#define RUNTIME_H

#include "ast.h"

typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_STRING,
    VALUE_BOOL,
    VALUE_NULL
} ValueType;

typedef struct {
    ValueType type;
    union {
        int int_val;
        double float_val;
        char *string_val;
        int bool_var;
    } data;
} Value;

typedef struct Symbol {
    char *name;
    Value value;
    struct Symbol *next;
} Symbol;

typedef struct {
    Symbol *symbols;
} SymbolTable;

typedef struct {
    SymbolTable *global_table;
    SymbolTable *local_table;
    int error_occurerd;
    char error_message[256];
} Interpreter;

Interpreter* interpreter_init(void);
void interpreter_free(Interpreter *interpreter);

SymbolTable* symbol_table_create(void);
void symbol_table_free(SymbolTable *table);
void symbol_table_set(SymbolTable *table, char *name, Value value);
Value* symbol_table_get(SymbolTable *table, char *name);

Value value_create_int(int val);
Value value_create_float(double val);
Value value_create_string(char *val);
Value value_create_bool(int val);
void value_free(Value *value);
void print_value(Value value);

Value evaluate_expression(Interpreter *interpreter, ASTNode *node);
void execute_statement(Interpreter *interpreter, ASTNode *node);

#endif
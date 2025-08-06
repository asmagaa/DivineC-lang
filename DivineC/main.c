/* DivineC - How it was made/how it works:

token system (token.h/c):
- Defines all language tokens with divine/religious keywords instead of traditional ones, examples: "let there be" for variable declaration, "blessed" for function, "divine" for conditionals.
- Contains token types enum and token structure with type, value, line/column info.
- This is the vocabulary of our language - every meaningful word becomes a token.

lexical analysis (lexer.h/c):
- Takes raw source code as input and converts it into a stream of tokens.
- Scans character by character, recognizing keywords, identifiers, numbers, operators.
- Handles whitespace, comments, and tracks line/column positions for error reporting.
- Output: sequence of tokens that the parser can understand.

syntax tree (ast.h/c):
- Defines the tree structure that represents the program's hierarchical syntax.
- Each node type represents different language constructs (variables, functions, expressions, etc.).
- Contains all AST node types and structures for building/manipulating the tree.
- This is the "meaning" of the program in a structured, executable form.

parser (parser.h/c):
- Implements recursive descent parsing using the language's grammar rules.
- Takes tokens from lexer and builds an Abstract Syntax Tree (AST).
- Handles operator precedence, statement parsing, expression parsing.
- Validates syntax and reports parsing errors with meaningful messages.

interpreter / more like a runtime (runtime.h/c):
- Executes the AST by traversing it and performing the actual computations.
- Manages variable storage, function calls, control flow (if/while/for).
- Handles different data types (int, float, string, bool) and their operations.
- Contains the execution environment and implements all language semantics.

flow:
Source Code → Lexer (tokenization) → Parser (AST building) → Runtime (execution)

The main.c file orchestrates this entire process:
- Reads source file into memory,
- Creates lexer with source code,
- Creates parser with lexer,
- Parses program into AST,
- Creates interpreter and executes the AST,
- Cleans up all allocated memory.

I hope no one died after reading the code, if someone did, you are in heaven now, blessed by my divine code :).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/token.h"
#include "includes/lexer.h"
#include "includes/ast.h"
#include "includes/parser.h"
#include "includes/runtime.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = malloc(file_size + 1);
    fread(source, 1, file_size, file);
    source[file_size] = '\0';
    fclose(file);

    printf("---DivineC Interpreter---\n");
    printf("Running: %s\n\n", argv[1]);

    Lexer *lexer = lexer_init(source);
    Parser *parser = parser_init(lexer);
    ASTNode *ast = parse_program(parser);

    Interpreter *interpreter = interpreter_init();
    execute_statement(interpreter, ast);

    printf("\n---Execution Complete---\n");

    ast_node_free(ast);
    parser_free(parser);
    lexer_free(lexer);
    interpreter_free(interpreter);
    free(source);

    return 0;
}
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
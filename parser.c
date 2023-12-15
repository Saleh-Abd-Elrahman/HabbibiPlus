#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

// Token and Token types definitions are assumed to be included from your lexer.h

Token *tokens; // Assuming this is a global variable storing the tokenized input
int currentTokenIndex = 0; // Global variable to track the current token being processed

// Forward declaration of the parsing function
void expression();

// Function to match the expected token type and move to the next token
void consume(TokenType expectedType) {
    if (tokens[currentTokenIndex].type == expectedType) {
        currentTokenIndex++;
    } else {
        fprintf(stderr, "Error: Unexpected token\n");
        exit(EXIT_FAILURE);
    }
}

// Function to parse an integer
int parseInteger() {
    if (tokens[currentTokenIndex].type == TOKEN_INT) {
        int value = tokens[currentTokenIndex].intValue;
        consume(TOKEN_INT);
        return value;
    } else {
        fprintf(stderr, "Error: Expected integer\n");
        exit(EXIT_FAILURE);
    }
}

// Function to parse an expression
void expression() {
    int result = parseInteger();

    while (tokens[currentTokenIndex].type == TOKEN_PLUS || tokens[currentTokenIndex].type == TOKEN_MINUS) {
        if (tokens[currentTokenIndex].type == TOKEN_PLUS) {
            consume(TOKEN_PLUS);
            result += parseInteger();
        } else {
            consume(TOKEN_MINUS);
            result -= parseInteger();
        }
    }

    printf("Result: %d\n", result);
}

int main() {
    // Assuming you have tokenized input stored in 'tokens'
    // Call the parser to start parsing
    expression();

    if (tokens[currentTokenIndex].type != TOKEN_EOF) {
        fprintf(stderr, "Error: Unprocessed tokens remain\n");
        exit(EXIT_FAILURE);
    }

    printf("Parsing successful\n");
    return 0;
}

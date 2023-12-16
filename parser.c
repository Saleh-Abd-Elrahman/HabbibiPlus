#include "lexer.h"
#include "lexer.c"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

Token *tokens;
int currentTokenIndex = 0;
Token currentToken;

void nextToken() {
    currentToken = tokens[currentTokenIndex++];
}

void parseError(const char* message) {
    fprintf(stderr, "Parse error: %s\n", message);
    fprintf(stderr, "Unhandled token type: %d\n", currentToken.type);
    exit(EXIT_FAILURE);
}

void expect(TokenType expectedType) {
    if (currentToken.type == expectedType) {
        nextToken();
    } else {
        parseError("Unexpected token");
        
    }
}

int parseInteger() {
    int value = currentToken.intValue; // Assuming the token's value field holds the integer value
    expect(TOKEN_INT);
    return value;
}

int parseTerm() {
    int result = parseInteger();

        if (currentToken.type == TOKEN_PLUS) {
            nextToken(); // Consume the '+' token
            int nextValue = parseInteger();
            result += nextValue;
        }

        if (currentToken.type == TOKEN_MINUS) {
            nextToken(); 
            int nextValue = parseInteger();
            result -= nextValue;
        }

        if (currentToken.type == TOKEN_STAR) {
            nextToken(); 
            int nextValue = parseInteger();
            result *= nextValue;
        }

        if (currentToken.type == TOKEN_SLASH) {
            nextToken(); 
            int nextValue = parseInteger();
            result /= nextValue;
        }



    return result;
}

void parseExpression() {
    // This simplistic example only deals with integer literals and addition
    int result = parseTerm();
    printf("Result: %d\n", result);
}

void parseStatement() {
    // Implement parsing different kinds of statements by checking the first token
    // Here, as an example, we assume every statement is an expression followed by a semicolon
    parseExpression();
    expect(TOKEN_SEMICOLON);
}

void parseProgram() {
    nextToken(); 
    while (currentToken.type != TOKEN_EOF) {
        parseStatement();
    }
}

int main(int argc, char *argv[]) {
    wchar_t *source = L"3*5-2;"; // The source code as a wide character string
    tokens = tokenize(source);

    parseProgram();

    // Clean up the token array when done
    free(tokens);

    return 0;
}

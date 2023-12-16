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

int parseFactor() {
    int value;
    if (currentToken.type == TOKEN_INT) {
        value = currentToken.intValue;
        nextToken(); // Consume the integer token
    } else {
        parseError("Expecting an integer");
    }
    return value;
}

int parseTerm() {
    int result = parseFactor();

    while (currentToken.type == TOKEN_STAR || currentToken.type == TOKEN_SLASH) {
        TokenType operatorType = currentToken.type;
        nextToken(); // Consume the operator token

        int nextValue = parseFactor();

        if (operatorType == TOKEN_STAR) {
            result *= nextValue;
        } else if (operatorType == TOKEN_SLASH) {
            if (nextValue == 0) {
                parseError("Division by zero");
            }
            result /= nextValue;
        }
    }

    return result;
}

int parseExpression() {
    int result = parseTerm();

    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        TokenType operatorType = currentToken.type;
        nextToken(); // Consume the operator token

        int nextValue = parseTerm();

        if (operatorType == TOKEN_PLUS) {
            result += nextValue;
        } else if (operatorType == TOKEN_MINUS) {
            result -= nextValue;
        }
    }

    return result;
}

void parseStatement() {
    int result = parseExpression();
    printf("Result: %d\n", result);
    expect(TOKEN_SEMICOLON);
}

void parseProgram() {
    nextToken(); // Start parsing by fetching the first token
    while (currentToken.type != TOKEN_EOF) {
        parseStatement();
    }
}

int main(int argc, char *argv[]) {
    wchar_t *source = L"3*5+1-2;"; // The source code as a wide character string
    tokens = tokenize(source);

    parseProgram();

    // Clean up the token array when done
    free(tokens);

    return 0;
}

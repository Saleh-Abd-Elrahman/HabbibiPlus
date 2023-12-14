#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

// Function to tokenize the input
Token *tokenize(char *source) {
    int capacity = 10;
    Token *tokens = malloc(capacity * sizeof(Token));
    if (!tokens) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    int tokenCount = 0;

    while (*source != '\0') {
        // Resize tokens array if necessary
        if (tokenCount >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(Token));
            if (!tokens) {
                fprintf(stderr, "Failed to reallocate memory\n");
                exit(EXIT_FAILURE);
            }
        }

        if (isdigit(*source)) {
            tokens[tokenCount].type = TOKEN_INT;
            tokens[tokenCount].intValue = strtol(source, &source, 10); // strtol advances the source pointer
        } else {
            switch (*source) {
                case '+': 
                    tokens[tokenCount].type = TOKEN_PLUS; 
                    break;
                case '-': 
                    tokens[tokenCount].type = TOKEN_MINUS; 
                    break;
                case '*': 
                    tokens[tokenCount].type = TOKEN_STAR; 
                    break;
                case '/': 
                    tokens[tokenCount].type = TOKEN_SLASH; 
                    break;
                case '(': 
                    tokens[tokenCount].type = TOKEN_LPAREN; 
                    break;
                case ')': 
                    tokens[tokenCount].type = TOKEN_RPAREN; 
                    break;

                case 'إذا': 
                    tokens[tokenCount].type = TOKEN_IF; 
                    break;

                case 'وإلا': 
                    tokens[tokenCount].type = TOKEN_ELSE; 
                    break;

                case 'بينما': 
                    tokens[tokenCount].type = TOKEN_WHILE; 
                    break;
                
                case 'ل': 
                    tokens[tokenCount].type = TOKEN_FOR; 
                    break;
                
                case '%': 
                    tokens[tokenCount].type = TOKEN_MODULUS; 
                    break;
                
                case '^': 
                    tokens[tokenCount].type = TOKEN_EXPONENT; 
                    break;

                case '==': 
                    tokens[tokenCount].type = TOKEN_EQUAL_TO; 
                    break;

                case '<': 
                    tokens[tokenCount].type = TOKEN_LESS_THAN; 
                    break;

                case '>': 
                    tokens[tokenCount].type = TOKEN_GREATER_THAN; 
                    break;

                case '&&': 
                    tokens[tokenCount].type = TOKEN_AND; 
                    break;
                
                case '||': 
                    tokens[tokenCount].type = TOKEN_OR; 
                    break;
                
                case '"': 
                    tokens[tokenCount].type = TOKEN_DOUBLE_QUOTE; 
                    break;
                
                case '+=': 
                    tokens[tokenCount].type = TOKEN_INCREMENT_BY; 
                    break;

                case '-=': 
                    tokens[tokenCount].type = TOKEN_DECREASE_BY; 
                    break;

                case '*=': 
                    tokens[tokenCount].type = TOKEN_MULTIPLY_BY; 
                    break;

                case '/=': 
                    tokens[tokenCount].type = TOKEN_DIVIDE_BY; 
                    break;

                case '%=': 
                    tokens[tokenCount].type = TOKEN_MOD_BY; 
                    break;

                case '!=': 
                    tokens[tokenCount].type = TOKEN_NOT_EQUAL_TO; 
                    break;

                case '<=': 
                    tokens[tokenCount].type = TOKEN_LESS_THAN_OR_EQUAL_TO; 
                    break;

                case '>=': 
                    tokens[tokenCount].type = TOKEN_GREATER_THAN_OR_EQUAL_TO; 
                    break;

                case ',': 
                    tokens[tokenCount].type = TOKEN_COMMA; 
                    break;

                case ';': 
                    tokens[tokenCount].type = TOKEN_SEMICOLON; 
                    break;

                case '.': 
                    tokens[tokenCount].type = TOKEN_PERIOD; 
                    break;

                case '?': 
                    tokens[tokenCount].type = TOKEN_QUESTION_MARK; 
                    break;

                case '!': 
                    tokens[tokenCount].type = TOKEN_EXCLAMATION_MARK; 
                    break;

                case '[': 
                    tokens[tokenCount].type = TOKEN_LEFT_BRACKET; 
                    break;

                case ']': 
                    tokens[tokenCount].type = TOKEN_RIGHT_BRACKET; 
                    break;

                default: 
                    printf("Unexpected character: %c\n", *source);
                    exit(1);
            }
            source++;
        }
        tokenCount++;
    }

    // Mark the end of the tokens
    tokens[tokenCount].type = TOKEN_EOF;
    return tokens;
}

// Function to print tokens for debugging
void printToken(Token token) {
    switch (token.type) {
        case TOKEN_INT: 
            printf("INT(%d) ", token.intValue); // here
            break;
        case TOKEN_FLOAT:
            printf("FLOAT(%f) ", token.floatValue); // here
            break;
        case TOKEN_PLUS: 
            printf("PLUS "); 
            break;
        case TOKEN_MINUS: 
            printf("MINUS "); 
            break;
        case TOKEN_STAR: 
            printf("STAR "); 
            break;
        case TOKEN_SLASH: 
            printf("SLASH "); 
            break;
        case TOKEN_LPAREN: 
            printf("LPAREN "); 
            break;
        case TOKEN_RPAREN: 
            printf("RPAREN "); 
            break;
        case TOKEN_VARIABLE: 
            printf("VARIABLE(%s) ", token.identifier); // here
            break;
        case TOKEN_EOF: 
            printf("EOF "); 
            break;
        case TOKEN_FOR: 
            printf("FOR "); 
            break;
        case TOKEN_IF: 
            printf("IF "); 
            break;
        case TOKEN_ELSE: 
            printf("ELSE "); 
            break;
        case TOKEN_WHILE: 
            printf("WHILE "); 
            break;
        case TOKEN_CHAR: 
            printf("CHAR(%c) ", token.varName); // here
            break;
        case TOKEN_RETURN: 
            printf("RETURN "); 
            break;
        case TOKEN_MODULUS: 
            printf("MODULUS "); 
            break;
        case TOKEN_EQUAL_TO: 
            printf("EQUAL_TO "); 
            break;
        case TOKEN_LESS_THAN: 
            printf("LESS_THAN "); 
            break;
        case TOKEN_GREATER_THAN: 
            printf("GREATER_THAN "); 
            break;
        case TOKEN_AND: 
            printf("AND "); 
            break;
        case TOKEN_OR: 
            printf("OR "); 
            break;
        case TOKEN_DOUBLE_QUOTE:
            printf("DOUBLE_QUOTE ");
            break;
        case TOKEN_INCREMENT_BY:
            printf("INCREMENT_BY ");
            break;
        case TOKEN_MULTIPLY_BY:
            printf("MULTIPLY_BY ");
            break;
        case TOKEN_DECREASE_BY:
            printf("DECREASE_BY ");
            break;
        case TOKEN_DIVIDE_BY:
            printf("DIVIDE_BY ");
            break;
        case TOKEN_MOD_BY:
            printf("MOD_BY ");
            break;
        case TOKEN_NOT_EQUAL_TO:
            printf("NOT_EQUAL_TO ");
            break;
        case TOKEN_LESS_THAN_OR_EQUAL_TO:
            printf("LESS_THAN_OR_EQUAL_TO ");
            break;
        case TOKEN_GREATER_THAN_OR_EQUAL_TO:
            printf("GREATER_THAN_OR_EQUAL_TO ");
            break;
        case TOKEN_COMMA:
            printf("COMMA ");
            break;
        case TOKEN_SEMICOLON:
            printf("SEMICOLON ");
            break;
        case TOKEN_PERIOD:
            printf("PERIOD ");
            break;
        case TOKEN_COLON:
            printf("COLON ");
            break;
        case TOKEN_QUESTION_MARK:
            printf("QUESTION_MARK ");
            break;
        case TOKEN_EXCLAMATION_MARK:
            printf("EXCLAMATION_MARK ");
            break;
        case TOKEN_LEFT_BRACKET:
            printf("LEFT_BRACKET ");
            break;
        case TOKEN_RIGHT_BRACKET:
            printf("RIGHT_BRACKET ");
            break;
        default:
            printf("UNKNOWN ");
            break;
    }
}

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
            printf("INT(%d) ", token.intValue); 
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
        case TOKEN_EOF: 
            printf("EOF "); 
            break;
    }
}
#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

// Function to tokenize the input
Token *tokenize(wchar_t *source) 
{
    int capacity = 10;
    Token *tokens = malloc(capacity * sizeof(Token));
    if (!tokens) 
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    int tokenCount = 0;

    while (*source != '\0')
    {
        // Resize tokens array if necessary
        if (tokenCount >= capacity) 
        {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(Token));
            if (!tokens) 
            {
                fprintf(stderr, "Failed to reallocate memory\n");
                exit(EXIT_FAILURE);
            }
        }

        if (isspace(*source)) 
        {
            source++;
            continue;
        }

        if (isdigit(*source)) 
        {
            tokens[tokenCount].type = TOKEN_INT;
            tokens[tokenCount].intValue = wcstol(source, &source, 10);; // strtol advances the source pointer
        } 
        
        else 
        {
            switch (*source) 
            {
                case '+':
                    if (wcsncmp(source, L"+=", wcslen(L"+=")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_INCREMENT_BY;
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_PLUS; 
                    break;
                    
                case '-':
                    if (wcsncmp(source, L"-=", wcslen(L"-=")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_DECREASE_BY; 
                        source++;
                        break;
                    } 
                    tokens[tokenCount].type = TOKEN_MINUS; 
                    break;

                case '*': 
                    if (wcsncmp(source, L"*=", wcslen(L"*=")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_MULTIPLY_BY; 
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_STAR; 
                    break;

                case '/': 
                    if (wcsncmp(source, L"/=", wcslen(L"/=")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_DIVIDE_BY; 
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_SLASH; 
                    break;

                case '(': 
                    tokens[tokenCount].type = TOKEN_LPAREN; 
                    break;

                case ')': 
                    tokens[tokenCount].type = TOKEN_RPAREN; 
                    break;

                case '%': 
                    if (wcsncmp(source, L"%=", wcslen(L"%=")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_MOD_BY; 
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_MODULUS; 
                    break;
                
                case '^': 
                    tokens[tokenCount].type = TOKEN_EXPONENT; 
                    break;

                case '<': 
                    if (wcsncmp(source, L"<=", wcslen(L"<=")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_LESS_THAN_OR_EQUAL_TO; 
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_LESS_THAN; 
                    break;

                case '>': 
                    if (wcsncmp(source, L">=", wcslen(L">=")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_GREATER_THAN_OR_EQUAL_TO;
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_GREATER_THAN; 
                    break;

                case '"': 
                    tokens[tokenCount].type = TOKEN_DOUBLE_QUOTE; 
                    break;

                case ',': 
                    tokens[tokenCount].type = TOKEN_COMMA; 
                    break;

                case ';': 
                    tokens[tokenCount].type = TOKEN_SEMICOLON; 
                    break;

                case '=':
                    if (wcsncmp(source, L"==", wcslen(L"==")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_EQUAL_TO; 
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_ASSIGNMENT; 
                    break;

                case '.': 
                    tokens[tokenCount].type = TOKEN_PERIOD; 
                    break;

                case '?': 
                    tokens[tokenCount].type = TOKEN_QUESTION_MARK; 
                    break;

                case '!': 
                    if (wcsncmp(source, L"!=", wcslen(L"!=")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_NOT_EQUAL_TO;
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_EXCLAMATION_MARK; 
                    break;

                case '[': 
                    tokens[tokenCount].type = TOKEN_LEFT_BRACKET; 
                    break;

                case ']': 
                    tokens[tokenCount].type = TOKEN_RIGHT_BRACKET; 
                    break;

                case '#': 
                    tokens[tokenCount].type = TOKEN_COMMENT; 
                    break;

                default: 
                    if (wcsncmp(source, L"إذا", wcslen(L"إذا")) == 0) {
                        tokens[tokenCount].type = TOKEN_IF;
                        source += wcslen(L"إذا"); // Move the source pointer past the recognized token
                        break;
                    }
                
                    else if (wcsncmp(source, L"وإلا", wcslen(L"وإلا")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_ELSE;
                        source += wcslen(L"وإلا");
                        break;
                    }

                    else if (wcsncmp(source, L"بينما", wcslen(L"بينما")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_WHILE;
                        source += wcslen(L"بينما");
                        break;
                    }
                     
                    else if (wcsncmp(source, L"ل", wcslen(L"ل")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_FOR;
                        source += wcslen(L"ل");
                        break;
                    }

                    else if (wcsncmp(source, L"مطبعة", wcslen(L"مطبعة")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_PRINT;
                        source += wcslen(L"مطبعة");
                        break;
                    }

                    else if (wcsncmp(source, L"&&", wcslen(L"&&")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_AND; 
                        source++;
                        break;
                    }

                    else if (wcsncmp(source, L"||", wcslen(L"||")) == 0)
                    {
                        tokens[tokenCount].type = TOKEN_OR; 
                        source++;
                        break;
                    }

                    else
                    {
                        printf("Unexpected character: %c\n", *source);
                        exit(1);
                    }                  
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
void printToken(Token token) 
{
    switch (token.type) 
    {
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
            printf("VARIABLE(%s) ", token.varName); // here
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
            printf("STRING(%s) ", token.varName); // here
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

        case TOKEN_ASSIGNMENT:
            printf("ASSIGNMENT ");
            break;
            
        default:
            printf("UNKNOWN ");
            break;
    }
}
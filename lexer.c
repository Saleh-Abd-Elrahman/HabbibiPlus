#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

int isArabicLetter(wchar_t ch) {
    // Check if the character falls within the Arabic Unicode range
    return (ch >= 0x0600 && ch <= 0x06FF); // This range covers most Arabic characters
}

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

        if (isdigit(*source) || (*source == '-' && isdigit(*(source + 1)))) 
        {
            wchar_t *endPtr;
            tokens[tokenCount].type = TOKEN_INT;
            tokens[tokenCount].intValue = wcstol(source, &endPtr, 10); // Try to read as integer first

            if (*endPtr == L'.') {
                if (isdigit(*(endPtr + 1))) {
                    tokens[tokenCount].type = TOKEN_DOUBLE;
                    tokens[tokenCount].doubleValue = wcstod(source, &source); // Read DOUBLE, advance source
                } else {
                    source = endPtr; // Advance source after reading int
                }
            } else {
                source = endPtr; // Advance source after reading int
            }

        }



        
        else 
        {
            switch (*source) 
            {
                case '+':
                    if (wcsncmp(source, L"+=", wcslen(L"+=")) == 0) {
                        tokens[tokenCount].type = TOKEN_INCREMENT_BY;
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_PLUS; 
                    break;
                    
                case '-':
                    if (wcsncmp(source, L"-=", wcslen(L"-=")) == 0) {
                        tokens[tokenCount].type = TOKEN_DECREASE_BY; 
                        source++;
                        break;
                    } 
                    tokens[tokenCount].type = TOKEN_MINUS; 
                    break;

                case '*': 
                    if (wcsncmp(source, L"*=", wcslen(L"*=")) == 0) {
                        tokens[tokenCount].type = TOKEN_MULTIPLY_BY; 
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_STAR; 
                    break;

                case '/': 
                    if (wcsncmp(source, L"/=", wcslen(L"/=")) == 0) {
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
                    if (wcsncmp(source, L"%=", wcslen(L"%=")) == 0) {
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
                    if (wcsncmp(source, L"<=", wcslen(L"<=")) == 0) {
                        tokens[tokenCount].type = TOKEN_LESS_THAN_OR_EQUAL_TO; 
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_LESS_THAN; 
                    break;

                case '>': 
                    if (wcsncmp(source, L">=", wcslen(L">=")) == 0) {
                        tokens[tokenCount].type = TOKEN_GREATER_THAN_OR_EQUAL_TO;
                        source++;
                        break;
                    }
                    tokens[tokenCount].type = TOKEN_GREATER_THAN; 
                    break;
        
                case ',': 
                    tokens[tokenCount].type = TOKEN_COMMA; 
                    break;

                case ';': 
                    tokens[tokenCount].type = TOKEN_SEMICOLON; 
                    break;

                case '=':
                    if (wcsncmp(source, L"==", wcslen(L"==")) == 0) {
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
                    if (wcsncmp(source, L"!=", wcslen(L"!=")) == 0) {
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
                
                    else if (wcsncmp(source, L"وإلا", wcslen(L"وإلا")) == 0) {
                        tokens[tokenCount].type = TOKEN_ELSE;
                        source += wcslen(L"وإلا");
                        break;
                    }

                    else if (wcsncmp(source, L"بينما", wcslen(L"بينما")) == 0) {
                        tokens[tokenCount].type = TOKEN_WHILE;
                        source += wcslen(L"بينما");
                        break;
                    }
                     
                    else if (wcsncmp(source, L"ل", wcslen(L"ل")) == 0) {
                        wchar_t* nextChar = source + 1;

                        // Check if the next character is an Arabic letter
                        if (isArabicLetter(*nextChar)) {
                            // Tokenize as a variable
                            tokens[tokenCount].type = TOKEN_VARIABLE;
                            tokens[tokenCount].varName = malloc(sizeof(wchar_t)); // Allocate initial memory
                            tokens[tokenCount].varName[0] = L'\0'; // Set as empty string

                            while (isArabicLetter(*source) || isdigit(*source) || *source == L'_') {
                                size_t len = wcslen(tokens[tokenCount].varName);
                                wchar_t* str = realloc(tokens[tokenCount].varName, sizeof(wchar_t) * (len + 2));
                                if (!str) {
                                    fprintf(stderr, "Failed to reallocate memory\n");
                                    exit(EXIT_FAILURE);
                                }
                                tokens[tokenCount].varName = str;
                                wchar_t tempStr[2] = {*source, L'\0'}; // Create a temporary string
                                wcscat(tokens[tokenCount].varName, tempStr); // Append the character
                                source++;
                            }
                        } else {
                            // Tokenize as for
                            tokens[tokenCount].type = TOKEN_FOR;
                            source++; // Move past "ل"
                        }
                        break;
                    }

                    else if (wcsncmp(source, L"طباعة", wcslen(L"طباعة")) == 0) {
                        tokens[tokenCount].type = TOKEN_PRINT;
                        source += wcslen(L"طباعة") - 1;
                        break;
                    } 

                    else if (wcsncmp(source, L"&&", wcslen(L"&&")) == 0) {
                        tokens[tokenCount].type = TOKEN_AND; 
                        source++;
                        break;
                    }

                    else if (wcsncmp(source, L"||", wcslen(L"||")) == 0) {
                        tokens[tokenCount].type = TOKEN_OR; 
                        source++;
                        break;
                    }
                    
                    else if (isArabicLetter(*source)) {
                        tokens[tokenCount].type = TOKEN_VARIABLE;
                        tokens[tokenCount].varName = malloc(sizeof(wchar_t)); // Allocate initial memory
                        tokens[tokenCount].varName[0] = L'\0'; // Set as empty string

                        while (isArabicLetter(*source) || isdigit(*source) || *source == L'_') {
                            size_t len = wcslen(tokens[tokenCount].varName);
                            wchar_t* str = realloc(tokens[tokenCount].varName, sizeof(wchar_t) * (len + 2));
                            if (!str) {
                                fprintf(stderr, "Failed to reallocate memory\n");
                                exit(EXIT_FAILURE);
                            }
                            tokens[tokenCount].varName = str;
                            wchar_t tempStr[2] = {*source, L'\0'}; // Create a temporary string
                            wcscat(tokens[tokenCount].varName, tempStr); // Append the character
                            source++;
                        }
                        source--;
                        break;
                    }

                    else if (*source == '"') {
                        source++; // Skip the opening quote
                        wchar_t* start = source; // Remember the start of the string

                        // Find the closing quote or end of the source
                        while (*source && *source != '"') {
                            source++;
                        }

                        if (*source == '"') {
                            size_t len = source - start;
                            tokens[tokenCount].type = TOKEN_CHAR;
                            tokens[tokenCount].charValue = malloc(sizeof(wchar_t) * (len + 1)); // Allocate memory for the string
                            if (tokens[tokenCount].charValue)
                            {
                                wcsncpy(tokens[tokenCount].charValue, start, len); // Copy the string
                                tokens[tokenCount].charValue[len] = L'\0'; // Null-terminate the string
                            } 
                            else {
                                fprintf(stderr, "Failed to allocate memory\n");
                                exit(EXIT_FAILURE);
                            }
                        } 
                        else {
                            fprintf(stderr, "Unterminated string literal\n");
                            exit(EXIT_FAILURE);
                        }
                        break;
                    }
                    else {
                        printf("Unexpected character: %c\n", *source);
                        break;
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

        case TOKEN_DOUBLE:
            printf("DOUBLE(%lf) ", token.doubleValue); // here
            break;

        case TOKEN_PLUS: 
            printf("PLUS "); 
            break;

        case TOKEN_MINUS: 
            printf("MINUS "); 
            break;

        case TOKEN_EXPONENT: 
            printf("EXPONENT"); 
            break;

        case TOKEN_COMMENT: 
            printf("COMMENT"); 
            break;
        
        case TOKEN_PRINT: 
            printf("PRINT"); 
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
            wprintf(L"VARIABLE(%ls) ", token.varName);
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
            wprintf(L"STRING(%ls) ", token.charValue);
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
#include "lexer.h"
#include "lexer.c"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

Token *tokens;
int currentTokenIndex = 0;
Token currentToken;

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_CHAR
} ValueType;

typedef struct {
    wchar_t *name;  // Variable name
    ValueType type;  // Type of the value
    union {
        int intValue;
        double doubleValue;
        wchar_t *charValue;
    } value;
} Symbol;

#define MAX_SYMBOLS 100
Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

int variableExists(wchar_t *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (wcscmp(symbolTable[i].name, name) == 0) {
            return 1; // Variable found
        }
    }
    return 0; // Variable not found
}

void addSymbol(wchar_t *name, ValueType type, int intValue, double doubleValue, wchar_t *charValue) {
    if (symbolCount >= MAX_SYMBOLS) {
        fwprintf(stderr, L"Symbol table overflow\n");
        exit(EXIT_FAILURE);
    }
    symbolTable[symbolCount].name = _wcsdup(name);
    symbolTable[symbolCount].type = type;
    switch (type) {
        case TYPE_INT:
            symbolTable[symbolCount].value.intValue = intValue;
            break;
        case TYPE_DOUBLE:
            symbolTable[symbolCount].value.doubleValue = doubleValue;
            break;
        case TYPE_CHAR:
            symbolTable[symbolCount].value.charValue = _wcsdup(charValue);
            break;
        default:
            fwprintf(stderr, L"Unknown type\n");
            exit(EXIT_FAILURE);
    }
    symbolCount++;
}

void updateSymbol(wchar_t *name, ValueType type, int intValue, double doubleValue, wchar_t *charValue) {
    for (int i = 0; i < symbolCount; i++) {
        if (wcscmp(symbolTable[i].name, name) == 0) {
            symbolTable[i].type = type; // Update type in case it changes
            switch (type) {
                case TYPE_INT:
                    symbolTable[i].value.intValue = intValue;
                    break;
                case TYPE_DOUBLE:
                    symbolTable[i].value.doubleValue = doubleValue;
                    break;
                case TYPE_CHAR:
                    free(symbolTable[i].value.charValue);
                    symbolTable[i].value.charValue = _wcsdup(charValue);
                    break;
                default:
                    fwprintf(stderr, L"Unknown type\n");
                    exit(EXIT_FAILURE);
            }
            return;
        }
    }
    fwprintf(stderr, L"Variable not found for update: %ls\n", name);
    exit(EXIT_FAILURE);
}

int getIntValue(wchar_t *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (wcscmp(symbolTable[i].name, name) == 0) {
            if (symbolTable[i].type == TYPE_INT) {
                return symbolTable[i].value.intValue;
            } else {
                fwprintf(stderr, L"Type error: %ls is not an integer\n", name);
                exit(EXIT_FAILURE);
            }
        }
    }
    fwprintf(stderr, L"Undefined integer variable: %ls\n", name);
    exit(EXIT_FAILURE);
}

double getDoubleValue(wchar_t *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (wcscmp(symbolTable[i].name, name) == 0) {
            if (symbolTable[i].type == TYPE_DOUBLE) {
                return symbolTable[i].value.doubleValue;
            } else {
                fwprintf(stderr, L"Type error: %ls is not a double\n", name);
                exit(EXIT_FAILURE);
            }
        }
    }
    fwprintf(stderr, L"Undefined double variable: %ls\n", name);
    exit(EXIT_FAILURE);
}

wchar_t* getCharValue(wchar_t *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (wcscmp(symbolTable[i].name, name) == 0) {
            if (symbolTable[i].type == TYPE_CHAR) {
                return symbolTable[i].value.charValue;
            } else {
                fwprintf(stderr, L"Type error: %ls is not a string\n", name);
                exit(EXIT_FAILURE);
            }
        }
    }
    fwprintf(stderr, L"Undefined string variable: %ls\n", name);
    exit(EXIT_FAILURE);
}


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

void parsePrintStatement() {
    nextToken(); // Consume the print token

    // Expect the left parenthesis
    expect(TOKEN_LPAREN);

    switch (currentToken.type) {
        case TOKEN_CHAR:
            // Print the string literal
            printf("%ls\n", currentToken.charValue);
            nextToken(); // Consume the char token
            break;
        case TOKEN_VARIABLE:
            // Print the value of the variable
            if (currentToken.type == TYPE_INT){
                printf("%d\n", getIntValue(currentToken.varName));
            }
            else if(currentToken.type == TYPE_DOUBLE){
                printf("%lf\n", getDoubleValue(currentToken.varName));
            }
            else if(currentToken.type == TYPE_CHAR){
                wprintf(L"%ls\n", getCharValue(currentToken.varName));
            }
            nextToken(); // Consume the variable token
            break;
        default:
            parseError("Expected a string or a variable in print statement");
    }

    // Expect the right parenthesis and semicolon
    expect(TOKEN_RPAREN);
    expect(TOKEN_SEMICOLON);
}

void parseAssignment() {
    if (currentToken.type != TOKEN_VARIABLE) {
        parseError(L"Expected variable name");
    }

    wchar_t *varName = _wcsdup(currentToken.varName); // Store the variable name
    nextToken(); // Move to the assignment operator

    TokenType assignmentType = currentToken.type; // Store the assignment type
    nextToken(); // Move past the assignment operator

    // Handle different types of right-hand side expressions
    if (currentToken.type == TOKEN_INT) {
        int intValue = currentToken.intValue; // Assuming you have this field in your Token struct
        handleIntAssignment(varName, assignmentType, intValue);
    } else if (currentToken.type == TOKEN_DOUBLE) {
        double doubleValue = currentToken.doubleValue; // Assuming you have this field
        handleDoubleAssignment(varName, assignmentType, doubleValue);
    } else if (currentToken.type == TOKEN_CHAR) {
        wchar_t *charValue = _wcsdup(currentToken.charValue); // Assuming you have this field
        handleCharAssignment(varName, assignmentType, charValue);
        free(charValue);
    } else {
        parseError(L"Invalid right-hand side in assignment");
    }

    free(varName); // Clean up the allocated variable name

    expect(TOKEN_SEMICOLON); // Expect a semicolon at the end of the assignment
}





void parseStatement() {
    switch (currentToken.type) {
        case TOKEN_VARIABLE:
            parseAssignment();  // Handle variable assignment
            break;
        case TOKEN_FOR:
            parseForStatement();  // Handle for loop
            break;
        case TOKEN_IF:
            parseIfStatement();  // Handle if statement
            break;
        case TOKEN_WHILE:
            parseWhileStatement();  // Handle while loop
            break;
        case TOKEN_PRINT:
            parsePrintStatement();  // Handle print statement
            break;
        case TOKEN_RETURN:
            parseReturnStatement();  // Handle return statement
            break;
        default:
            parseError("Unexpected token in statement");
    }
}


void parseProgram() {
    nextToken(); // Start parsing by fetching the first token
    while (currentToken.type != TOKEN_EOF) {
        parseStatement();
    }
}

void freeSymbolTable() {
    for (int i = 0; i < symbolCount; i++) {
        // Free the memory allocated for the name of the symbol
        free(symbolTable[i].name);
        symbolTable[i].name = NULL;

        // If the symbol is of string type, also free the memory allocated for the string value
        if (symbolTable[i].type == TYPE_CHAR) {
            free(symbolTable[i].value.charValue);
            symbolTable[i].value.charValue = NULL;
        }
    }

    // Reset the symbol count to 0
    symbolCount = 0;
}

int main(int argc, char *argv[]) {
    setlocale(LC_CTYPE, "");

    FILE *file = fopen("source_code.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Determine the length of the file
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file contents
    wchar_t *input = malloc((length + 1) * sizeof(wchar_t));
    if (!input) {
        wprintf(L"Failed to allocate memory for file content\n");
        fclose(file);
        return 1;
    }

    // Read the file into the allocated array
    int i = 0;
    wint_t wc;
    while ((wc = fgetwc(file)) != WEOF) {
        input[i++] = wc;
    }
    input[i] = L'\0';

    fclose(file);
    tokens = tokenize(input);

    parseProgram();

    // Clean up the token array when done
    free(tokens);
    freeSymbolTable();

    return 0;
}

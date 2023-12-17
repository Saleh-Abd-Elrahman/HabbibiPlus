#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>


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

Token evaluateExpression();

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

    wchar_t* nameCopy = wcsdup(name);
    if (!nameCopy) {
        fwprintf(stderr, L"Failed to allocate memory for symbol name\n");
        exit(EXIT_FAILURE);
    }

    symbolTable[symbolCount].name = nameCopy;
    symbolTable[symbolCount].type = type;

    switch (type) {
        case TYPE_INT:
            symbolTable[symbolCount].value.intValue = intValue;
            break;
        case TYPE_DOUBLE:
            symbolTable[symbolCount].value.doubleValue = doubleValue;
            break;
        case TYPE_CHAR:
            {
                wchar_t* charValueCopy = wcsdup(charValue);
                if (!charValueCopy) {
                    fwprintf(stderr, L"Failed to allocate memory for char value\n");
                    free(nameCopy);  // Free the previously allocated name before exiting
                    exit(EXIT_FAILURE);
                }
                symbolTable[symbolCount].value.charValue = charValueCopy;
            }
            break;
        default:
            fwprintf(stderr, L"Unknown type\n");
            free(nameCopy);  // Free the name in case of an error
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
                    symbolTable[i].value.charValue = wcsdup(charValue);
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

void handleAssignment(wchar_t *varName, ValueType valueType, int intValue, double doubleValue, wchar_t *charValue) {
    if (variableExists(varName)) {
        // Update existing variable
        updateSymbol(varName, valueType, intValue, doubleValue, charValue);
    } else {
        // Add new variable
        addSymbol(varName, valueType, intValue, doubleValue, charValue);
    }
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

void parseError(wchar_t* message) {
    fprintf(stderr, "Parse error: %ls\n", message);
    fprintf(stderr, "Unhandled token type: %d\n", currentToken.type);
    exit(EXIT_FAILURE);
}

void expect(TokenType expectedType) {
    if (currentToken.type == expectedType) {
        nextToken();
    } else {
        parseError(L"Unexpected token");
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
            parseError(L"Expected a string or a variable in print statement");
    }

    // Expect the right parenthesis and semicolon
    expect(TOKEN_RPAREN);
    expect(TOKEN_SEMICOLON);
}

// Converts a token from integer to double type.
void convertToDouble(Token *token) {
    if (token->type == TOKEN_INT) {
        token->doubleValue = (double)token->intValue; // Convert int to double
        token->type = TOKEN_DOUBLE; // Update the token type
    }
}


// Performs arithmetic operations based on the operator type.
Token performArithmeticOperation(Token left, Token right, TokenType operatorType) {
    Token result;

    // Handle type conversion if operands are of different types
    if (left.type != right.type) {
        if (left.type == TOKEN_DOUBLE || right.type == TOKEN_DOUBLE) {
            convertToDouble(&left);
            convertToDouble(&right);
        }
    }

    // Determine the result type (double if any operand is double)
    result.type = (left.type == TOKEN_DOUBLE || right.type == TOKEN_DOUBLE) ? TOKEN_DOUBLE : TOKEN_INT;

    // Perform the arithmetic operation based on the result type
    if (result.type == TOKEN_INT) {
        // Integer arithmetic
        switch (operatorType) {
            case TOKEN_PLUS:
                result.intValue = left.intValue + right.intValue;
                break;
            case TOKEN_MINUS:
                result.intValue = left.intValue - right.intValue;
                break;
            case TOKEN_STAR:
                result.intValue = left.intValue * right.intValue;
                break;
            case TOKEN_SLASH:
                if (right.intValue == 0) {
                    parseError(L"Runtime error: Division by zero in expression.\n");
                    exit(EXIT_FAILURE);
                }
                result.intValue = left.intValue / right.intValue; // Handle division by zero
                break;
            default:
                parseError(L"Unexpected token in statement");
        }
    } else if (result.type == TOKEN_DOUBLE) {
        // Floating-point arithmetic
        switch (operatorType) {
            case TOKEN_PLUS:
                result.doubleValue = left.doubleValue + right.doubleValue;
                break;
            case TOKEN_MINUS:
                result.doubleValue = left.doubleValue - right.doubleValue;
                break;
            case TOKEN_STAR:
                result.doubleValue = left.doubleValue * right.doubleValue;
                break;
            case TOKEN_SLASH:
                if (right.doubleValue == 0) {
                        parseError(L"Runtime error: Division by zero in expression.\n");
                        exit(EXIT_FAILURE);
                    }
                result.doubleValue = left.doubleValue / right.doubleValue; // Handle division by zero
                break;
            default:
                parseError(L"Unexpected token in statement");
        }
    }

    return result;
}

// Parses primary expressions like numbers and parenthesized expressions.
Token parsePrimaryExpression() {
    Token result;
    if (currentToken.type == TOKEN_INT || currentToken.type == TOKEN_DOUBLE) {
        // If the current token is a number, return it as the result
        result = currentToken;
        nextToken(); // Move past the number
        return result;
    } else if (currentToken.type == TOKEN_LPAREN) {
        nextToken(); // Move past the '('
        result = evaluateExpression(); // Evaluate the expression inside the parentheses
        if (currentToken.type != TOKEN_RPAREN) {
            parseError(L"Expected ')'");
        }
        nextToken(); // Move past the ')'
        return result;
    } else {
        // If the token is not a number or a parenthesis, it's an error
        parseError(L"Expected a primary expression");
        Token errorToken;
        errorToken.type = TOKEN_ERROR; // Assuming TOKEN_ERROR is a defined error type
        return errorToken;
    }
}

// Parses multiplication and division.
Token parseMultiplicationDivision() {
    // Parse a primary expression, which could be a number or a parenthesized expression
    Token result = parsePrimaryExpression();

    // Loop to handle a series of multiplication/division operations
    while (currentToken.type == TOKEN_STAR || currentToken.type == TOKEN_SLASH) {
        TokenType operatorType = currentToken.type;
        nextToken(); // Move past the '*' or '/' operator
        Token right = parsePrimaryExpression(); // Parse the right operand

        // Perform the arithmetic operation and update the result
        result = performArithmeticOperation(result, right, operatorType);
    }

    return result;
}

// Parses addition and subtraction, which have lower precedence than multiplication and division.
Token parseAdditionSubtraction() {
    // First, parse the higher precedence operations (multiplication and division)
    Token result = parseMultiplicationDivision();

    // Loop to handle a series of addition/subtraction operations
    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        TokenType operatorType = currentToken.type;
        nextToken(); // Move past the '+' or '-' operator
        Token right = parseMultiplicationDivision(); // Parse the right operand

        // Perform the arithmetic operation and update the result
        result = performArithmeticOperation(result, right, operatorType);
    }

    return result;
}

// Entry point for evaluating an expression.
Token evaluateExpression() {
    // Start with the lowest precedence operations (addition and subtraction)
    return parseAdditionSubtraction();
}
void parseIncrementation(wchar_t *varName, double value, TokenType operation) {
    for (int i = 0; i < symbolCount; i++) {
        if (wcscmp(symbolTable[i].name, varName) == 0) {
            if (symbolTable[i].type == TYPE_INT) {
                if (operation == TOKEN_INCREMENT_BY)
                    symbolTable[i].value.intValue += (int)value;
                else if (operation == TOKEN_DECREASE_BY)
                    symbolTable[i].value.intValue -= (int)value;
                else if (operation == TOKEN_MULTIPLY_BY)
                    symbolTable[i].value.intValue *= (int)value;
                else if (operation == TOKEN_DIVIDE_BY)
                    symbolTable[i].value.intValue /= (int)value;
                else if (operation == TOKEN_MOD_BY)
                    symbolTable[i].value.intValue %= (int)value;
            } else if (symbolTable[i].type == TYPE_DOUBLE) {
                if (operation == TOKEN_INCREMENT_BY)
                    symbolTable[i].value.doubleValue += value;
                else if (operation == TOKEN_DECREASE_BY)
                    symbolTable[i].value.doubleValue -= value;
                else if (operation == TOKEN_MULTIPLY_BY)
                    symbolTable[i].value.doubleValue *= value;
                else if (operation == TOKEN_DIVIDE_BY)
                    symbolTable[i].value.doubleValue /= value;
                // Note: Modulo operation not applicable for doubles
            }
            return;
        }
    }
    fwprintf(stderr, L"Variable not found for update: %ls\n", varName);
    exit(EXIT_FAILURE);
}

// Implementation of parseIncrementBy
void parseIncrementBy(wchar_t *varName, TokenType type, int intValue, double doubleValue) {
    double value = (type == TYPE_INT) ? (double)intValue : doubleValue;
    parseIncrementation(varName, value, TOKEN_INCREMENT_BY);
}

// Implementation of parseDivideBy
void parseDivideBy(wchar_t *varName, TokenType type, int intValue, double doubleValue) {
    double value = (type == TYPE_INT) ? (double)intValue : doubleValue;
    parseIncrementation(varName, value, TOKEN_DIVIDE_BY);
}

// Implementation of parseDecreaseBy
void parseDecreaseBy(wchar_t *varName, TokenType type, int intValue, double doubleValue) {
    double value = (type == TYPE_INT) ? (double)intValue : doubleValue;
    parseIncrementation(varName, value, TOKEN_DECREASE_BY);
}

// Implementation of parseMultiplyBy
void parseMultiplyBy(wchar_t *varName, TokenType type, int intValue, double doubleValue) {
    double value = (type == TYPE_INT) ? (double)intValue : doubleValue;
    parseIncrementation(varName, value, TOKEN_MULTIPLY_BY);
}

// Implementation of parseModBy
void parseModBy(wchar_t *varName, TokenType type, int intValue, double doubleValue) {
    if (type != TYPE_INT) {
        fwprintf(stderr, L"Modulo operation not supported for double\n");
        exit(EXIT_FAILURE);
    }
    parseIncrementation(varName, (double)intValue, TOKEN_MOD_BY);
}

void parseAssignment() {
    if (currentToken.type != TOKEN_VARIABLE) {
        parseError(L"Expected variable name");
    }

    wchar_t *varName = wcsdup(currentToken.varName); // Store the variable name
    nextToken(); // Move to the assignment operator

    TokenType assignmentType = currentToken.type; // Store the assignment type
    nextToken(); // Move past the assignment operator

    // Evaluate the right-hand side expression
    Token rhsResult = evaluateExpression();

    if (assignmentType == TOKEN_ASSIGNMENT) {
        // Handle different types of right-hand side expressions
        if (rhsResult.type == TYPE_INT) {
            handleAssignment(varName, TYPE_INT, rhsResult.intValue, 0, NULL);
        } else if (rhsResult.type == TYPE_DOUBLE) {
            handleAssignment(varName, TYPE_DOUBLE, 0, rhsResult.doubleValue, NULL);
        } else if (rhsResult.type == TYPE_CHAR) {
            if(!rhsResult.charValue){
                wprintf(L"%ls", rhsResult.charValue);
                exit(EXIT_FAILURE);
            }

            handleAssignment(varName, TYPE_CHAR, 0, 0, rhsResult.charValue);
        } else {
            parseError(L"Invalid right-hand side in assignment");
        }
        
    }
    else if(assignmentType == TOKEN_INCREMENT_BY) {
        if (rhsResult.type == TOKEN_INT) {
            parseIncrementBy(varName, TOKEN_INT, rhsResult.intValue, 0);
        } else if (rhsResult.type == TOKEN_DOUBLE) {
            parseIncrementBy(varName, TOKEN_DOUBLE, 0, rhsResult.doubleValue);
        } else {
            parseError(L"Invalid right-hand side in assignment");
        }
    }
    else if(assignmentType == TOKEN_DIVIDE_BY) {
        if (rhsResult.type == TOKEN_INT) {
            parseDivideBy(varName, TOKEN_INT, rhsResult.intValue, 0);
        } else if (rhsResult.type == TOKEN_DOUBLE) {
            parseDivideBy(varName, TOKEN_DOUBLE, 0, rhsResult.doubleValue);
        } else {
            parseError(L"Invalid right-hand side in assignment");
        }
    }
    else if(assignmentType == TOKEN_DECREASE_BY) {
        if (rhsResult.type == TOKEN_INT) {
            parseDecreaseBy(varName, TOKEN_INT, rhsResult.intValue, 0);
        } else if (rhsResult.type == TOKEN_DOUBLE) {
            parseDecreaseBy(varName, TOKEN_DOUBLE, 0, rhsResult.doubleValue);
        } else {
            parseError(L"Invalid right-hand side in assignment");
        }
    }
    else if(assignmentType == TOKEN_MULTIPLY_BY) {
        if (rhsResult.type == TOKEN_INT) {
            parseMultiplyBy(varName, TOKEN_INT, rhsResult.intValue, 0);
        } else if (rhsResult.type == TOKEN_DOUBLE) {
            parseMultiplyBy(varName, TOKEN_DOUBLE, 0, rhsResult.doubleValue);
        } else {
            parseError(L"Invalid right-hand side in assignment");
        }
    }
    else if(assignmentType == TOKEN_MOD_BY) {
        if (rhsResult.type == TOKEN_INT) {
            parseModBy(varName, TOKEN_INT, rhsResult.intValue, 0);
        } else if (rhsResult.type == TOKEN_DOUBLE) {
            parseModBy(varName, TOKEN_DOUBLE, 0, rhsResult.doubleValue);
        } else {
            parseError(L"Invalid right-hand side in assignment");
        }
    }
    else {
        parseError(L"Expected assignment operator");
    }

    free(varName); // Clean up the allocated variable name
    expect(TOKEN_SEMICOLON); // Expect a semicolon at the end of the assignment
}

void parseStatement() {
    switch (currentToken.type) {
        case TOKEN_VARIABLE:
            parseAssignment();  // Handle variable assignment
            break;
        /*
        case TOKEN_FOR:
            parseForStatement();  // Handle for loop
            break;
        case TOKEN_IF:
            parseIfStatement();  // Handle if statement
            break;
        case TOKEN_WHILE:
            parseWhileStatement();  // Handle while loop
            break; 
        */
        case TOKEN_PRINT:
            parsePrintStatement();  // Handle print statement
            break;
        /*
        case TOKEN_RETURN:
            parseReturnStatement();  // Handle return statement
            break;
        */
        default:
            parseError(L"Unexpected token in statement");
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



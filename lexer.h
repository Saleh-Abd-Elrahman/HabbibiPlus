#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_INT, 
    TOKEN_FLOAT,
    TOKEN_PLUS, 
    TOKEN_MINUS,  
    TOKEN_STAR, 
    TOKEN_SLASH, 
    TOKEN_LPAREN,   
    TOKEN_RPAREN,  
    TOKEN_VARIABLE, // General variable token type
    TOKEN_EOF,  
    TOKEN_FOR,  
    TOKEN_IF, 
    TOKEN_ELSE, 
    TOKEN_WHILE, 
    TOKEN_CHAR,
    TOKEN_RETURN,
    TOKEN_MODULUS, 
    TOKEN_EXPONENT, 
    TOKEN_EQUAL_TO, 
    TOKEN_LESS_THAN, 
    TOKEN_GREATER_THAN, 
    TOKEN_AND, 
    TOKEN_OR, 
    TOKEN_DOUBLE_QUOTE, // replaced by this
    TOKEN_INCREMENT_BY,
    TOKEN_MULTIPLY_BY,
    TOKEN_DECREASE_BY,
    TOKEN_DIVIDE_BY,
    TOKEN_MOD_BY,
    TOKEN_NOT_EQUAL_TO,
    TOKEN_LESS_THAN_OR_EQUAL_TO,
    TOKEN_GREATER_THAN_OR_EQUAL_TO,
    TOKEN_COMMA,
    TOKEN_SEMICOLON, 
    TOKEN_PERIOD,
    TOKEN_COLON, 
    TOKEN_QUESTION_MARK, 
    TOKEN_EXCLAMATION_MARK, 
    TOKEN_LEFT_BRACKET, 
    TOKEN_RIGHT_BRACKET, 
    TOKEN_COMMENT, 
    TOKEN_PRINT,
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    union {
        int intValue;    // For TOKEN_INT
        float floatValue; // For TOKEN_FLOAT
        char* varName;    // For TOKEN_VARIABLE
    };
} Token;


Token *tokenize(char *source);
void printToken(Token token);

#endif // LEXER_H
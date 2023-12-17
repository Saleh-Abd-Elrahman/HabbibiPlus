while (currentToken.type == TOKEN_PLUS) {
            nextToken(); // Consume the '+' token
            int nextValue = parseInteger();
            result += nextValue;
            break;
        }

        while (currentToken.type == TOKEN_MINUS) {
            nextToken(); 
            int nextValue = parseInteger();
            result -= nextValue;
            break;
        }

        while (currentToken.type == TOKEN_STAR) {
            nextToken(); 
            int nextValue = parseInteger();
            result *= nextValue;
            break;
        }

        while (currentToken.type == TOKEN_SLASH) {
            nextToken(); 
            int nextValue = parseInteger();
            result /= nextValue;
            break;
        }



while (currentToken.type != TOKEN_SEMICOLON){

        if (currentToken.type == TOKEN_PLUS) {
            nextToken(); // Consume the '+' token
            int nextValue = parseInteger();
            result += nextValue;
            nextToken();
            break;
        }

        if (currentToken.type == TOKEN_MINUS) {
            nextToken(); 
            int nextValue = parseInteger();
            result -= nextValue;
            nextToken();
            break;
        }

        if (currentToken.type == TOKEN_STAR) {
            nextToken(); 
            int nextValue = parseInteger();
            result *= nextValue;
            nextToken();
            break;
        }

        if (currentToken.type == TOKEN_SLASH) {
            nextToken(); 
            int nextValue = parseInteger();
            result /= nextValue;
            nextToken();
            break;
        }
    }



    return result;


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

    while (currentToken.type == TOKEN_STAR || currentToken.type == TOKEN_SLASH || currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS){
        TokenType operatorType = currentToken.type;
        nextToken();

        int nextValue = parseFactor();

        if (operatorType == TOKEN_STAR) {
            result *= nextValue;
        } else if (operatorType == TOKEN_PLUS) {
            result += nextValue;
        } else if (operatorType == TOKEN_MINUS) {
            result -= nextValue;
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

    while (currentToken.type == TOKEN_STAR || currentToken.type == TOKEN_SLASH || currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        TokenType operatorType = currentToken.type;
        nextToken(); // Consume the operator token

        int nextValue = parseTerm();

        if (operatorType == TOKEN_STAR) {
            result *= nextValue;
        } else if (operatorType == TOKEN_PLUS) {
            result += nextValue;
        } else if (operatorType == TOKEN_MINUS) {
            result -= nextValue;
        } else if (operatorType == TOKEN_SLASH) {
            if (nextValue == 0) {
                parseError("Division by zero");
            }
            result /= nextValue;
        }
    }

    return result;
}


    int result = parseExpression();
    printf("Result: %d\n", result);
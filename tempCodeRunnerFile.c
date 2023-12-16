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
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "lexer.h" 
#include "lexer.c"// Assuming your lexer code is in lexer.h and lexer.c

int main() {
    setlocale(LC_CTYPE, "");
    wchar_t input[100] = L"إذا"; // Use wide character string
    Token *tokens = tokenize(input);


    printf("Tokens:\n");
    for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
        printToken(tokens[i]);
        printf("\n");
    }


    // Free allocated memory for tokens (don't forget to free memory after usage)
    free(tokens);

    return 0;
}
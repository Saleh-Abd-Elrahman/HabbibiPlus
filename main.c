#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "lexer.h" 
#include "lexer.c"// Assuming your lexer code is in lexer.h and lexer.c

int main() {
    setlocale(LC_CTYPE, "");
<<<<<<< HEAD
    wchar_t input[100] = L"إذا"; // Use wide character string
=======
    wchar_t input[20] = L"="; // Use wide character string
>>>>>>> 230cb23fc6026a93a61d00209522e13285956df0
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
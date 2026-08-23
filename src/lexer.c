#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"

Token **tokenize(char *input, int *count) {
    Token **tokens = NULL;
    int size = 0;

    char *word = strtok(input, " \t\n");

    while (word != NULL) {
        tokens = realloc(tokens, sizeof(Token *) * (size + 1));

        tokens[size] = create_token(WORD, word);
        size++;

        word = strtok(NULL, " \t\n");
    }

    tokens = realloc(tokens, sizeof(Token *) * (size + 1));
    tokens[size] = create_token(END, "END");
    size++;

    *count = size;
    return tokens;
}

void free_tokens(Token **tokens, int count) {
    for (int i = 0; i < count; i++) {
        free_token(tokens[i]);
    }

    free(tokens);
}

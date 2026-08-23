#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token.h"

Token *create_token(TokenType type, char *value) {
    Token *token = malloc(sizeof(Token));

    token->type = type;

    if (value) {
        token->value = malloc(strlen(value) + 1);
        strcpy(token->value, value);
    } else {
        token->value = NULL;
    }

    return token;
}

void free_token(Token *token) {
    if (!token)
        return;

    free(token->value);
    free(token);
}

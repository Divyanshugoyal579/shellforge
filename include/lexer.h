#ifndef LEXER_H
#define LEXER_H

#include "token.h"

Token *tokenize(const char *input, int *count);

void free_tokens(Token *tokens, int count);

#endif

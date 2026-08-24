#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct {
    char *argv[64];
    int argc;
    char *input;
    char *output;
    int append;
    int background;
} Command;

void parse(Token *tokens, Command *cmd);
void print_pipeline(Command *cmd);

#endif

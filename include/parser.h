#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define MAX_COMMANDS 20
#define MAX_ARGS 100

typedef struct {
    char *argv[MAX_ARGS];
    int argc;

    char *input_file;
    char *output_file;

    int append;
    int background;
} Command;

typedef struct {
    Command commands[MAX_COMMANDS];
    int count;
} Pipeline;

int parse_tokens(Token *tokens, int count, Pipeline *pipeline);

void free_pipeline(Pipeline *pipeline);

void print_pipeline(const Pipeline *pipeline);

#endif

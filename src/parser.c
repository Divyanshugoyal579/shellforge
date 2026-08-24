#include <stdio.h>
#include <string.h>
#include "parser.h"

void parse(Token *tokens, Command *cmd)
{
    cmd->argc = 0;
    cmd->input = NULL;
    cmd->output = NULL;
    cmd->append = 0;
    cmd->background = 0;

    for (int i = 0; tokens[i].type != END; i++)
    {
        if (tokens[i].type == WORD)
        {
            cmd->argv[cmd->argc++] = tokens[i].value;
        }
    }

    cmd->argv[cmd->argc] = NULL;
}

void print_pipeline(Command *cmd)
{
    printf("\n========== PIPELINE ==========\n\n");

    printf("Command 1\n");
    printf("------------------------------------\n");
    printf("Arguments\n");

    for (int i = 0; i < cmd->argc; i++)
        printf("argv[%d] = %s\n", i, cmd->argv[i]);

    printf("Input      : %s\n", cmd->input ? cmd->input : "None");
    printf("Output     : %s\n", cmd->output ? cmd->output : "None");
    printf("Append     : %s\n", cmd->append ? "Yes" : "No");
    printf("Background : %s\n", cmd->background ? "Yes" : "No");

    printf("====================================\n");
}

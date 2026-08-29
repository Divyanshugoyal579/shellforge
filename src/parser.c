#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static char *duplicate_string(const char *str)
{
    if (str == NULL)
        return NULL;

    char *copy = malloc(strlen(str) + 1);

    if (copy == NULL)
        return NULL;

    strcpy(copy, str);

    return copy;
}

static void initialize_command(Command *cmd)
{
    memset(cmd, 0, sizeof(Command));
}

int parse_tokens(Token *tokens, int count, Pipeline *pipeline)
{
    if (tokens == NULL || pipeline == NULL)
        return 0;

    memset(pipeline, 0, sizeof(Pipeline));

    pipeline->count = 1;
    initialize_command(&pipeline->commands[0]);

    Command *cmd = &pipeline->commands[0];

    for (int i = 0; i < count; i++)
    {
        TokenType type = tokens[i].type;

        /* End of input */
        if (type == TOKEN_END)
        {
            break;
        }

        /* Normal word / argument */
        if (type == TOKEN_WORD)
        {
            if (cmd->argc >= MAX_ARGS - 1)
                return 0;

            cmd->argv[cmd->argc] =
                duplicate_string(tokens[i].value);

            if (cmd->argv[cmd->argc] == NULL)
                return 0;

            cmd->argc++;

            /* argv must end with NULL */
            cmd->argv[cmd->argc] = NULL;
        }

        /* Pipe: | */
        else if (type == TOKEN_PIPE)
        {
            if (cmd->argc == 0)
                return 0;

            if (pipeline->count >= MAX_COMMANDS)
                return 0;

            cmd = &pipeline->commands[pipeline->count];

            initialize_command(cmd);

            pipeline->count++;
        }

        /* Input redirection: < */
        else if (type == TOKEN_REDIRECT_IN)
        {
            if (i + 1 >= count ||
                tokens[i + 1].type != TOKEN_WORD)
            {
                return 0;
            }

            i++;

            cmd->input_file =
                duplicate_string(tokens[i].value);

            if (cmd->input_file == NULL)
                return 0;
        }

        /* Output redirection: > or >> */
        else if (type == TOKEN_REDIRECT_OUT ||
                 type == TOKEN_APPEND)
        {
            if (i + 1 >= count ||
                tokens[i + 1].type != TOKEN_WORD)
            {
                return 0;
            }

            i++;

            cmd->output_file =
                duplicate_string(tokens[i].value);

            if (cmd->output_file == NULL)
                return 0;

            if (type == TOKEN_APPEND)
                cmd->append = 1;
            else
                cmd->append = 0;
        }

        /* Background: & */
        else if (type == TOKEN_BACKGROUND)
        {
            cmd->background = 1;
        }
    }

    /* Last command cannot be empty */
    if (pipeline->commands[pipeline->count - 1].argc == 0)
        return 0;

    return 1;
}

void free_pipeline(Pipeline *pipeline)
{
    if (pipeline == NULL)
        return;

    for (int i = 0; i < pipeline->count; i++)
    {
        Command *cmd = &pipeline->commands[i];

        /* Free arguments */
        for (int j = 0; j < cmd->argc; j++)
        {
            free(cmd->argv[j]);
        }

        /* Free input/output files */
        free(cmd->input_file);
        free(cmd->output_file);
    }

    memset(pipeline, 0, sizeof(Pipeline));
}

void print_pipeline(const Pipeline *pipeline)
{
    printf("\n========== PIPELINE ==========\n\n");

    for (int i = 0; i < pipeline->count; i++)
    {
        const Command *cmd = &pipeline->commands[i];

        printf("Command %d\n", i + 1);
        printf("------------------------------\n");

        printf("Arguments\n");

        for (int j = 0; j < cmd->argc; j++)
        {
            printf("argv[%d] = %s\n",
                   j,
                   cmd->argv[j]);
        }

        if (cmd->input_file != NULL)
            printf("Input    : %s\n", cmd->input_file);
        else
            printf("Input    : None\n");

        if (cmd->output_file != NULL)
            printf("Output   : %s\n", cmd->output_file);
        else
            printf("Output   : None\n");

        printf("Append   : %s\n",
               cmd->append ? "Yes" : "No");

        printf("Background : %s\n",
               cmd->background ? "Yes" : "No");

        printf("==============================\n");
    }
}

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "expand.h"

/*
 * Expand environment variables.
 *
 * Examples:
 *   $HOME
 *   ${HOME}
 *   $USER
 *   $?
 */
static char *expand_word(const char *word)
{
    size_t capacity = 128;
    size_t length = 0;

    char *result = malloc(capacity);

    if (result == NULL)
        return NULL;

    size_t i = 0;

    while (word[i] != '\0')
    {
        /* Environment variable */
        if (word[i] == '$')
        {
            /* Special variable $? */
            if (word[i + 1] == '?')
            {
                const char *value = "0";
                size_t value_length = strlen(value);

                while (length + value_length + 1 >= capacity)
                {
                    capacity *= 2;

                    char *temp = realloc(result, capacity);

                    if (temp == NULL)
                    {
                        free(result);
                        return NULL;
                    }

                    result = temp;
                }

                memcpy(result + length,
                       value,
                       value_length);

                length += value_length;
                i += 2;

                continue;
            }

            /*
             * ${VARIABLE}
             */
            if (word[i + 1] == '{')
            {
                size_t start = i + 2;
                size_t j = start;

                while (word[j] != '\0' && word[j] != '}')
                    j++;

                if (word[j] == '}')
                {
                    size_t name_length = j - start;

                    char *name = malloc(name_length + 1);

                    if (name == NULL)
                    {
                        free(result);
                        return NULL;
                    }

                    memcpy(name,
                           word + start,
                           name_length);

                    name[name_length] = '\0';

                    const char *value = getenv(name);

                    if (value == NULL)
                        value = "";

                    size_t value_length = strlen(value);

                    while (length + value_length + 1 >= capacity)
                    {
                        capacity *= 2;

                        char *temp =
                            realloc(result, capacity);

                        if (temp == NULL)
                        {
                            free(name);
                            free(result);
                            return NULL;
                        }

                        result = temp;
                    }

                    memcpy(result + length,
                           value,
                           value_length);

                    length += value_length;

                    free(name);

                    i = j + 1;

                    continue;
                }
            }

            /*
             * $VARIABLE
             */
            if (isalpha((unsigned char)word[i + 1]) ||
                word[i + 1] == '_')
            {
                size_t start = i + 1;
                size_t j = start;

                while (isalnum((unsigned char)word[j]) ||
                       word[j] == '_')
                {
                    j++;
                }

                size_t name_length = j - start;

                char *name = malloc(name_length + 1);

                if (name == NULL)
                {
                    free(result);
                    return NULL;
                }

                memcpy(name,
                       word + start,
                       name_length);

                name[name_length] = '\0';

                const char *value = getenv(name);

                if (value == NULL)
                    value = "";

                size_t value_length = strlen(value);

                while (length + value_length + 1 >= capacity)
                {
                    capacity *= 2;

                    char *temp =
                        realloc(result, capacity);

                    if (temp == NULL)
                    {
                        free(name);
                        free(result);
                        return NULL;
                    }

                    result = temp;
                }

                memcpy(result + length,
                       value,
                       value_length);

                length += value_length;

                free(name);

                i = j;

                continue;
            }
        }

        /*
         * Normal character
         */
        if (length + 2 >= capacity)
        {
            capacity *= 2;

            char *temp = realloc(result, capacity);

            if (temp == NULL)
            {
                free(result);
                return NULL;
            }

            result = temp;
        }

        result[length] = word[i];

        length++;
        i++;
    }

    result[length] = '\0';

    return result;
}


/*
 * Expand all words, input files and output files
 * inside the pipeline.
 */
int expand_pipeline(Pipeline *pipeline)
{
    if (pipeline == NULL)
        return 0;

    for (int i = 0; i < pipeline->count; i++)
    {
        Command *cmd = &pipeline->commands[i];

        /*
         * Expand command arguments
         */
        for (int j = 0; j < cmd->argc; j++)
        {
            char *expanded =
                expand_word(cmd->argv[j]);

            if (expanded == NULL)
                return 0;

            free(cmd->argv[j]);

            cmd->argv[j] = expanded;
        }

        /*
         * Expand input file
         */
        if (cmd->input_file != NULL)
        {
            char *expanded =
                expand_word(cmd->input_file);

            if (expanded == NULL)
                return 0;

            free(cmd->input_file);

            cmd->input_file = expanded;
        }

        /*
         * Expand output file
         */
        if (cmd->output_file != NULL)
        {
            char *expanded =
                expand_word(cmd->output_file);

            if (expanded == NULL)
                return 0;

            free(cmd->output_file);

            cmd->output_file = expanded;
        }
    }

    return 1;
}

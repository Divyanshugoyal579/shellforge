#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

void lexer(char *input, Token tokens[])
{
    int i = 0;

    char *word = strtok(input, " \t\n");

    while (word != NULL && i < 127)
    {
        tokens[i].type = WORD;

        tokens[i].value = malloc(strlen(word) + 1);

        if (tokens[i].value == NULL)
        {
            printf("Memory allocation error\n");
            exit(1);
        }

        strcpy(tokens[i].value, word);

        i++;

        word = strtok(NULL, " \t\n");
    }

    tokens[i].type = END;
    tokens[i].value = NULL;
}

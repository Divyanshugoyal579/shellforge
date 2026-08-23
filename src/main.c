#include <stdio.h>
#include <string.h>
#include "../include/lexer.h"

int main() {
    char input[256];

    printf("================================\n");
    printf("           ShellForge\n");
    printf("    A Unix Style Shell written in C\n");
    printf("================================\n");

    while (1) {
        printf("shellforge$ ");

        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        if (strlen(input) == 0)
            continue;

        int count;
        Token **tokens = tokenize(input, &count);

        printf("\n------------ TOKENS ------------\n");

        for (int i = 0; i < count; i++) {
            if (tokens[i]->type == WORD)
                printf("%d : WORD\t%s\n", i, tokens[i]->value);
            else
                printf("%d : END\t%s\n", i, tokens[i]->value);
        }

        printf("--------------------------------\n");

        free_tokens(tokens, count);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>

int main(void)
{
    // Display a welcome banner when the shell starts
    printf("=====================================\n");
    printf("Shellforge\n");
    printf("A Unix Style Shell written in C\n");
    printf("=====================================\n");

    char *line;

    while (1)
    {
        // Display prompt and read input
        line = readline("shellforge$ ");

        // Handle Ctrl+D (EOF)
        if (line == NULL)
        {
            printf("\nGoodbye!\n");
            break;
        }

        // Ignore empty input
        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        // Save command in history
        add_history(line);

        // Exit command
        if (strcmp(line, "exit") == 0)
        {
            free(line);
            printf("Exiting...\n");
            break;
        }

        // Echo the entered command
        printf("YOU ENTERED: %s\n", line);

        // Free allocated memory
        free(line);
    }

    return 0;
}

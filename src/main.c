#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "token.h"
#include "parser.h"

int main()
{
    char input[1024];
    Token tokens[128] = {0};
    Command cmd = {0};

    printf("==============================\n");
    printf("          ShellForge\n");
    printf("   A Unix Style Shell written in C\n");
    printf("==============================\n");

    while (1)
    {
        printf("shellforge$ ");

        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0)
            break;

        if (strlen(input) == 0)
            continue;

        lexer(input, tokens);

        printf("\n------------- TOKENS -------------\n");

        int i = 0;

        while (i < 128 && tokens[i].type != END)
        {
            printf("%d : WORD\t%s\n", i, tokens[i].value);
            i++;
        }

        printf("%d : END\t\tEND\n", i);

        printf("----------------------------------\n");

        memset(&cmd, 0, sizeof(cmd));

        parse(tokens, &cmd);

        print_pipeline(&cmd);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "expand.h"
#include "builtin.h"
#include "execute.h"


/*
 * =========================================================
 * PRINT TOKENS
 * =========================================================
 */
static void print_tokens(Token *tokens, int count)
{
    printf("\n------------- TOKENS -------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d : %-12s %s\n",
               i,
               token_type_name(tokens[i].type),
               tokens[i].value);
    }

    printf("----------------------------------\n");
}


/*
 * =========================================================
 * MAIN
 * =========================================================
 */
int main(void)
{
    char *input;


    /*
     * ShellForge header
     */
    printf("=====================================\n");
    printf("           Shellforge\n");
    printf("     A Unix Style Shell written in C\n");
    printf("=====================================\n");


    /*
     * =====================================================
     * SHELL LOOP
     * =====================================================
     */
    while (1)
    {
        /*
         * Read command from user
         */
        input = readline("shellforge$ ");


        /*
         * Ctrl + D
         */
        if (input == NULL)
        {
            printf("\n");
            break;
        }


        /*
         * Ignore empty input
         */
        if (input[0] == '\0')
        {
            free(input);
            continue;
        }


        /*
         * Add command to history
         */
        add_history(input);


        /*
         * =================================================
         * LEXER
         * =================================================
         */
        int count = 0;

        Token *tokens = tokenize(input, &count);

        if (tokens == NULL)
        {
            fprintf(stderr, "Lexer error\n");

            free(input);

            continue;
        }


        /*
         * Display tokens
         */
        print_tokens(tokens, count);


        /*
         * =================================================
         * PARSER
         * =================================================
         */
        Pipeline pipeline;

        if (!parse_tokens(tokens, count, &pipeline))
        {
            fprintf(stderr,
                    "Parser error: invalid command\n");

            free_tokens(tokens, count);

            free(input);

            continue;
        }


        /*
         * =================================================
         * EXPAND
         * =================================================
         */
        if (!expand_pipeline(&pipeline))
        {
            fprintf(stderr,
                    "Expansion error\n");

            free_pipeline(&pipeline);

            free_tokens(tokens, count);

            free(input);

            continue;
        }


        /*
         * Display parsed pipeline
         */
        print_pipeline(&pipeline);


        /*
         * =================================================
         * EXECUTION
         * =================================================
         *
         * Currently execute one command.
         */
        if (pipeline.count == 1)
        {
            int result =
                execute_command(&pipeline.commands[0]);


            /*
             * execute_builtin("exit")
             * returns 1.
             */
            if (result == 1)
            {
                free_pipeline(&pipeline);

                free_tokens(tokens, count);

                free(input);

                break;
            }
        }


        /*
         * =================================================
         * CLEANUP
         * =================================================
         */
        free_pipeline(&pipeline);

        free_tokens(tokens, count);

        free(input);
    }


    return 0;
}


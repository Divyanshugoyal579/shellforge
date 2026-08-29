#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

#define MAX_TOKENS 100

Token *tokenize(const char *input, int *count) {

    Token *tokens = malloc(sizeof(Token) * MAX_TOKENS);

    if (tokens == NULL) {
        return NULL;
    }

    *count = 0;

    int i = 0;

    while (input[i] != '\0') {

        /* Ignore spaces */
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        /* Pipe */
        if (input[i] == '|') {

            tokens[*count].type = TOKEN_PIPE;
            tokens[*count].value = strdup("|");

            (*count)++;
            i++;

            continue;
        }

        /* Input redirection */
        if (input[i] == '<') {

            tokens[*count].type = TOKEN_REDIRECT_IN;
            tokens[*count].value = strdup("<");

            (*count)++;
            i++;

            continue;
        }

        /* Output redirection */
        if (input[i] == '>') {

            tokens[*count].type = TOKEN_REDIRECT_OUT;
            tokens[*count].value = strdup(">");

            (*count)++;
            i++;

            continue;
        }

        /*
         * Read a word.
         *
         * Quotes are removed:
         *
         * echo "hello"
         *
         * becomes:
         *
         * echo
         * hello
         */

        char word[1024];
        int j = 0;

        char quote = '\0';

        while (input[i] != '\0') {

            /* Start quote */
            if ((input[i] == '\'' || input[i] == '"')
                && quote == '\0') {

                quote = input[i];
                i++;
                continue;
            }

            /* End quote */
            if (input[i] == quote) {

                quote = '\0';
                i++;
                continue;
            }

            /* Stop word at whitespace outside quotes */
            if (isspace((unsigned char)input[i])
                && quote == '\0') {
                break;
            }

            /* Operators outside quotes */
            if (quote == '\0' &&
                (input[i] == '|' ||
                 input[i] == '<' ||
                 input[i] == '>')) {
                break;
            }

            /*
             * Escape character.
             *
             * Example:
             *
             * hello\ world
             *
             * becomes:
             *
             * hello world
             */

            if (input[i] == '\\' && input[i + 1] != '\0') {

                i++;

                word[j++] = input[i];

                i++;

                continue;
            }

            word[j++] = input[i];

            i++;

            if (j >= 1023) {
                break;
            }
        }

        word[j] = '\0';

        if (j > 0) {

            tokens[*count].type = TOKEN_WORD;
            tokens[*count].value = strdup(word);

            (*count)++;
        }

        if (*count >= MAX_TOKENS - 1) {
            break;
        }
    }

    return tokens;
}


void free_tokens(Token *tokens, int count) {

    for (int i = 0; i < count; i++) {
        free(tokens[i].value);
    }

    free(tokens);
}

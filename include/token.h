#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    WORD,
    END
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token *create_token(TokenType type, char *value);
void free_token(Token *token);

#endif

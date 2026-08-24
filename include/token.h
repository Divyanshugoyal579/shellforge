#ifndef TOKEN_H
#define TOKEN_H

typedef enum
{
    WORD,
    END
} TokenType;

typedef struct
{
    TokenType type;
    char *value;
} Token;

#endif

#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_FUNCTION,
    TOKEN_VARIABLE,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN
} TokenType;

typedef struct {
    TokenType type;
    char value[50];
} Token;
#endif
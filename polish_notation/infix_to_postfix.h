#ifndef INFIX_TO_POSTFIX_H
#define INFIX_TO_POSTFIX_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"

int precedence(const char *op);

int is_function_name(const char *s);

void ensure_capacity(Token **arr, int *cap, int need);

void append_token(Token **out, int *n, int *cap, TokenType type, const char *value);

void push_operator(Stack *opstack, Token **out, int *n, int *cap, const char *op);

void pop_until_left_paren(Stack *opstack, Token **out, int *n, int *cap);

void handle_unary_minus(Stack *opstack, Token **out, int *n, int *cap);

void infix_to_postfix(const Token *infix_tokens, int num_infix_tokens, Token **postfix_tokens,
                      int *num_postfix_tokens);

#endif
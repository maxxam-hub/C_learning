#ifndef VALIDATE_EXPRESSION_H
#define VALIDATE_EXPRESSION_H

#include "token.h"

int validate_expression(const char *expr);

int validate_tokens(const Token *toks, int n);

#endif
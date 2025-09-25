#ifndef CALCULATE_POSTFIX_H
#define CALCULATE_POSTFIX_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"

void push_double(Stack *s, double v);
double pop_double(Stack *s);
void function_variable(const Token *current_token, Stack *operand_stack, double x);
void function_operator(const Token *current_token, Stack *operand_stack);
void function_function(const Token *current_token, Stack *operand_stack);
double calculate_postfix(const Token *postfix_tokens, int num_postfix_tokens, double x);

#endif
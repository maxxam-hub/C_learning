#include "calculate_postfix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void push_double(Stack *s, double v) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%.15g", v);
    stack_push(s, buf);
}

double pop_double(Stack *s) {
    double v = atof(stack_top(s));
    free(stack_pop(s));
    return v;
}

void function_variable(const Token *current_token, Stack *operand_stack, double x) {
    push_double(operand_stack, (current_token->value[0] == '-') ? -x : x);
}

void function_operator(const Token *current_token, Stack *operand_stack) {
    double b = pop_double(operand_stack);
    double a = pop_double(operand_stack);
    double r = 0.0;

    if (strcmp(current_token->value, "+") == 0)
        r = a + b;
    else if (strcmp(current_token->value, "-") == 0)
        r = a - b;
    else if (strcmp(current_token->value, "*") == 0)
        r = a * b;
    else if (strcmp(current_token->value, "/") == 0)
        r = a / b;

    push_double(operand_stack, r);
}

void function_function(const Token *current_token, Stack *operand_stack) {
    double a = pop_double(operand_stack);
    double r = 0.0;

    if (strcmp(current_token->value, "sin") == 0)
        r = sin(a);
    else if (strcmp(current_token->value, "cos") == 0)
        r = cos(a);
    else if (strcmp(current_token->value, "tan") == 0)
        r = tan(a);
    else if (strcmp(current_token->value, "ctg") == 0)
        r = 1.0 / tan(a);
    else if (strcmp(current_token->value, "sqrt") == 0)
        r = sqrt(a);
    else if (strcmp(current_token->value, "ln") == 0)
        r = log(a);

    push_double(operand_stack, r);
}

double calculate_postfix(const Token *postfix_tokens, int num_postfix_tokens, double x) {
    Stack s;
    stack_init(&s, num_postfix_tokens);

    for (int i = 0; i < num_postfix_tokens; i++) {
        const Token *t = &postfix_tokens[i];

        if (t->type == TOKEN_NUMBER)
            stack_push(&s, t->value);
        else if (t->type == TOKEN_VARIABLE)
            function_variable(t, &s, x);
        else if (t->type == TOKEN_OPERATOR)
            function_operator(t, &s);
        else if (t->type == TOKEN_FUNCTION)
            function_function(t, &s);
    }

    double result = pop_double(&s);
    stack_free(&s);
    return result;
}
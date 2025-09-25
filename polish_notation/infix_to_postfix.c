#include "infix_to_postfix.h"

int precedence(const char *op) {
    int priority = 0;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) priority = 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) priority = 2;
    if (strcmp(op, "sin") == 0 || strcmp(op, "cos") == 0 || strcmp(op, "tan") == 0 ||
        strcmp(op, "ctg") == 0 || strcmp(op, "sqrt") == 0 || strcmp(op, "ln") == 0)
        priority = 3;
    return priority;
}

int is_function_name(const char *s) {
    return strcmp(s, "sin") == 0 || strcmp(s, "cos") == 0 || strcmp(s, "tan") == 0 || strcmp(s, "ctg") == 0 ||
           strcmp(s, "sqrt") == 0 || strcmp(s, "ln") == 0;
}

void ensure_capacity(Token **arr, int *cap, int need) {
    if (*cap >= need) return;
    int new_cap = (*cap == 0) ? 16 : *cap;
    while (new_cap < need) new_cap <<= 1;
    *arr = (Token *)realloc(*arr, sizeof(Token) * new_cap);
    *cap = new_cap;
}

void append_token(Token **out, int *n, int *cap, TokenType type, const char *value) {
    ensure_capacity(out, cap, *n + 1);
    (*out)[*n].type = type;
    strncpy((*out)[*n].value, value, sizeof((*out)[*n].value) - 1);
    (*out)[*n].value[sizeof((*out)[*n].value) - 1] = '\0';
    (*n)++;
}

void push_operator(Stack *opstack, Token **out, int *n, int *cap, const char *op) {
    while (!stack_empty(opstack) && strcmp(stack_top(opstack), "(") != 0 &&
           precedence(stack_top(opstack)) >= precedence(op)) {
        char *tok = stack_pop(opstack);
        append_token(out, n, cap, is_function_name(tok) ? TOKEN_FUNCTION : TOKEN_OPERATOR, tok);
        free(tok);
    }
    stack_push(opstack, op);
}

void pop_until_left_paren(Stack *opstack, Token **out, int *n, int *cap) {
    while (!stack_empty(opstack) && strcmp(stack_top(opstack), "(") != 0) {
        char *tok = stack_pop(opstack);
        append_token(out, n, cap, is_function_name(tok) ? TOKEN_FUNCTION : TOKEN_OPERATOR, tok);
        free(tok);
    }
    if (!stack_empty(opstack) && strcmp(stack_top(opstack), "(") == 0) {
        free(stack_pop(opstack));
    }
    if (!stack_empty(opstack) && is_function_name(stack_top(opstack))) {
        char *fn = stack_pop(opstack);
        append_token(out, n, cap, TOKEN_FUNCTION, fn);
        free(fn);
    }
}

void handle_unary_minus(Stack *opstack, Token **out, int *n, int *cap) {
    append_token(out, n, cap, TOKEN_NUMBER, "-1");
    push_operator(opstack, out, n, cap, "*");
}

void infix_to_postfix(const Token *infix_tokens, int num_infix_tokens, Token **postfix_tokens,
                      int *num_postfix_tokens) {
    *postfix_tokens = NULL;
    *num_postfix_tokens = 0;

    int out_cap = 0;
    Stack opstack;
    stack_init(&opstack, num_infix_tokens + 16);

    for (int i = 0; i < num_infix_tokens; ++i) {
        const Token *t = &infix_tokens[i];
        TokenType ty = t->type;

        if (ty == TOKEN_NUMBER || ty == TOKEN_VARIABLE) {
            append_token(postfix_tokens, num_postfix_tokens, &out_cap, ty, t->value);

        } else if (ty == TOKEN_FUNCTION) {
            stack_push(&opstack, t->value);

        } else if (ty == TOKEN_LEFT_PAREN) {
            stack_push(&opstack, t->value);

        } else if (ty == TOKEN_RIGHT_PAREN) {
            pop_until_left_paren(&opstack, postfix_tokens, num_postfix_tokens, &out_cap);

        } else if (ty == TOKEN_OPERATOR) {
            if (t->value[0] == '#') {
                handle_unary_minus(&opstack, postfix_tokens, num_postfix_tokens, &out_cap);
            } else {
                push_operator(&opstack, postfix_tokens, num_postfix_tokens, &out_cap, t->value);
            }
        }
    }
    while (!stack_empty(&opstack)) {
        char *tok = stack_pop(&opstack);
        append_token(postfix_tokens, num_postfix_tokens, &out_cap,
                     is_function_name(tok) ? TOKEN_FUNCTION : TOKEN_OPERATOR, tok);
        free(tok);
    }
    stack_free(&opstack);
}
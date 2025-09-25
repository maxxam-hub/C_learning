#include "validate_expression.h"

#include <string.h>

#include "infix_to_postfix.h"
#include "parse_expression.h"

int ve_is_legal_char(char c) {
    return (c == ' ' || c == '.' || c == '+' || c == '*' || c == '-' || c == '(' || c == ')' || c == '/' ||
            (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int ve_raw_lex_ok(const char *expr) {
    if (!expr || !*expr) return 0;
    for (int i = 0; expr[i] != '\0'; ++i) {
        if (!ve_is_legal_char(expr[i])) return 0;
    }
    return 1;
}

int validate_tokens(const Token *t, int n) {
    if (!t || n <= 0) return 0;
    int balance = 0;
    int expect_operand = 1;
    for (int i = 0; i < n; ++i) {
        TokenType ty = t[i].type;
        if (ty == TOKEN_LEFT_PAREN) {
            balance++;
            if (i + 1 < n && t[i + 1].type == TOKEN_RIGHT_PAREN) return 0;
            expect_operand = 1;
            continue;
        }
        if (ty == TOKEN_RIGHT_PAREN) {
            balance--;
            if (balance < 0) return 0;
            expect_operand = 0;
            continue;
        }
        if (ty == TOKEN_FUNCTION) {
            if (i + 1 >= n || t[i + 1].type != TOKEN_LEFT_PAREN) return 0;
            expect_operand = 1;
            continue;
        }
        if (ty == TOKEN_OPERATOR) {
            if (t[i].value[0] == '#') {
                if (!expect_operand) return 0;
                expect_operand = 1;
            } else {
                if (expect_operand) return 0;
                expect_operand = 1;
            }
            continue;
        }
        if (ty == TOKEN_NUMBER || ty == TOKEN_VARIABLE) {
            if (!expect_operand) return 0;
            expect_operand = 0;
            continue;
        }
        return 0;
    }
    if (expect_operand) return 0;
    if (balance != 0) return 0;
    return 1;
}

int validate_expression(const char *expr) {
    if (!ve_raw_lex_ok(expr)) return 0;

    Token *toks = NULL;
    int ntoks = 0;
    parse_expression(expr, &toks, &ntoks);

    int ok = validate_tokens(toks, ntoks);

    free(toks);
    return ok;
}
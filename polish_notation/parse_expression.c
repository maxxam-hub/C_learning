#include "parse_expression.h"

void append_token_parsing(Token **tokens, int *num_tokens, TokenType type, const char *value) {
    *tokens = (Token *)realloc(*tokens, sizeof(Token) * (*num_tokens + 1));
    (*tokens)[*num_tokens].type = type;
    strncpy((*tokens)[*num_tokens].value, value, sizeof((*tokens)[*num_tokens].value) - 1);
    (*tokens)[*num_tokens].value[sizeof((*tokens)[*num_tokens].value) - 1] = '\0';
    (*num_tokens)++;
}
void append_char_token(Token **tokens, int *num_tokens, TokenType type, char ch) {
    char buf[2] = {ch, '\0'};
    append_token_parsing(tokens, num_tokens, type, buf);
}

int is_operator(char c) { return c == '+' || c == '*' || c == '-' || c == '/'; }
int my_isdigit(char c) { return (c >= '0' && c <= '9'); }
int my_isalpha(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }

void processing_numbers(const char *expression, Token **tokens, int *num_tokens, int *i) {
    char num[50] = {'\0'};
    int j = 0, dot_cnt = 0;
    while (my_isdigit(expression[*i]) || (expression[*i] == '.' && dot_cnt == 0)) {
        if (expression[*i] == '.') dot_cnt++;
        if (j < (int)sizeof(num) - 1) num[j++] = expression[*i];
        (*i)++;
    }
    num[j] = '\0';
    append_token_parsing(tokens, num_tokens, TOKEN_NUMBER, num);
}
void unary_minus(Token **tokens, int *num_tokens, int *i) {
    append_char_token(tokens, num_tokens, TOKEN_OPERATOR, '#');
    (*i)++;
}
void processing_operators(const char *expression, Token **tokens, int *num_tokens, int *i) {
    append_char_token(tokens, num_tokens, TOKEN_OPERATOR, expression[*i]);
    (*i)++;
}
void processing_the_left_bracket(Token **tokens, int *num_tokens, int *i) {
    append_char_token(tokens, num_tokens, TOKEN_LEFT_PAREN, '(');
    (*i)++;
}
void processing_the_right_bracket(Token **tokens, int *num_tokens, int *i) {
    append_char_token(tokens, num_tokens, TOKEN_RIGHT_PAREN, ')');
    (*i)++;
}
void processing_functions_or_variables(const char *expression, Token **tokens, int *num_tokens, int *i) {
    char ident[50] = {'\0'};
    int j = 0;
    while (my_isalpha(expression[*i])) {
        if (j < (int)sizeof(ident) - 1) ident[j++] = expression[*i];
        (*i)++;
    }
    ident[j] = '\0';

    if (strcmp(ident, "sin") == 0 || strcmp(ident, "cos") == 0 || strcmp(ident, "tan") == 0 ||
        strcmp(ident, "ctg") == 0 || strcmp(ident, "sqrt") == 0 || strcmp(ident, "ln") == 0) {
        append_token_parsing(tokens, num_tokens, TOKEN_FUNCTION, ident);
    } else {
        append_token_parsing(tokens, num_tokens, TOKEN_VARIABLE, ident);
    }
}

void parse_expression(const char *expression, Token **tokens, int *num_tokens) {
    *tokens = NULL;
    *num_tokens = 0;

    for (int i = 0; expression[i] != '\0';) {
        char c = expression[i];
        int advanced = 0;

        if (c == ' ') {
            i++;
            advanced = 1;
        } else if (my_isdigit(c)) {
            processing_numbers(expression, tokens, num_tokens, &i);
            advanced = 1;
        } else if (c == '-' && (i == 0 || strchr("(+-*/", expression[i - 1]) != NULL)) {
            unary_minus(tokens, num_tokens, &i);
            advanced = 1;
        } else if (is_operator(c)) {
            processing_operators(expression, tokens, num_tokens, &i);
            advanced = 1;
        } else if (c == '(') {
            processing_the_left_bracket(tokens, num_tokens, &i);
            advanced = 1;
        } else if (c == ')') {
            processing_the_right_bracket(tokens, num_tokens, &i);
            advanced = 1;
        } else if (my_isalpha(c)) {
            processing_functions_or_variables(expression, tokens, num_tokens, &i);
            advanced = 1;
        }
        if (!advanced) i++;
    }
}
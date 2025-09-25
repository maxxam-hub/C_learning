#include "draw_plot.h"
#include "infix_to_postfix.h"
#include "parse_expression.h"
#include "validate_expression.h"

int main() {
    char expression[100];

    printf("Input expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';
    if (validate_expression(expression)) {
        Token *infix_tokens = NULL;
        int num_infix_tokens = 0;
        parse_expression(expression, &infix_tokens, &num_infix_tokens);
        Token *postfix_tokens = NULL;
        int num_postfix_tokens = 0;
        infix_to_postfix(infix_tokens, num_infix_tokens, &postfix_tokens, &num_postfix_tokens);

        int graphic_matrix[HEIGHT][WIDTH];
        fill_graph(graphic_matrix);
        create_graphic(graphic_matrix, postfix_tokens, num_postfix_tokens);
        print_graph(graphic_matrix);

        free(infix_tokens);
        free(postfix_tokens);
    } else {
        printf("n/a");
    }

    return 0;
}
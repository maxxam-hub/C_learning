#include "draw_plot.h"

void fill_graph(int matrix[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            matrix[i][j] = void_sym;
        }
    }
}

void create_graphic(int matrix[HEIGHT][WIDTH], const Token *postfix_tokens, int num_postfix_tokens) {
    for (int i = 0; i < WIDTH; i++) {
        double x = (i * 4.0 * M_PI) / (WIDTH - 1);
        double res = calculate_postfix(postfix_tokens, num_postfix_tokens, x);
        int y = round((res + 1) * (HEIGHT - 1) / 2);
        if (y >= 0 && y < HEIGHT) {
            matrix[y][i] = sym;
        }
    }
}

void print_graph(int matrix[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(matrix[i][j]);
        }
        putchar('\n');
    }
}
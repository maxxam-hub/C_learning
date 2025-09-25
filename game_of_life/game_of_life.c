#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

void draw(int** gen, WINDOW* win, int height, int width);
int** read_from_file(int height, int width);
int** init_gen(int height, int width);
void free_matrix(int** matrix, int height);
void tick_life(int** old_generation, int** new_generation, int height, int width);
int sum_lifes(int** matrix, int centre_i, int centre_j, int height, int width);
int action(int sleep_time_max, int sleep_time_min, int* sleep_time, int key);
void parse_line(const char* line, int* row, int width);

int main() {
    const int rows = 25, cols = 80, sleep_time_max = 50000, sleep_time_min = 100;
    int sleep_time = 1000;

    int** current_gen = read_from_file(rows, cols);
    int** next_generation = init_gen(rows, cols);
    (void)freopen("/dev/tty", "r", stdin);

    initscr();
    start_color(), init_pair(1, COLOR_WHITE, COLOR_BLACK), init_pair(2, COLOR_BLACK, COLOR_WHITE);

    WINDOW* field = newwin(rows + 2, cols + 2, 0, 0);
    cbreak(), noecho(), nodelay(field, TRUE);
    curs_set(0), box(field, 0, 0), wrefresh(field);

    int exit = 0;
    while (!exit && current_gen && next_generation) {
        int current_char = wgetch(field);
        exit = action(sleep_time_max, sleep_time_min, &sleep_time, current_char);

        draw(current_gen, field, rows, cols);
        tick_life(current_gen, next_generation, rows, cols);

        int** temp = current_gen;
        current_gen = next_generation;
        next_generation = temp;

        napms(sleep_time);
    }

    if (current_gen) free_matrix(current_gen, rows);
    if (next_generation) free_matrix(next_generation, rows);
    endwin();
    return 0;
}

void draw(int** gen, WINDOW* win, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mvwaddch(win, i + 1, j + 1, gen[i][j] ? ('o' | COLOR_PAIR(2) | A_BOLD) : ' ');
        }
    }
    wrefresh(win);
}

int** init_gen(int height, int width) {
    int valid = 1;
    int** matrix = calloc(height, sizeof(int*));
    if (matrix) {
        for (int i = 0; i < height && valid; i++) {
            matrix[i] = calloc(width, sizeof(int));
            if (!matrix[i]) {
                for (int k = 0; k < i; k++) free(matrix[k]);
                free(matrix);
                valid = 0;
            }
        }
    }
    return valid ? matrix : NULL;
}

void free_matrix(int** matrix, int height) {
    for (int i = 0; i < height; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int** read_from_file(int height, int width) {
    int** matrix = init_gen(height, width);
    if (!matrix) return NULL;

    char line[90];

    for (int i = 0; i < height; i++) {
        if (fgets(line, sizeof(line), stdin) != NULL) {
            parse_line(line, matrix[i], width);
        } else {
            for (int j = 0; j < width; j++) {
                matrix[i][j] = 0;
            }
        }
    }

    return matrix;
}

int sum_lifes(int** matrix, int centre_i, int centre_j, int height, int width) {
    int sum = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

            int ni = (centre_i + i + height) % height;
            int nj = (centre_j + j + width) % width;

            sum += matrix[ni][nj];
        }
    }

    return sum;
}

void tick_life(int** old_generation, int** new_generation, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int sum = sum_lifes(old_generation, i, j, height, width);

            if (old_generation[i][j] == 1) {
                new_generation[i][j] = (sum == 2 || sum == 3) ? 1 : 0;
            } else {
                new_generation[i][j] = (sum == 3) ? 1 : 0;
            }
        }
    }
}

int action(int sleep_time_max, int sleep_time_min, int* sleep_time, int key) {
    int exit = (key == ' ') ? 1 : 0;

    if (!exit) {
        if ((key == 'A' || key == 'a') && *sleep_time > sleep_time_min) {
            *sleep_time -= 100;
        }
        if ((key == 'Z' || key == 'z') && *sleep_time < sleep_time_max) {
            *sleep_time += 200;
        }
    }
    return exit;
}

void parse_line(const char* line, int* row, int width) {
    int j = 0;
    int finished = 0;

    while (j < width && !finished) {
        char c = line[j];
        if (c == '\n' || c == '\0') {
            finished = 1;
        } else if (c == '*' || c == 'X' || c == 'o' || c == '1') {
            row[j] = 1;
        } else {
            row[j] = 0;
        }
        j++;
    }

    while (j < width) {
        row[j] = 0;
        j++;
    }
}
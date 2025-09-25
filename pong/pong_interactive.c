#include <ncurses.h>
#include <stdio.h>
/// Функция отрисовки ракетки
int draw_rocket(const int x, const int y, const int p_x, const int p_y);

// Функция отрисовки границ
int draw_border(const int axis, const int border);

// Функция отрисовки мяча
int draw_ball(const int x, const int y, const int b_x, const int b_y);

// Функция отрисовки счёта игроков
int draw_score(const int width, const int height, const int x, const int y,
               const int p1_score, const int p2_score);

// Функция отрисовки центральной вертикальной линии
int draw_middle_line(const int x, const int y, const int width,
                     const int height);

// Функция отрисовки всего игрового поля
void draw_game(const int width, const int height, const int p1_x,
               const int p1_y, const int p1_score, const int p2_x,
               const int p2_y, const int p2_score, const int b_x,
               const int b_y);

// Функция проверки допустимости вводимого символа
int input_check(const char ch);

// Функция ожидания и обработки ввода клавиш игрока
char input_handle(void);

// Основная логика игры начинается после определения всех функций

int main() {
  // Инициализация ncurses
  initscr();
  cbreak();  // Отключение буферизации ввода
  noecho();  // Отключение отображения введенных символов
  nodelay(stdscr, TRUE);  // Неблокирующий ввод
  curs_set(0);

  int w = 80, h = 25;
  int ball_x = w / 2, ball_y = h / 2;
  int vector_x = -1, vector_y = 1;
  int p1_x = 2, p1_y = h / 2, p1_score = 0;
  int p2_x = w - 2, p2_y = h / 2, p2_score = 0;
  int win_cond = 21;
  while (p1_score < win_cond && p2_score < win_cond) {
    napms(100);
    // Отрисовка
    draw_game(w, h, p1_x, p1_y, p1_score, p2_x, p2_y, p2_score, ball_x, ball_y);

    // Логика мяча
    if (ball_y == 1 || ball_y == h - 1) vector_y = -vector_y;

    if (ball_x == p1_x + 1 && (ball_y >= p1_y - 1 && ball_y <= p1_y + 1))
      vector_x = -vector_x;

    if (ball_x == p2_x - 1 && (ball_y >= p2_y - 1 && ball_y <= p2_y + 1))
      vector_x = -vector_x;

    ball_x += vector_x;
    ball_y += vector_y;

    // Логика счета
    if (ball_x < 2) {
      p2_score++;
      ball_x = w / 2;
      ball_y = h / 2;
    } else if (ball_x > w - 1) {
      p1_score++;
      ball_x = w / 2;
      ball_y = h / 2;
    }

    // Проверка победы
    if (p1_score >= 21 || p2_score >= 21) {
      if (p1_score >= 21)
        mvprintw(h / 2, w / 2 + 3 - 10, "Player one wins!");
      else
        mvprintw(h / 2, w / 2 + 3 - 10, "Player two wins!");
      refresh();
      getch();
      napms(1500);
    }

    // Обработка ввода
    int key = getch();
    if (key == 'a' && p1_y + 2 < h)
      p1_y++;
    else if (key == 'z' && p1_y - 2 > 0)
      p1_y--;
    else if (key == 'k' && p2_y + 2 < h)
      p2_y++;
    else if (key == 'm' && p2_y - 2 > 0)
      p2_y--;
    else if (key == 'q')
      p1_score = 22;
  }

  endwin();
  return 0;
}

// Функция рисования ракетки на экране
int draw_rocket(const int x, const int y, const int p_x, const int p_y) {
  int result = 0;
  // Проверяет, находится ли точка в пределах области ракетки
  if ((p_x == x) && ((p_y - y >= -1) && (p_y - y <= 1))) {
    printw("|");  // Отображается символ ракетки
    result = 1;
  }
  return result;
}

// Функция рисования горизонтальных границ игрового поля
int draw_border(const int axis, const int border) {
  int result = 0;
  // Горизонтальные стены отображаются символом "#"
  if (axis == 0 || axis == border) {
    printw("#");
    result = 1;
  }
  return result;
}

// Функция рисования мяча на экране
int draw_ball(const int x, const int y, const int b_x, const int b_y) {
  int result = 0;
  // Печатает символ "*", если координата соответствует положению мяча
  if ((x == b_x) && (y == b_y)) {
    printw("*");
    result = 1;
  }
  return result;
}

// Функция рисования счётчика игроков
int draw_score(const int width, const int height, const int x, const int y,
               const int p1_score, const int p2_score) {
  int result = 0;
  // Выводит цифры счёта игроков в соответствующих местах на экране
  if ((y == height - 2) && (x == 15)) {
    printw("%02d", p1_score);  // Счёт первого игрока слева
    result = 1;
  } else if ((y == height - 2) && (x == width - 15)) {
    printw("%02d", p2_score);  // Счёт второго игрока справа
    result = 1;
  } else if ((y == height - 2) && ((x == width - 14) || (x == 16))) {
    result = 1;  // Пропускаем символы между цифрами
  }
  return result;
}

// Функция рисования вертикальной линии посередине поля
int draw_middle_line(const int x, const int y, const int width,
                     const int height) {
  int result = 0;
  // Центральная линия нарисована чередующимися столбиками "|" каждые два ряда
  if (x == width / 2 && y > 0 && y < height && y % 2 == 1) {
    printw("|");
    result = 1;
  }
  return result;
}

// Функция полного обновления экрана и отрисовки текущего состояния игры
void draw_game(const int width, const int height, const int p1_x,
               const int p1_y, const int p1_score, const int p2_x,
               const int p2_y, const int p2_score, const int b_x,
               const int b_y) {
  // Полностью очищает экран перед обновлением изображения
  clear();

  // Поочерёдно проверяются условия рисования каждого элемента на каждом пикселе
  for (int y = height; y >= 0; y--) {
    for (int x = 0; x <= width; x++) {
      if (!(draw_border(x, width) || draw_border(y, height) ||
            draw_rocket(x, y, p1_x, p1_y) || draw_rocket(x, y, p2_x, p2_y) ||
            draw_ball(x, y, b_x, b_y) ||
            draw_score(width, height, x, y, p1_score, p2_score) ||
            draw_middle_line(x, y, width, height)))

        // Возможно, данная функция выглядит проще с использованием continue,
        // но это можно воспринимать как противоречие принципу о запрете
        // использования goto (у них есть схожести)

        // Для пустых мест выводим пробел
        printw(" ");
    }
    printw("\n");
  }
}

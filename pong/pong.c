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
  // Инициализация игровых параметров

  const int w = 80;
  const int h = 25;
  const int ball_start_x = w / 2;
  const int ball_start_y = h / 2;
  int ball_y = ball_start_y;
  int ball_x = ball_start_x;
  const int ri_1 = 2;
  const int ri_2 = w - 2;
  int rstart_1 = 12;
  int rstart_2 = 12;
  int vector_x = -1;
  int vector_y = 1;
  const int win = 21;
  int player1 = 0;
  int player2 = 0;

  // Главный игровой цикл
  while (player1 < win && player2 < win) {
    char key;  // перенесено в цикл т.к. src/pong.c:53:8: style: The scope of
               // the variable 'key' can be reduced. [variableScope]

    draw_game(w, h, ri_1, rstart_1, player1, ri_2, rstart_2, player2, ball_x,
              ball_y);

    // Обработка движения мяча
    if (ball_y == 1 || ball_y == h - 1) {
      vector_y = -vector_y;  // Отскок от верхней/нижней границы
    }
    if (ball_x == ri_1 + 1 && (ball_y == rstart_1 - 1 || ball_y == rstart_1 ||
                               ball_y == rstart_1 + 1)) {
      vector_x = -vector_x;  // Отскок от левой ракетки
    }
    if (ball_x == ri_2 - 1 && (ball_y == rstart_2 - 1 || ball_y == rstart_2 ||
                               ball_y == rstart_2 + 1)) {
      vector_x = -vector_x;  // Отскок от правой ракетки
    }

    // Движение мяча
    ball_y += vector_y;
    ball_x += vector_x;

    // Проверка на гол
    if (ball_x < 2) {
      player2 += 1;
      ball_x = ball_start_x;
      ball_y = ball_start_y;
    }
    if (ball_x > w - 1) {
      player1 += 1;
      ball_x = ball_start_x;
      ball_y = ball_start_y;
    }

    // Проверка победы
    if (player1 == win)
      printf("Победил первый игрок\n");
    else if (player2 == win)
      printf("Победил второй игрок\n");

    // Обработка управления
    key = input_handle();
    if (key == 'A' && rstart_1 < h - 2)
      rstart_1++;
    else if (key == 'Z' && rstart_1 > 2)
      rstart_1--;
    else if (key == 'K' && rstart_2 < h - 2)
      rstart_2++;
    else if (key == 'M' && rstart_2 > 2)
      rstart_2--;
    else if (key == 'Q')
      player1 =
          22;  // это не логическая ошибка, после этого следующая итерация не
               // начнется, сообщение о победе не будет выведено (сделано для
               // соблюдения принципа Дейкстры - один выход на цикл)
    while (getchar() != '\n');
  }

  return 0;
}

// Прототипы функций находятся вверху файла

// Функция рисования ракетки на экране
int draw_rocket(const int x, const int y, const int p_x, const int p_y) {
  int result = 0;
  // Проверяет, находится ли точка в пределах области ракетки
  if ((p_x == x) && ((p_y - y >= -1) && (p_y - y <= 1))) {
    printf("|");  // Отображается символ ракетки
    result = 1;
  }
  return result;
}

// Функция рисования границ игрового поля
int draw_border(const int axis, const int border) {
  int result = 0;
  // Верхняя и нижняя границы также обозначены символом "#"
  if (axis == 0 || axis == border) {
    printf("#");
    result = 1;
  }
  return result;
}

// Функция рисования мяча на экране
int draw_ball(const int x, const int y, const int b_x, const int b_y) {
  int result = 0;
  // Печатает символ "*", если координата соответствует положению мяча
  if ((x == b_x) && (y == b_y)) {
    printf("*");
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
    printf("%02d", p1_score);  // Счёт первого игрока слева
    result = 1;
  } else if ((y == height - 2) && (x == width - 15)) {
    printf("%02d", p2_score);  // Счёт второго игрока справа
    result = 1;
  } else if ((y == height - 2) && ((x == width - 14) || (x == 16))) {
    result = 1;
  }
  return result;
}

// Функция рисования вертикальной линии посередине поля
int draw_middle_line(const int x, const int y, const int width,
                     const int height) {
  int result = 0;
  // Центральная линия нарисована чередующимися столбиками "|" каждые два ряда
  if (x == width / 2 && y > 0 && y < height && y % 2 == 1) {
    printf("|");
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
  printf("\033[H\033[J");

  // Поочерёдно проверяются условия рисования каждого элемента на каждом пикселе
  for (int y = height; y >= 0; y--) {
    for (int x = 0; x <= width; x++) {
      if (!(draw_border(x, width) || draw_border(y, height) ||
            draw_rocket(x, y, p1_x, p1_y) || draw_rocket(x, y, p2_x, p2_y) ||
            draw_ball(x, y, b_x, b_y) ||
            draw_score(width, height, x, y, p1_score, p2_score) ||
            draw_middle_line(x, y, width, height)))
        printf(" ");

      // Возможно, данная функция выглядит проще с использованием continue,
      // но это можно воспринимать как противоречие принципу о запрете
      // использования goto (у них есть схожести)
    }
    printf("\n");
  }
}

// Функция проверки допустимых символов управления игрой
int input_check(const char ch) {
  return (ch == ' ' || ch == 'A' || ch == 'Z' || ch == 'K' || ch == 'M' ||
          ch == 'Q');
}

// Функция ожидания и обработки ввода пользователя
char input_handle() {
  char input = '$';  // Начинаем с недопустимого символа
  while (!input_check(input)) {
    input = getchar();  // Читаем следующий символ с клавиатуры
  }
  return input;
}
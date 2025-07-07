#include "gui/cli/snake_frontend.h"
#include "gui/cli/tetris_frontend.h"

#include <iostream>

using namespace s21;

int main() {
  initialization_cli();
  start_color();

  if (has_colors() == FALSE) {
    endwin();
    printf("Терминал не поддерживает цвета\n");
    return 1;
  }
  init_colors();
  char ch = '\0';
  while (ch != '3') {
    std::cout << "Выбери игру \t 1. Tetris \t 2. Snake \t 3. Выйти\n";
    std::cin >> ch;

    if (ch == '1') {
      tetris_game_loop();
    } else if (ch == '2') {
      snake_game_loop();
    }
    clear();
    refresh();
  }
  endwin();
  return 0;
}

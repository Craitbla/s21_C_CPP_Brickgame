#include "snake_frontend.h"

namespace s21 {

void snake_game_loop() {
  ConsoleSnakeView view;
  int input = STUB;
  UserAction_t signal = Stub;
  GameInfo_t stats;
  stats.pause = BEGGINING;

  while (stats.pause != EXIT && stats.pause != WIN) {
    userInput(&signal);
    input = (int)signal;
    signal = get_signal(input);

    stats = view.updateCurrentState(signal);
    view.draw(stats);
  }
  napms(2000);
};

GameInfo_t ConsoleSnakeView::updateCurrentState(const UserAction_t &signal) {
  return snake_cont_.updateCurrentState(signal);
}

void ConsoleSnakeView::draw(const GameInfo_t &stats) {
  if (stats.pause == BEGGINING) {
    print_banner_start_game();
  } else if (stats.pause == STOP) {
    print_banner_pause();
  } else if (stats.pause == GO) {
    print_baze_snake_field(stats);
    print_additional_field(stats);
  }
  if (stats.pause == WIN) {
    print_banner_victory();
  } else if (stats.pause == EXIT) {
    print_banner_game_over();
  }
  refresh();
};

void ConsoleSnakeView::print_banner_victory() {
  clear();
  mvaddstr(BOT_Y / 2 - 1, 7, "YOU WON!!!");
  refresh();
};

void ConsoleSnakeView::print_baze_snake_field(const GameInfo_t &stats) {
  for (int i = 0; i < BOT_Y + 1; i++) {
    for (int j = 0; j < FIELD_MAX_X + 1; j++) {
      print_color(stats.field[i][j], i, j);
    }
  }
};

}  // namespace s21
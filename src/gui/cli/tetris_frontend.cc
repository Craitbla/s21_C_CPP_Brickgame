#include "tetris_frontend.h"

namespace s21 {

void tetris_game_loop() {
  ConsoleTetrisView view;
  int input = STUB;
  UserAction_t signal = Stub;
  params_t prms;

  prms_init(&prms);

  while (prms.stats_->pause != EXIT && prms.stats_->pause != WIN) {
    userInput(&signal);
    input = (int)signal;
    signal = get_signal(input);

    view.updateCurrentState(prms, signal);
    view.draw(*prms.stats_);
  }
  napms(2000);
  free_prms(&prms);
};

void ConsoleTetrisView::updateCurrentState(params_t &prms,
                                           const UserAction_t signal) {
  tetris_cont_.updateCurrentState(prms, signal);
}

void ConsoleTetrisView::draw(const GameInfo_t &stats) {
  if (stats.pause == BEGGINING) {
    print_banner_start_game();
  } else if (stats.pause == STOP) {
    print_banner_pause();
  } else if (stats.pause == GO) {
    print_baze_tetris_field(stats);
    print_additional_field(stats);
    print_next_figure(stats.next);
  } else if (stats.pause == EXIT) {
    print_banner_game_over();
  }
  refresh();
};

void ConsoleTetrisView::print_baze_tetris_field(const GameInfo_t &stats) {
  for (int i = 0; i < BOT_Y + 1; i++) {
    for (int j = 0; j < FIELD_MAX_X + 1; j++) {
      print_color(stats.field[i][j], i, j);
    }
  }
};

void ConsoleTetrisView::print_next_figure(int **next) {
  mvaddstr(FIGURE_NEXT_Y, ADDITIONAL_X, "NEXT_FIGURE");
  int x_fig = 0, y_fig = 0;
  for (int i = FIGURE_NEXT_Y + 2; i < FIGURE_NEXT_Y + 2 + FIGURE_SIZE; i++) {
    x_fig = 0;
    for (int j = ADDITIONAL_X + 1; j < ADDITIONAL_X + 1 + FIGURE_SIZE; j++) {
      print_color(next[y_fig][x_fig], i, j);
      x_fig++;
    }
    y_fig++;
  }
};

}  // namespace s21

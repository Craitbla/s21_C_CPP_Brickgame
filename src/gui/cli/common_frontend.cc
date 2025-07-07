
#include "common_frontend.h"

void userInput(UserAction_t *action) { *action = (UserAction_t)getch(); }

UserAction_t get_signal(const int user_input) {
  UserAction_t key = Stub;

  if (user_input == KEY_DOWN)
    key = Down;
  else if (user_input == KEY_UP)
    key = Up;
  else if (user_input == KEY_LEFT)
    key = Left;
  else if (user_input == KEY_RIGHT)
    key = Right;
  else if (user_input == ESC)
    key = Terminate;
  else if (user_input == ENTER)
    key = Start;
  else if (user_input == KEY_LOWER_P || user_input == KEY_UPPER_P)
    key = Pause;
  else if (user_input == SPACE) {
    key = Action;
  } else
    key = Stub;

  return key;
}

void initialization_cli() {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  cbreak();
  nodelay(stdscr, TRUE);
}

void init_colors() {
  init_pair(1, COLOR_BLACK, COLOR_CYAN);
  init_pair(2, COLOR_BLACK, COLOR_YELLOW);
  init_pair(3, COLOR_BLACK, COLOR_BLUE);
  if (COLORS >= EXTENDED_PALETTE) {
    init_pair(4, COLOR_BLACK, COLOR_ORANGE);
  } else {
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
  }
  init_pair(5, COLOR_BLACK, COLOR_GREEN);
  init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(7, COLOR_BLACK, COLOR_RED);
  if (COLORS >= EXTENDED_PALETTE) {
    init_color(COLOR_DARK_GREEN, R_PROPORTIONS, G_AND_B_PROPORTIONS,
               G_AND_B_PROPORTIONS);
    init_pair(8, COLOR_BLACK, COLOR_DARK_GREEN);
  } else {
    init_pair(8, COLOR_BLACK, COLOR_GREEN);
  }
  init_pair(9, COLOR_BLACK, COLOR_WHITE);
}

namespace s21 {

void ConsoleGameView::print_color(int design, int i, int j) {
  attron(COLOR_PAIR(design));
  mvaddch(i, j, ' ');
  attroff(COLOR_PAIR(design));
}

void ConsoleGameView::print_additional_field(const GameInfo_t &stats) {
  mvaddstr(ADD_HIGH_SCORE_Y, ADDITIONAL_X, "HIGH_SCORE");
  mvaddch(ADD_HIGH_SCORE_Y + 1, ADDITIONAL_X - 1, ' ');
  printw("%d", stats.high_score);

  mvaddstr(ADD_SCORE_Y, ADDITIONAL_X, "SCORE");
  mvaddch(ADD_SCORE_Y + 1, ADDITIONAL_X - 1, ' ');
  printw("%d", stats.score);

  mvaddstr(ADD_LEVEL_Y, ADDITIONAL_X, "LEVEL");
  mvaddch(ADD_LEVEL_Y + 1, ADDITIONAL_X - 1, ' ');
  printw("%d", stats.level);
}

void ConsoleGameView::print_banner_start_game() {
  clear();
  mvaddstr(BOT_Y / 2 - 1, 7, "PRESS ENTER");
  mvaddstr(BOT_Y / 2 + 1, 4, "TO START THE GAME");
  refresh();
}

void ConsoleGameView::print_banner_game_over() {
  clear();
  mvaddstr(BOT_Y / 2, 7, "GAME OVER");
  refresh();
}

void ConsoleGameView::print_banner_pause() {
  clear();
  mvaddstr(BOT_Y / 2, 7, "PAUSE");
  refresh();
}

}  // namespace s21
#ifndef COMMON_FRONTEND_H
#define COMMON_FRONTEND_H

#include <ncurses.h>

#include "../../brick_game/common_back.h"
#include "../i_view.h"

#define EXTENDED_PALETTE 256
#define COLOR_ORANGE 214
#define COLOR_DARK_GREEN 100
#define R_PROPORTIONS 0
#define G_AND_B_PROPORTIONS 500

#ifdef __cplusplus
extern "C" {
#endif

void initialization_cli();
void userInput(UserAction_t *action);
UserAction_t get_signal(int user_input);
void init_colors();

#ifdef __cplusplus
}
#endif

namespace s21 {

class ConsoleGameView : public IGameView { 
protected:
  virtual ~ConsoleGameView() = default;
  void print_color(int design, int i, int j);

  void print_additional_field(const GameInfo_t &stats);
  void print_banner_start_game();
  void print_banner_game_over();
  void print_banner_pause();

public:
  virtual void draw(const GameInfo_t &stats) = 0;
};

} // namespace s21

#endif
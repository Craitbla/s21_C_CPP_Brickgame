#ifndef SNAKE_FRONT_H
#define SNAKE_FRONT_H

#include "../controller.h"
#include "common_frontend.h"

namespace s21 {

class ConsoleSnakeView : public ConsoleGameView {
private:
  SnakeController snake_cont_;

  void print_banner_victory();
  void print_baze_snake_field(const GameInfo_t &stats);

public:
  GameInfo_t updateCurrentState(const UserAction_t &signal);
  void draw(const GameInfo_t &stats) override;
};

void snake_game_loop();

} // namespace s21

#endif


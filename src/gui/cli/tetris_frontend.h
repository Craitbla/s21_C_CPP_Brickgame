#ifndef TETRIS_FRONT_H
#define TETRIS_FRONT_H

// #include "../../brick_game/tetris/backend.h"
#include "common_frontend.h"

#include "../controller.h"

namespace s21 {

class ConsoleTetrisView : public ConsoleGameView {
private:
  TetrisController tetris_cont_;

  void print_baze_tetris_field(const GameInfo_t &stats);
  void print_next_figure(int **next);

public:
  void updateCurrentState(params_t &prms, const UserAction_t signal);
  void draw(const GameInfo_t &stats) override;
};

void tetris_game_loop();

} // namespace s21

#endif
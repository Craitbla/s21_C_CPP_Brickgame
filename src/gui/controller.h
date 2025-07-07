#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../brick_game/snake/model.h"
#include "../brick_game/tetris/backend.h"

namespace s21 {

////////////////////////////////////////
// TetrisController
////////////////////////////////////////

class TetrisController { 
public:
  void updateCurrentState(params_t &prms, const UserAction_t &signal) {
    return ::updateCurrentState(&prms, signal);
  }

  void prms_init(params_t &prms) { return ::prms_init(&prms); }

  TetrisController() {};
  ~TetrisController() {};
};

////////////////////////////////////////
// SnakeController
////////////////////////////////////////

class SnakeController {
protected:
  GameModel model_;

public:
  GameInfo_t updateCurrentState(const UserAction_t &signal) {
    return model_.updateCurrentState(signal);
  }

  SnakeController() {};
  ~SnakeController() {};
};

} // namespace s21

#endif

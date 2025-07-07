#ifndef I_VIEW_H
#define I_VIEW_H

#include "../brick_game/common_back.h"

namespace s21 {

////////////////////////////////////////
// IGameView
////////////////////////////////////////

class IGameView {
public:
  virtual void draw(const GameInfo_t &stats) = 0;
  virtual ~IGameView() = default;
};

} // namespace s21

#endif
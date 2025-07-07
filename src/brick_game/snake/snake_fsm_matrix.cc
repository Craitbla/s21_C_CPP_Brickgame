#include "model.h"

namespace s21 {

std::array<std::array<action_t, 9>, 8> GameModel::create_fsm_table() {
  return {
      create_start_row(),     create_spawn_row(),        create_moving_row(),
      create_shifting_row(),  create_eating_apple_row(), create_gameover_row(),
      create_exitstate_row(), create_pause_row(),
  };
};

bool GameModel::check_move_time() {
  bool permission = false;

  if (time_vars.ticks_left_ <= 0) {
    time_vars.ticks_left_ = time_vars.ticks_;
    permission = true;
  }
  time_vars.ticks_left_--;

  if (stats_.level <= MAX_LEVEL) {
    stats_.level = stats_.score / LEVEL_INCREASE_VALUE + 1;
    time_vars.ticks_ = SNAKE_TICKS_START - stats_.level - 1;
  } else {
    stats_.level = MAX_LEVEL;
    time_vars.ticks_ = SNAKE_TICKS_START - MAX_LEVEL - 1;
  }

  if (time_vars.ticks_ < 1) {
    time_vars.ticks_ = 1;
  }

  return permission;
};

void GameModel::check() {
  if (check_move_time()) {
    state_ = SHIFTING;
  }
}

std::array<action_t, 9> GameModel::create_start_row() {
  return {
      [this]() { spawn(); },
      []() {},
      [this]() { exitstate(); },
      []() {},
      []() {},
      []() {},
      []() {},
      []() {},
      []() {},
  };
}

std::array<action_t, 9> GameModel::create_spawn_row() {
  std::array<action_t, 9> row;
  row.fill([this]() { spawn(); });
  return row;
}

std::array<action_t, 9> GameModel::create_moving_row() {
  return {
      [this]() {check(); },     [this]() { include_pause(); },
      [this]() { exitstate(); }, [this]() { moveleft(); },
      [this]() { moveright(); }, [this]() { moveup(); },
      [this]() { movedown(); },  [this]() {check(); },
      [this]() {check(); },
  };
}

std::array<action_t, 9> GameModel::create_shifting_row() {
  return {
      [this]() { shifting(); },  [this]() { include_pause(); },
      [this]() { exitstate(); }, [this]() { shifting(); },
      [this]() { shifting(); },  [this]() { shifting(); },
      [this]() { shifting(); },  [this]() { shifting(); },
      [this]() { shifting(); },
  };
}

std::array<action_t, 9> GameModel::create_eating_apple_row() {
  std::array<action_t, 9> row;
  row.fill([this]() { eating_apple(); });
  return row;
}

std::array<action_t, 9> GameModel::create_gameover_row() {
  std::array<action_t, 9> row;
  row.fill([this]() { gameover(); });
  return row;
}

std::array<action_t, 9> GameModel::create_exitstate_row() {
  std::array<action_t, 9> row;
  row.fill([this]() { exitstate(); });
  return row;
}

std::array<action_t, 9> GameModel::create_pause_row() {
  return {
      []() {},
      [this]() { include_pause(); },
      [this]() { exitstate(); },
      []() {},
      []() {},
      []() {},
      []() {},
      []() {},
      []() {},
  };
}

void GameModel::spawn() {
  set_snake();

  set_spawn_data_apple();
  set_apple();

  state_ = MOVING;
  stats_.pause = GO;
}

void GameModel::set_snake() {
  std::deque<PosCoords_t> body = snake_.get_body();
  PosCoords_t head = body.front();
  stats_.field[head.y_][head.x_] = SNAKE_HEAD;
  for (PosCoords_t cell : body) {
    if (cell.x_ != head.x_ || cell.y_ != head.y_) {
      stats_.field[cell.y_][cell.x_] = SNAKE_BODY;
    }
  }
}

void GameModel::set_apple() {
  PosCoords_t apple_pos = apple_.get_pos();
  stats_.field[apple_pos.y_][apple_pos.x_] = APPLE;
}

void GameModel::move(bool grow_flag) {
  if (grow_flag == DO_NOT_GROW) {
    remove_tail();
    snake_.pop_back();
  }

  set_new_head();
};

void GameModel::move_snake_dir(Direction_t dir) {
  if (dir == (snake_.get_last_dir() + DIRECTION_REVERSER) % NUM_OF_DIRECTIONS) {
    return;
  }

  check_tile = snake_.get_new_head_tile(dir);

  int check_tile_data;
  PosCoords_t tail = snake_.get_tail();

  if (check_tile.y_ == tail.y_ && check_tile.x_ == tail.x_) {
    check_tile_data = EMPTY_TILE;
  } else {
    check_tile_data = stats_.field[check_tile.y_][check_tile.x_];
  }

  if (check_tile_data == EMPTY_TILE) {
    move(DO_NOT_GROW);
    state_ = MOVING;
  } else if (check_tile_data == APPLE) {
    move(GROW);
    state_ = EATING_APPLE;
  } else if (check_tile_data == WALL || check_tile_data == SNAKE_BODY) {
    state_ = GAMEOVER;
  }

  snake_.set_new_dir(dir);
}

void GameModel::include_pause() {
  if (stats_.pause != STOP) {
    stats_.pause = STOP;
    state_ = PAUSE;
  } else if (stats_.pause == STOP) {
    stats_.pause = GO;
    state_ = MOVING;
  }
}

void GameModel::movedown() { move_snake_dir(DOWN); };
void GameModel::moveup() { move_snake_dir(UP); };
void GameModel::moveleft() { move_snake_dir(LEFT); };
void GameModel::moveright() { move_snake_dir(RIGHT); };

void GameModel::move_last_dir() { move_snake_dir(snake_.get_last_dir()); }
void GameModel::shifting() { move_last_dir(); }

void GameModel::eating_apple() {
  stats_.score += 1;
  set_spawn_data_apple();
  set_apple();
  state_ = MOVING;
}

void GameModel::gameover() {
  state_ = EXIT_STATE;
  stats_.pause = EXIT;
}
void GameModel::exitstate() {
  state_ = EXIT_STATE;
  stats_.pause = EXIT;
};

}  // namespace s21
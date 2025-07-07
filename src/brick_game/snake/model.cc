#include "model.h"

namespace s21 {

////////////////////////////////////////////
// APPLE
////////////////////////////////////////////

Apple::Apple() { pos_ = DEFAULT_POS_COORDS_T; };

PosCoords_t Apple::rand_pos() const {
  PosCoords_t new_pos;
  new_pos.x_ = 1 + (rand() % (RIGHT_X - 1));
  new_pos.y_ = 1 + (rand() % (BOT_Y - 1));
  return new_pos;
};

void Apple::set_pos(PosCoords_t new_pos) { pos_ = new_pos; };

PosCoords_t Apple::get_pos() const { return pos_; };

////////////////////////////////////////////
// SNAKE
////////////////////////////////////////////

void Snake::pop_back() { body_.pop_back(); }

void Snake::push_front(PosCoords_t new_head) { body_.push_front(new_head); }

//

std::deque<PosCoords_t> Snake::get_body() const { return body_; };

PosCoords_t Snake::get_tail() const { return body_.back(); }
PosCoords_t Snake::get_head() const { return body_.front(); }
Direction_t Snake::get_last_dir() const { return dir_; };

void Snake::set_new_dir(Direction_t new_dir) { dir_ = new_dir; };

Snake::Snake() { set_spawn_data(); };

void Snake::set_spawn_data() {
  dir_ = INITIAL_SNAKE_DIR;
  for (int i = 0; i < INITIAL_SNAKE_LENGHT; i++) {
    body_.push_back({INITIAL_SNAKE_POS_Y + i, INITIAL_SNAKE_POS_X});
  }
};

PosCoords_t Snake::get_new_head_tile(Direction_t dir) const {
  PosCoords_t new_head = *(get_body().begin());
  int add_x = 0, add_y = 0;
  switch (dir) {
    case DOWN:
      add_y = 1;
      break;
    case UP:
      add_y = -1;
      break;
    case LEFT:
      add_x = -1;
      break;
    case RIGHT:
      add_x = 1;
      break;
    default:
      break;
  }
  new_head.x_ += add_x;
  new_head.y_ += add_y;
  return new_head;
};

////////////////////////////////////////////
// MODEL
////////////////////////////////////////////

GameInfo_t GameModel::updateCurrentState(const UserAction_t &signal) {
  clock_gettime(CLOCK_MONOTONIC, &sp_start);

  action_t act = fsm_table[state_][signal];
  if (act) {
    act();
  }

  if (state_ != PAUSE) {
    if (signal == Stub) {
      delay_in_real_time();
    }
    if (stats_.pause != BEGGINING && state_ != GAMEOVER && state_ != PAUSE &&
        state_ != EATING_APPLE && check_move_time()) {
      state_ = SHIFTING;
    }
  }

  if (stats_.score == WINNING_SCORE) {
    stats_.pause = WIN;
  }
  if (stats_.score > stats_.high_score) {
    stats_.high_score = stats_.score;
    save_high_score(stats_.score, SNAKE_SAVE_FILE_NAME);
  }

  return stats_;
};

void GameModel::delay_in_real_time() {
  clock_gettime(CLOCK_MONOTONIC, &sp_end);
  if (sp_end.tv_sec - sp_start.tv_sec <= 0 &&
      (ts2.tv_nsec = DELAY_PERIOD - (sp_end.tv_nsec - sp_start.tv_nsec)) > 0) {
    nanosleep(&ts2, &ts1);
  }
}

GameModel::GameModel() {
  stats_.field = mat_malloc();
  stats_.next = NULL;
  stats_.score = INITIAL_SCORE;
  stats_.high_score = read_high_score(SNAKE_SAVE_FILE_NAME);
  stats_.level = INITIAL_LEVEL;
  stats_.speed = INITIAL_SPEED;
  stats_.pause = BEGGINING;

  state_ = START;

  time_vars = {SNAKE_TICKS_START, SNAKE_TICKS_START};
  ts2 = {0, 0};

  check_tile = DEFAULT_POS_COORDS_T;

  set_snake();
};

GameModel::~GameModel() {
  if (stats_.field != NULL) {
    free(stats_.field);
    stats_.field = NULL;
  };
}

void GameModel::set_spawn_data_apple() {
  bool rand_again_flag = true;
  while (rand_again_flag) {
    apple_.set_pos(apple_.rand_pos());
    if (stats_.field[apple_.get_pos().y_][apple_.get_pos().x_] == EMPTY_TILE) {
      rand_again_flag = false;
    }
  }
};

void GameModel::remove_tail() {
  PosCoords_t tail = snake_.get_tail();
  stats_.field[tail.y_][tail.x_] = EMPTY_TILE;
};

void GameModel::set_new_head() {
  PosCoords_t old_head = snake_.get_head();
  stats_.field[old_head.y_][old_head.x_] = SNAKE_BODY;

  snake_.push_front(check_tile);
  stats_.field[check_tile.y_][check_tile.x_] = SNAKE_HEAD;
}

}  // namespace s21
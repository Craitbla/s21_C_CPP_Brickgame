#ifndef MODEL_H
#define MODEL_H

#include <deque>
#include <functional>

#include "../common_back.h"

namespace s21 {

#define SNAKE_TICKS_START 45

#define INITIAL_SNAKE_POS_X 5
#define INITIAL_SNAKE_POS_Y 10
#define INITIAL_SNAKE_DIR UP
#define INITIAL_SNAKE_LENGHT 4
#define LEVEL_INCREASE_VALUE 5

#define SNAKE_SAVE_FILE_NAME "snake_high_score.txt"
#define DEFAULT_POS_COORDS_T {0, 0}

#define MAX_LEVEL 10
#define WINNING_SCORE 196

#define DIRECTION_REVERSER 2
#define NUM_OF_DIRECTIONS 4

typedef enum { DO_NOT_GROW = 0, GROW } SnakeState;

typedef enum {
  EMPTY_TILE = 0,
  SNAKE_BODY = 5,
  APPLE = 7,
  SNAKE_HEAD = 8,
  WALL = 9
} SnakeDesignationInMatrix_t;

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  EATING_APPLE,
  GAMEOVER,
  EXIT_STATE,
  PAUSE
} SnakeGameState;

typedef enum { DOWN, LEFT, UP, RIGHT } Direction_t;

class Apple {
private:
  PosCoords_t pos_;

public:
  Apple();
  ~Apple() {};

  void set_pos(PosCoords_t new_pos);
  PosCoords_t get_pos() const;
  PosCoords_t rand_pos() const;
};

class Snake {
private:
  Direction_t dir_;
  std::deque<PosCoords_t> body_;

public:
  Snake();
  ~Snake() {};

  std::deque<PosCoords_t> get_body() const;
  PosCoords_t get_tail() const;
  PosCoords_t get_head() const;

  void set_new_dir(Direction_t new_dir);
  Direction_t get_last_dir() const;

  void pop_back();
  void push_front(PosCoords_t new_head);

  void set_spawn_data();

  PosCoords_t get_new_head_tile(Direction_t dir) const;
};

using action_t = std::function<void()>;

class GameModel {
private:
  GameInfo_t stats_;
  SnakeGameState state_;
  Apple apple_;
  Snake snake_;

  DelayTime_t time_vars;
  struct timespec sp_start, sp_end, ts1, ts2;

  PosCoords_t check_tile;

  std::array<action_t, 9> create_start_row();
  std::array<action_t, 9> create_spawn_row();
  std::array<action_t, 9> create_moving_row();
  std::array<action_t, 9> create_shifting_row();
  std::array<action_t, 9> create_eating_apple_row();
  std::array<action_t, 9> create_gameover_row();
  std::array<action_t, 9> create_exitstate_row();
  std::array<action_t, 9> create_pause_row();

  std::array<std::array<action_t, 9>, 8> create_fsm_table();

  const std::array<std::array<action_t, 9>, 8> fsm_table = create_fsm_table();

  void check();
  void spawn();
  void movedown();
  void moveup();
  void moveright();
  void moveleft();
  void move_last_dir();
  void shifting();
  void eating_apple();
  void include_pause();
  void gameover();
  void exitstate();

  bool check_move_time();
  void delay_in_real_time();

  // apple
  void set_spawn_data_apple();
  void set_apple();

  // snake
  void remove_tail();
  void set_new_head();

  void set_snake();

  void move(bool grow_flag);
  void move_snake_dir(Direction_t dir);

public:
  GameModel();
  ~GameModel();

  GameInfo_t updateCurrentState(const UserAction_t &signal);
};

} // namespace s21
#endif

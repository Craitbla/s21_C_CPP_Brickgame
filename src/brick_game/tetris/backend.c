#include "backend.h"

PosCoords_t figures[NUMBER_OF_FIGURES][NUM_OF_POSITIONS][NUM_OF_POINTS] = {
    {
        {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
        {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
        {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
        {{0, 1}, {1, 1}, {2, 1}, {3, 1}}  // I
    },
    {
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}}  // O
    },
    {
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
        {{0, 1}, {1, 1}, {2, 0}, {2, 1}}  // J
    },
    {
        {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
        {{0, 0}, {0, 1}, {1, 1}, {2, 1}}  // L
    },
    {
        {{0, 1}, {0, 2}, {1, 0}, {1, 1}},
        {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
        {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}}  // S
    },
    {
        {{0, 1}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 1}}  // T
    },
    {
        {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
        {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
        {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 0}}  // Z
    }};

int ticks_list[10] = {0, 2, 4, 7, 10, 13, 16, 18, 21, 24};

int rand_type() { return rand() % 7; }

void stats_init(GameInfo_t **stats) {
  *stats = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  (*stats)->next = figure_malloc();
  (*stats)->score = INITIAL_SCORE;
  (*stats)->high_score = read_high_score(TETRIS_SAVE_FILE_NAME);
  (*stats)->level = INITIAL_LEVEL;
  (*stats)->speed = INITIAL_SPEED;
  (*stats)->pause = BEGGINING;
  (*stats)->field = mat_malloc();
}

void prms_init(params_t *prms) {
  stats_init(&prms->stats_);

  prms->state_ = (tetris_game_state *)malloc(sizeof(tetris_game_state));
  prms->figure_ = (tet_figure_t *)malloc(sizeof(tet_figure_t));
  prms->next_figure_ = (tet_figure_t *)malloc(sizeof(tet_figure_t));
  prms->time_vars_ = (DelayTime_t *)malloc(sizeof(DelayTime_t));
  prms->prev_pos_ = (PosCoords_t *)malloc(sizeof(PosCoords_t));
  prms->down_flag_ = (bool *)malloc(sizeof(bool));
  prms->timespec_ = (timespecs *)malloc(sizeof(timespecs));

  *(prms->state_) = START;
  *prms->figure_ =
      assembly_tet_figure(rand_type(), START_POSITION, START_POS_COORDS_T);
  *prms->next_figure_ =
      assembly_tet_figure(rand_type(), START_POSITION, START_POS_COORDS_T);
  set_next_figure(prms->stats_->next, prms->next_figure_);
  *prms->time_vars_ = (DelayTime_t){TET_TICKS_START, TET_TICKS_START};
  *prms->prev_pos_ = START_POS_COORDS_T;
  *(prms->down_flag_) = false;
  memset(prms->timespec_, 0, sizeof(timespecs));
}

void free_and_null(void **ptr) {
  if (ptr && *ptr) {
    free(*ptr);
    *ptr = NULL;
  }
}

void free_stats(GameInfo_t **stats) {
  if (stats && *stats) {
    free_and_null((void **)&((*stats)->field));
    free_and_null((void **)&((*stats)->next));
    free(*stats);
    *stats = NULL;
  }
}

void free_prms(params_t *prms) {
  free_stats(&prms->stats_);

  free_and_null((void **)&prms->state_);
  free_and_null((void **)&prms->figure_);

  free_and_null((void **)&prms->next_figure_);
  free_and_null((void **)&prms->time_vars_);
  free_and_null((void **)&prms->prev_pos_);

  free_and_null((void **)&prms->down_flag_);
  free_and_null((void **)&prms->timespec_);
}

void delay_in_real_time_tetris(timespecs *cur_t) {
  clock_gettime(CLOCK_MONOTONIC, &cur_t->sp_end);
  if (cur_t->sp_end.tv_sec - cur_t->sp_start.tv_sec <= 0 &&
      (cur_t->ts2.tv_nsec = DELAY_PERIOD - (cur_t->sp_end.tv_nsec -
                                            cur_t->sp_start.tv_nsec)) > 0) {
    nanosleep(&cur_t->ts2, &cur_t->ts1);
  }
}

void change_points(tet_figure_t *fig) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    fig->points_[i] = figures[fig->piece_type_][fig->position_][i];
  }
}

tet_figure_t assembly_tet_figure(int piece_type, int position,
                                 PosCoords_t coord) {
  tet_figure_t fig;
  fig.piece_type_ = piece_type;
  fig.position_ = position;
  fig.coord_ = coord;
  change_points(&fig);

  fig.design_ = fig.piece_type_ + 1;
  return fig;
}

int **figure_malloc() {
  int **figure = NULL;
  int cols = FIGURE_SIZE, rows = FIGURE_SIZE;
  figure = (int **)malloc(rows * sizeof(int *) + cols * rows * sizeof(int));
  figure[0] = (int *)(figure + rows);

  for (int i = 1; i < rows; i++) {
    figure[i] = figure[0] + i * cols;
  }

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      figure[i][j] = EMPTY_TILE;
    }
  }
  return figure;
}

void set_fig(tet_figure_t *fig, GameInfo_t *stats) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    int x = figures[fig->piece_type_][fig->position_][i].x_;
    int y = figures[fig->piece_type_][fig->position_][i].y_;
    stats->field[fig->coord_.y_ + y][fig->coord_.x_ + x] = fig->design_;
  }
}

void remove_fig(tet_figure_t *fig, GameInfo_t *stats, PosCoords_t *prev_pos) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    int x = figures[fig->piece_type_][fig->position_][i].x_;
    int y = figures[fig->piece_type_][fig->position_][i].y_;
    stats->field[prev_pos->y_ + y][prev_pos->x_ + x] = EMPTY_TILE;
  }
  prev_pos->x_ = fig->coord_.x_;
  prev_pos->y_ = fig->coord_.y_;
}

bool check_attaching_permission(params_t *prms) {
  bool check_flag = 0;
  PosCoords_t fig_pos = prms->figure_->coord_;
  PosCoords_t point;

  for (int i = 0; i < NUM_OF_POINTS; i++) {
    point = prms->figure_->points_[i];
    if (prms->stats_->field[fig_pos.y_ + point.y_ + 1][fig_pos.x_ + point.x_] !=
        EMPTY_TILE) {
      check_flag = 1;
      i = NUM_OF_POINTS;
    }
  }
  return check_flag;
}

bool check_collision_left(params_t *prms) {
  bool check_flag = 0;
  PosCoords_t fig_pos = prms->figure_->coord_;
  PosCoords_t point;
  for (int i = 0; i < NUM_OF_POINTS; i++) {
    point = prms->figure_->points_[i];
    if (prms->stats_->field[fig_pos.y_ + point.y_][fig_pos.x_ + point.x_ - 1] !=
        0) {
      check_flag = 1;
    }
  }
  return check_flag;
}

bool check_collision_right(params_t *prms) {
  bool check_flag = 0;
  PosCoords_t fig_pos = prms->figure_->coord_;
  PosCoords_t point;
  for (int i = 0; i < NUM_OF_POINTS; i++) {
    point = prms->figure_->points_[i];
    if (prms->stats_->field[fig_pos.y_ + point.y_][fig_pos.x_ + point.x_ + 1] !=
        0) {
      check_flag = 1;
    }
  }
  return check_flag;
}

bool check_move_time(DelayTime_t *time_vars, GameInfo_t *stats) {
  bool permission = 0;
  int speed;
  if (time_vars->ticks_left_ <= 0) {
    time_vars->ticks_left_ = time_vars->ticks_;
    permission = 1;
  }
  time_vars->ticks_left_--;
  if (stats->score / 600 < 10) {
    stats->level = stats->score / 600 + 1;
    time_vars->ticks_ = TET_TICKS_START - ticks_list[stats->level - 1];
    speed = ticks_list[stats->level - 1];
    if (time_vars->ticks_ < 1) {
      time_vars->ticks_ = 1;
    }
  } else {
    time_vars->ticks_ = TET_TICKS_START - ticks_list[9];
    speed = ticks_list[9];
    if (time_vars->ticks_ < 1) {
      time_vars->ticks_ = 1;
    }
    stats->level = 10;
  }
  stats->speed = speed;
  return permission;
}

int check_clean_string(int **field) {
  bool flag_of_fullness = 1;
  int counter_clean = 0;
  for (int i = BOT_Y - 1; i > 0; i--) {
    flag_of_fullness = 1;
    for (int j = 1; j < RIGHT_X; j++) {
      if (field[i][j] == EMPTY_TILE) {
        flag_of_fullness = 0;
      }
    }
    if (flag_of_fullness == 1) {
      counter_clean++;
      shift_down(i, field);
      i++;
      add_zeroes_string(field);
    }
  }
  return counter_clean;
}

void shift_down(int y, int **field) {
  for (int p = y; p > TOP_Y; p--) {
    for (int o = 1; o < RIGHT_X; o++) {
      field[p][o] = field[p - 1][o];
    }
  }
}

void add_zeroes_string(int **field) {
  for (int t = 1; t < RIGHT_X; t++) {
    field[1][t] = EMPTY_TILE;
  }
}

bool check_spawn_permission(params_t *prms) {
  bool check_flag = 0;
  PosCoords_t fig_pos = prms->figure_->coord_;
  PosCoords_t point;

  for (int i = 0; i < NUM_OF_POINTS; i++) {
    point = prms->figure_->points_[i];
    if (prms->stats_->field[fig_pos.y_ + point.y_][fig_pos.x_ + point.x_] !=
        0) {
      check_flag = 1;
      i = NUM_OF_POINTS;
    }
  }
  return check_flag;
}

void set_next_figure(int **next, tet_figure_t *fig) {
  for (int i = 0; i < NUM_OF_POINTS; i++) {
    next[fig->points_[i].y_][fig->points_[i].x_] = fig->design_;
  }
}

void clean_next_figure(int **next) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      next[i][j] = EMPTY_TILE;
    }
  }
}

// void final_delay() { napms(2000); }

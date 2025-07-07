#include "backend.h"

action fsm_table[8][9] = {
    {spawn, NULL, exitstate, NULL, NULL, NULL, NULL, NULL, NULL},
    {spawn, spawn, spawn, spawn, spawn, spawn, spawn, spawn, spawn},
    {check, include_pause, exitstate, moveleft, moveright, check, movedown,
     rotation, check},
    {shifting, include_pause, exitstate, shifting, shifting, shifting, shifting,
     shifting, shifting},
    {attaching, attaching, attaching, attaching, attaching, attaching,
     attaching, attaching, attaching},
    {gameover, gameover, gameover, gameover, gameover, gameover, gameover,
     gameover, gameover},
    {exitstate, exitstate, exitstate, exitstate, exitstate, exitstate,
     exitstate, exitstate, exitstate},
    {NULL, include_pause, exitstate, NULL, NULL, NULL, NULL, NULL, NULL},
};

void updateCurrentState(params_t *prms, const UserAction_t signal) {
  if (*prms->state_ != PAUSE) {
    if (*prms->state_ != SPAWN) {
      remove_fig(prms->figure_, prms->stats_, prms->prev_pos_);
    }
    clean_next_figure(prms->stats_->next);
    clock_gettime(CLOCK_MONOTONIC, &prms->timespec_->sp_start);
  }
  action act = fsm_table[*prms->state_][signal];

  if (act) {
    act(prms);
  }

  if (*prms->state_ != EXIT_STATE && *prms->state_ != PAUSE &&
      *prms->state_ != START) {
    if (*prms->state_ != SPAWN && *prms->state_ != GAMEOVER) {
      set_fig(prms->figure_, prms->stats_);
      *prms->prev_pos_ = prms->figure_->coord_;
    }
    set_next_figure(prms->stats_->next, prms->next_figure_);
    delay_in_real_time_tetris(prms->timespec_);
  }

  if (prms->stats_->score > prms->stats_->high_score) {
    prms->stats_->high_score = prms->stats_->score;
    save_high_score(prms->stats_->score, TETRIS_SAVE_FILE_NAME);
  }
};

void rotation(params_t *prms) {
  bool check_flag = 1;
  PosCoords_t fig_pos = prms->figure_->coord_;
  PosCoords_t point;
  int prev_pos = prms->figure_->position_;

  prms->figure_->position_ = (prms->figure_->position_ + 1) % 4;
  change_points(prms->figure_);
  for (int i = 0; i < NUM_OF_POINTS; i++) {
    point = prms->figure_->points_[i];
    if (prms->stats_->field[fig_pos.y_ + point.y_][fig_pos.x_ + point.x_] !=
        0) {
      check_flag = 0;
      i = NUM_OF_POINTS;
    }
  }
  if (check_flag == 0) {
    prms->figure_->position_ = prev_pos;
    change_points(prms->figure_);
  }
  check(prms);
}

void check(params_t *prms) {
  if (check_attaching_permission(prms)) {
    *prms->state_ = ATTACHING;
  } else if (check_move_time(prms->time_vars_, prms->stats_)) {
    *prms->state_ = SHIFTING;
  } else {
    *prms->state_ = MOVING;
  }
}

void shifting(params_t *prms) {
  if (!check_attaching_permission(prms)) {
    prms->figure_->coord_.y_ += 1;
    *prms->state_ = MOVING;
  } else {
    *prms->state_ = ATTACHING;
    *prms->down_flag_ = 0;
  }
}

void attaching(params_t *prms) {
  set_fig(prms->figure_, prms->stats_);
  int strike_counter = check_clean_string(prms->stats_->field);
  if (strike_counter != 0) {
    switch (strike_counter) {
      case 1:
        prms->stats_->score += 100;
        break;
      case 2:
        prms->stats_->score += 300;
        break;
      case 3:
        prms->stats_->score += 700;
        break;
      case 4:
        prms->stats_->score += 1500;
        break;

      default:
        break;
    }
  }
  *prms->state_ = SPAWN;
}

void spawn(params_t *prms) {
  PosCoords_t start_pos = {1, 4};
  *prms->figure_ = *prms->next_figure_;
  if (prms->figure_->piece_type_ == 0) {
    prms->figure_->coord_.y_--;
  }
  *prms->next_figure_ =
      assembly_tet_figure(rand_type(), START_POSITION, start_pos);
  if (prms->next_figure_->piece_type_ == prms->figure_->piece_type_) {
    while (prms->next_figure_->piece_type_ == prms->figure_->piece_type_) {
      *prms->next_figure_ =
          assembly_tet_figure(rand_type(), START_POSITION, start_pos);
      change_points(prms->next_figure_);
    }
  }
  change_points(prms->next_figure_);
  set_next_figure(prms->stats_->next, prms->next_figure_);
  if (check_spawn_permission(prms)) {
    *prms->state_ = GAMEOVER;
  } else {
    *prms->state_ = MOVING;
    prms->stats_->pause = GO;
  }
}

void movedown(params_t *prms) {
  remove_fig(prms->figure_, prms->stats_, prms->prev_pos_);
  *prms->down_flag_ = 1;
  *prms->state_ = SHIFTING;
}

void moveright(params_t *prms) {
  if (!check_collision_right(prms)) {
    remove_fig(prms->figure_, prms->stats_, prms->prev_pos_);
    prms->figure_->coord_.x_ += 1;
  }
  if (check_attaching_permission(prms)) {
    *prms->state_ = ATTACHING;
  } else if (check_move_time(prms->time_vars_, prms->stats_)) {
    *prms->state_ = SHIFTING;
  } else
    *prms->state_ = MOVING;
}

void moveleft(params_t *prms) {
  if (!check_collision_left(prms)) {
    remove_fig(prms->figure_, prms->stats_, prms->prev_pos_);
    prms->figure_->coord_.x_ -= 1;
  }
  if (check_attaching_permission(prms)) {
    *prms->state_ = ATTACHING;
  } else if (check_move_time(prms->time_vars_, prms->stats_)) {
    *prms->state_ = SHIFTING;
  } else
    *prms->state_ = MOVING;
}

void include_pause(params_t *prms) {
  if (prms->stats_->pause != STOP) {
    prms->stats_->pause = STOP;
    *prms->state_ = PAUSE;
  } else if (prms->stats_->pause == STOP) {
    prms->stats_->pause = GO;
    *prms->state_ = MOVING;
  }
}

void gameover(params_t *prms) { go_out(prms); }

void exitstate(params_t *prms) { go_out(prms); }

void go_out(params_t *prms) {
  *prms->state_ = EXIT_STATE;
  prms->stats_->pause = EXIT;
}

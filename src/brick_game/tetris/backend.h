#ifndef BACKEND_H
#define BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../common_back.h"

#define MAX_LEVEL 10
#define NUMBER_OF_FIGURES 7
#define FIGURE_SIZE 4
#define NUM_OF_POSITIONS 4
#define NUM_OF_POINTS 4

#define START_POSITION 0
#define START_POS_COORDS_T ((PosCoords_t){1, 4})

#define TETRIS_SAVE_FILE_NAME "tetris_high_score.txt"
#define TET_TICKS_START 28

typedef enum {
  EMPTY_TILE = 0,
  LIGHT_BLUE, // I
  YELLOW,     // O
  BLUE,       // J
  ORANGE,     // L
  GREEN,      // S //  SNAKE_BODY
  PURPLE,     // T //  SNAKE_HEAD
  RED,        // Z //  APPLE
  WALL = 9
} TetrisDesignationInMatrix_t;

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  EXIT_STATE,
  PAUSE
} tetris_game_state;

typedef struct {
  int piece_type_;
  int position_;
  PosCoords_t coord_;
  PosCoords_t points_[FIGURE_SIZE];
  TetrisDesignationInMatrix_t design_;
} tet_figure_t;

typedef struct {
  struct timespec sp_start;
  struct timespec sp_end;
  struct timespec ts1;
  struct timespec ts2;
} timespecs;

typedef struct game_params {
  GameInfo_t *stats_;
  tetris_game_state *state_;
  tet_figure_t *figure_;
  tet_figure_t *next_figure_;
  DelayTime_t *time_vars_;
  PosCoords_t *prev_pos_;
  bool *down_flag_;
  timespecs *timespec_;
} params_t;

typedef void (*action)(params_t *prms);

void spawn(params_t *prms);
void movedown(params_t *prms);
void moveright(params_t *prms);
void moveleft(params_t *prms);
void shifting(params_t *prms);
void include_pause(params_t *prms);
void gameover(params_t *prms);
void exitstate(params_t *prms);
void check(params_t *prms);
void rotation(params_t *prms);
void attaching(params_t *prms);
void go_out(params_t *prms);

void stats_init(GameInfo_t **stats);

void prms_init(params_t *prms);
void free_and_null(void **ptr);
void free_prms(params_t *prms);

void updateCurrentState(params_t *prms, const UserAction_t signal);

void delay_in_real_time_tetris(timespecs *cur_t);

UserAction_t get_signal(const int user_input);

int **figure_malloc();

void set_fig(tet_figure_t *fig, GameInfo_t *stats);
void remove_fig(tet_figure_t *fig, GameInfo_t *stats, PosCoords_t *prev_pos);

tet_figure_t assembly_tet_figure(int piece_type, int position,
                                 PosCoords_t coord);
void change_points(tet_figure_t *fig);
int rand_type();

bool check_attaching_permission(params_t *prms);
int check_clean_string(int **field);
void shift_down(int y, int **field);
void add_zeroes_string(int **field);
bool check_collision_left(params_t *prms);
bool check_collision_right(params_t *prms);
bool check_spawn_permission(params_t *prms);
bool check_move_time(DelayTime_t *time_vars, GameInfo_t *stats);

void set_next_figure(int **next, tet_figure_t *fig);
void clean_next_figure(int **next);

void free_stats(GameInfo_t **stats);

#ifdef __cplusplus
}
#endif

//

#endif

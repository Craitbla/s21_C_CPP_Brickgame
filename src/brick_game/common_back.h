#ifndef COMMON_BACK_H
#define COMMON_BACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>

#ifdef border
#undef border
#endif

#ifdef scroll
#undef scroll
#endif

#define INITIAL_SCORE 0
#define INITIAL_LEVEL 1
#define INITIAL_SPEED 1

#define ESC 27
#define ENTER 10

#define KEY_DOWN 0402
#define KEY_UP 0403
#define KEY_LEFT 0404
#define KEY_RIGHT 0405

#define KEY_LOWER_P 112
#define KEY_UPPER_P 80
#define SPACE 32
#define STUB 9

#define MAX_LEVEL 10

#define TOP_Y 1
#define BOT_Y 21
#define LEFT_X 1
#define RIGHT_X 11

#define ADD_HIGH_SCORE_Y 1
#define ADD_SCORE_Y 4
#define ADD_LEVEL_Y 7
#define FIGURE_NEXT_Y 10
#define ADDITIONAL_X 13
#define FIELD_MAX_X 26

#define DELAY_PERIOD 33000000

typedef struct {
  int y_;
  int x_;
} PosCoords_t;

typedef struct {
  int ticks_;
  int ticks_left_;
} DelayTime_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Stub
} UserAction_t;

typedef enum { BEGGINING, GO, STOP, EXIT, WIN } PauseState_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

int read_high_score(const char *file_name);
void save_high_score(int high_score, const char *file_name);
int **mat_malloc();

#ifdef __cplusplus
}
#endif

#endif
#include "common_back.h"

typedef enum { EMPTY_TILE = 0, WALL = 9 } DesignationInMatrix_t;

int read_high_score(const char *file_name) {
  int high_score = 0;
  FILE *fp = fopen(file_name, "r+");
  if (fp != NULL) {
    fscanf(fp, "%d", &high_score);
  }
  fclose(fp);
  return high_score;
}

void save_high_score(int high_score, const char *file_name) {
  FILE *fp = fopen(file_name, "w+");
  if (fp != NULL) {
    fprintf(fp, "%d\n", high_score);
  }
  fclose(fp);
}

int **mat_malloc() {
  int **field = NULL;
  int cols = FIELD_MAX_X + 1, rows = BOT_Y + 1;
  field = (int **)malloc(rows * sizeof(int *) + cols * rows * sizeof(int));
  field[0] = (int *)(field + rows);

  for (int i = 1; i < rows; i++) {
    field[i] = field[0] + i * cols;
  }

  for (int i = 0; i < BOT_Y + 1; i++) {
    for (int j = 0; j < FIELD_MAX_X + 1; j++) {
      if (i == 0 || i == BOT_Y || j == 0 || j == RIGHT_X || j == FIELD_MAX_X ||
          (i == ADD_HIGH_SCORE_Y + 2 && j > RIGHT_X) ||
          (i == ADD_SCORE_Y + 2 && j > RIGHT_X) ||
          (i == ADD_LEVEL_Y + 2 && j > RIGHT_X))
        field[i][j] = WALL;
      else
        field[i][j] = EMPTY_TILE;
    }
  }

  return field;
}

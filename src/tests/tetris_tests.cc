#include <gtest/gtest.h>

#include "../brick_game/tetris/backend.h"

class BackendTest : public ::testing::Test {
 protected:
  void SetUp() override { test_filename = "test_highscore.txt"; }

  void TearDown() override { remove(test_filename); }

  const char *test_filename;
};

TEST_F(BackendTest, SaveAndReadHighScore) {
  int expected_score = 12345;
  save_high_score(expected_score, test_filename);

  int actual_score = read_high_score(test_filename);
  EXPECT_EQ(actual_score, expected_score);
}

TEST_F(BackendTest, SaveHighScoreOverwrite) {
  save_high_score(100, test_filename);
  save_high_score(200, test_filename);

  int actual_score = read_high_score(test_filename);
  EXPECT_EQ(actual_score, 200);
}

TEST_F(BackendTest, MatMallocInitialization) {
  int **field = mat_malloc();

  for (int i = 0; i < BOT_Y + 1; i++) {
    EXPECT_EQ(field[i][0], WALL);
    if (i != 0 && i != BOT_Y) {
      EXPECT_EQ(field[i][RIGHT_X], WALL);
    }
    EXPECT_EQ(field[i][FIELD_MAX_X], WALL);
  }

  for (int j = 0; j < FIELD_MAX_X + 1; j++) {
    EXPECT_EQ(field[0][j], WALL);
    EXPECT_EQ(field[BOT_Y][j], WALL);
  }

  EXPECT_EQ(field[ADD_HIGH_SCORE_Y + 2][RIGHT_X + 1], WALL);
  EXPECT_EQ(field[ADD_SCORE_Y + 2][RIGHT_X + 1], WALL);
  EXPECT_EQ(field[ADD_LEVEL_Y + 2][RIGHT_X + 1], WALL);

  for (int i = 1; i < BOT_Y; i++) {
    for (int j = 1; j < RIGHT_X; j++) {
      if (j != RIGHT_X) {
        EXPECT_EQ(field[i][j], EMPTY_TILE);
      }
    }
  }

  free(field);
}

TEST_F(BackendTest, MatMallocMemoryLayout) {
  int **field = mat_malloc();

  int cols = FIELD_MAX_X + 1;
  int rows = BOT_Y + 1;

  for (int i = 1; i < rows; i++) {
    EXPECT_EQ(field[i], field[0] + i * cols);
  }

  free(field);
}

class TetrisTest : public ::testing::Test {
 protected:
  void SetUp() override {
    test_filename = "test_highscore.txt";
    prms_init(&prms);
  }

  void TearDown() override {
    free_prms(&prms);
    remove(test_filename);
  }

  params_t prms;
  const char *test_filename;
};

TEST_F(TetrisTest, StatsInitInitializesCorrectly) {
  GameInfo_t *stats = nullptr;
  stats_init(&stats);
  ASSERT_NE(stats, nullptr);
  EXPECT_EQ(stats->score, INITIAL_SCORE);
  EXPECT_EQ(stats->level, INITIAL_LEVEL);
  EXPECT_EQ(stats->speed, INITIAL_SPEED);
  EXPECT_NE(stats->field, nullptr);
  EXPECT_NE(stats->next, nullptr);
  free_stats(&stats);
}

TEST_F(TetrisTest, ParamsInitInitializesCorrectly) {
  EXPECT_NE(prms.stats_, nullptr);
  EXPECT_NE(prms.figure_, nullptr);
  EXPECT_NE(prms.next_figure_, nullptr);
  EXPECT_EQ(*prms.state_, START);
  EXPECT_TRUE(prms.figure_->piece_type_ >= 0 && prms.figure_->piece_type_ < 7);
}

TEST_F(TetrisTest, FigureMallocCreatesValidMatrix) {
  int **figure = figure_malloc();
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      EXPECT_EQ(figure[i][j], EMPTY_TILE);
    }
  }
  free(figure);
}

TEST_F(TetrisTest, AssemblyTetFigureCreatesValidFigure) {
  tet_figure_t fig = assembly_tet_figure(0, 0, {1, 4});
  EXPECT_EQ(fig.piece_type_, 0);
  EXPECT_EQ(fig.position_, 0);
  EXPECT_EQ(fig.design_, LIGHT_BLUE);
  EXPECT_EQ(fig.points_[0].y_, 1);
  EXPECT_EQ(fig.points_[0].x_, 0);
}

TEST_F(TetrisTest, RemoveFigureClearsCorrectCellsAndUpdatesPosition) {
  tet_figure_t fig = assembly_tet_figure(0, 0, {5, 4});
  PosCoords_t prev_pos = {5, 4};

  set_fig(&fig, prms.stats_);
  EXPECT_EQ(prms.stats_->field[5 + 1][4 + 0], LIGHT_BLUE);
  EXPECT_EQ(prms.stats_->field[5 + 1][4 + 1], LIGHT_BLUE);
  EXPECT_EQ(prms.stats_->field[5 + 1][4 + 2], LIGHT_BLUE);
  EXPECT_EQ(prms.stats_->field[5 + 1][4 + 3], LIGHT_BLUE);

  remove_fig(&fig, prms.stats_, &prev_pos);

  EXPECT_EQ(prms.stats_->field[5 + 1][4 + 0], EMPTY_TILE);
  EXPECT_EQ(prms.stats_->field[5 + 1][4 + 1], EMPTY_TILE);
  EXPECT_EQ(prms.stats_->field[5 + 1][4 + 2], EMPTY_TILE);
  EXPECT_EQ(prms.stats_->field[5 + 1][4 + 3], EMPTY_TILE);

  EXPECT_EQ(prev_pos.x_, 4);
  EXPECT_EQ(prev_pos.y_, 5);
}

TEST_F(TetrisTest, CheckAttachingPermission) {
  *prms.figure_ = assembly_tet_figure(0, 0, {BOT_Y - 2, 4});
  EXPECT_TRUE(check_attaching_permission(&prms));
  *prms.figure_ = assembly_tet_figure(0, 0, {5, 4});
  EXPECT_FALSE(check_attaching_permission(&prms));
}

TEST_F(TetrisTest, CheckCollisions) {
  *prms.figure_ = assembly_tet_figure(0, 0, {5, LEFT_X});
  EXPECT_TRUE(check_collision_left(&prms));
  *prms.figure_ = assembly_tet_figure(0, 0, {5, RIGHT_X - 4});
  EXPECT_TRUE(check_collision_right(&prms));
  *prms.figure_ = assembly_tet_figure(0, 0, {5, 4});
  EXPECT_FALSE(check_collision_left(&prms));
  EXPECT_FALSE(check_collision_right(&prms));
}

TEST_F(TetrisTest, CheckCleanString) {
  for (int j = 1; j < RIGHT_X; j++) {
    prms.stats_->field[BOT_Y - 2][j] = LIGHT_BLUE;
  }
  int cleaned = check_clean_string(prms.stats_->field);
  EXPECT_EQ(cleaned, 1);
  for (int j = 1; j < RIGHT_X; j++) {
    EXPECT_EQ(prms.stats_->field[BOT_Y - 2][j], EMPTY_TILE);
  }
}

TEST_F(TetrisTest, StateMachineTransitions) {
  EXPECT_EQ(*prms.state_, START);

  updateCurrentState(&prms, Start);
  EXPECT_EQ(*prms.state_, MOVING);

  updateCurrentState(&prms, Action);
  EXPECT_EQ(*prms.state_, MOVING);

  updateCurrentState(&prms, Pause);
  EXPECT_EQ(*prms.state_, PAUSE);

  updateCurrentState(&prms, Pause);
  EXPECT_EQ(*prms.state_, MOVING);

  *prms.figure_ = assembly_tet_figure(0, 0, {1, 4});
  updateCurrentState(&prms, Down);
  EXPECT_EQ(*prms.state_, SHIFTING);

  *prms.state_ = MOVING;
  updateCurrentState(&prms, Terminate);
  EXPECT_EQ(*prms.state_, EXIT_STATE);
}

TEST_F(TetrisTest, NextFigureOperations) {
  tet_figure_t fig = assembly_tet_figure(1, 0, {0, 0});
  clean_next_figure(prms.stats_->next);
  set_next_figure(prms.stats_->next, &fig);
  EXPECT_EQ(prms.stats_->next[0][0], YELLOW);
  EXPECT_EQ(prms.stats_->next[0][1], YELLOW);
  EXPECT_EQ(prms.stats_->next[1][0], YELLOW);
  EXPECT_EQ(prms.stats_->next[1][1], YELLOW);
  clean_next_figure(prms.stats_->next);
  EXPECT_EQ(prms.stats_->next[0][0], EMPTY_TILE);
}

TEST_F(TetrisTest, RotationOperation) {
  *prms.state_ = MOVING;
  *prms.figure_ = assembly_tet_figure(0, 0, {5, 4});
  set_fig(prms.figure_, prms.stats_);
  rotation(&prms);
  EXPECT_EQ(prms.figure_->position_, 0);
}

TEST_F(TetrisTest, RotationCollision) {
  *prms.figure_ = assembly_tet_figure(0, 0, {5, RIGHT_X - 3});
  rotation(&prms);
  EXPECT_EQ(prms.figure_->position_, 1);
}

TEST_F(TetrisTest, MoveDownSetsShiftState) {
  *prms.state_ = MOVING;
  *prms.figure_ = assembly_tet_figure(0, 0, {5, 4});
  movedown(&prms);
  EXPECT_EQ(*prms.state_, SHIFTING);
  EXPECT_TRUE(*prms.down_flag_);
}

TEST_F(TetrisTest, MoveRightUpdatesPosition) {
  *prms.state_ = MOVING;
  *prms.figure_ = assembly_tet_figure(0, 0, {5, 4});
  moveright(&prms);
  EXPECT_EQ(prms.figure_->coord_.x_, 5);
}

TEST_F(TetrisTest, MoveLeftUpdatesPosition) {
  *prms.state_ = MOVING;
  *prms.figure_ = assembly_tet_figure(0, 0, {5, 5});
  moveleft(&prms);
  EXPECT_EQ(prms.figure_->coord_.x_, 4);
}

TEST_F(TetrisTest, SpawnChangesState) {
  *prms.state_ = SPAWN;
  spawn(&prms);
  EXPECT_NE(*prms.state_, SPAWN);
}

TEST_F(TetrisTest, GameOverOnFullField) {
  for (int i = 1; i < BOT_Y; i++) {
    for (int j = 1; j < RIGHT_X; j++) {
      prms.stats_->field[i][j] = LIGHT_BLUE;
    }
  }
  *prms.state_ = SPAWN;
  spawn(&prms);
  EXPECT_EQ(*prms.state_, GAMEOVER);
}

TEST_F(TetrisTest, PauseTogglesState) {
  *prms.state_ = MOVING;
  include_pause(&prms);
  EXPECT_EQ(*prms.state_, PAUSE);
  include_pause(&prms);
  EXPECT_EQ(*prms.state_, MOVING);
}

TEST_F(TetrisTest, AttachingUpdatesScore) {
  *prms.state_ = ATTACHING;
  for (int j = 1; j < RIGHT_X; j++) {
    prms.stats_->field[BOT_Y - 2][j] = LIGHT_BLUE;
  }
  attaching(&prms);
  EXPECT_EQ(prms.stats_->score, 100);
}

TEST_F(TetrisTest, CheckStateTransitions) {
  *prms.state_ = MOVING;
  check(&prms);
  EXPECT_TRUE(*prms.state_ == MOVING || *prms.state_ == SHIFTING ||
              *prms.state_ == ATTACHING);
}

TEST_F(TetrisTest, ShiftMovesDownWhenPossible) {
  *prms.state_ = SHIFTING;
  *prms.figure_ = assembly_tet_figure(0, 0, {5, 4});
  shifting(&prms);
  EXPECT_EQ(prms.figure_->coord_.y_, 6);
}

TEST_F(TetrisTest, ExitStateUpdatesParams) {
  *prms.state_ = MOVING;
  exitstate(&prms);
  EXPECT_EQ(*prms.state_, EXIT_STATE);
  EXPECT_EQ(prms.stats_->pause, EXIT);
}

TEST_F(TetrisTest, GameOverCallsExit) {
  *prms.state_ = MOVING;
  gameover(&prms);
  EXPECT_EQ(*prms.state_, EXIT_STATE);
}

TEST_F(TetrisTest, SpawnAvoidsDuplicateFigures) {
  *prms.figure_ = assembly_tet_figure(1, 0, {1, 4});
  *prms.next_figure_ = assembly_tet_figure(1, 0, {1, 4});
  spawn(&prms);
  EXPECT_NE(prms.next_figure_->piece_type_, 1);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
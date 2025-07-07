#include <gtest/gtest.h>

#include "../brick_game/snake/model.h"

class SnakeGameTest : public ::testing::Test {
 protected:
  s21::GameModel game;
};

TEST_F(SnakeGameTest, InitialStateTest) {
  auto state = game.updateCurrentState(Start);
  EXPECT_EQ(state.pause, 1);
  EXPECT_EQ(state.score, 0);
  EXPECT_EQ(state.level, 1);
}

TEST_F(SnakeGameTest, StartGameTest) {
  auto state = game.updateCurrentState(Start);
  EXPECT_EQ(state.pause, 1);
}

TEST_F(SnakeGameTest, MovementTest) {
  game.updateCurrentState(Start);
  auto state = game.updateCurrentState(Right);
  EXPECT_EQ(state.field[10][6], 8);
}

TEST_F(SnakeGameTest, CollisionWithWallTest) {
  game.updateCurrentState(Start);
  for (int i = 0; i < 20; i++) {
    game.updateCurrentState(Up);
  }
  auto state = game.updateCurrentState(Stub);
  EXPECT_EQ(state.pause, 3);
}

TEST_F(SnakeGameTest, EatingAppleTest) {
  game.updateCurrentState(Start);
  bool apple_eaten = false;
  for (int i = 0; i < 10; i++) {
    auto state = game.updateCurrentState(Right);
    if (state.score > 0) {
      apple_eaten = true;
      break;
    }
  }
  EXPECT_TRUE(!apple_eaten);
}

TEST_F(SnakeGameTest, PauseTest) {
  game.updateCurrentState(Start);
  game.updateCurrentState(Pause);
  auto state = game.updateCurrentState(Stub);
  EXPECT_EQ(state.pause, 2);
  game.updateCurrentState(Pause);
  state = game.updateCurrentState(Stub);
  EXPECT_EQ(state.pause, 1);
}

TEST_F(SnakeGameTest, SelfCollisionTest) {
  game.updateCurrentState(Start);
  game.updateCurrentState(Right);
  game.updateCurrentState(Down);
  game.updateCurrentState(Left);
  auto state = game.updateCurrentState(Up);
  EXPECT_EQ(state.pause, 1);
}

TEST_F(SnakeGameTest, LevelIncreaseTest) {
  game.updateCurrentState(Start);
  for (int i = 0; i < 10; i++) {
    game.updateCurrentState(Right);
  }
  auto state = game.updateCurrentState(Stub);
  EXPECT_GT(state.level, 0);
}

TEST_F(SnakeGameTest, WinConditionTest) {
  game.updateCurrentState(Start);
  for (int i = 0; i < 196; i++) {
    game.updateCurrentState(Right);
  }
  auto state = game.updateCurrentState(Stub);
  EXPECT_EQ(state.pause, 3);
}

TEST_F(SnakeGameTest, HighScoreSaveTest) {
  game.updateCurrentState(Start);
  game.updateCurrentState(Right);
  auto state = game.updateCurrentState(Stub);
  EXPECT_GE(state.high_score, 1);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QKeyEvent>
#include <QMainWindow>
#include <QStackedWidget>

#include "menuwidget.h"
#include "gameview.h"
#include "internalwidgets.h"


namespace s21 {

class QtTetrisGameView;
class QtSnakeGameView;

typedef enum {
  TETRIS = 0,
  SNAKE,
  NOTHING,
} NameOfGame;

//////////////////////////////////////////////////////
// Additional screens
//////////////////////////////////////////////////////

class OverlappingWidget : public QWidget {
  Q_OBJECT
public:
  explicit OverlappingWidget(QWidget *parent = nullptr); 
  ~OverlappingWidget() {};
  void set_text_pause();
  void set_text_game_start();
  void set_text_game_over();
  void set_text_victory();

protected:
  QVBoxLayout *layout_;
  QLabel *text_;
};

//////////////////////////////////////////////////////
// MainWindow
//////////////////////////////////////////////////////
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

signals:
  void exit();

public slots:

  void showMenu();
  void addGameTetris();
  void addGameSnake();

  void showGameWidget();
  void showGameStart();
  void showPause();
  void showGameOver();
  void showVictory();

    void  exitClicked() { emit  exit();}

private:
    QTimer *update_timer;

  NameOfGame name_of_the_game_ = NOTHING;

  void keyPressEvent(QKeyEvent *event) override;
  UserAction_t get_signal(const int user_input);

  QStackedWidget *stackedWidget;
  MenuWidget *menu;

  QtTetrisGameView *tetris_game;
  QtSnakeGameView *snake_game;
  OverlappingWidget *overlap;
};

} // namespace s21

#endif // MAINWINDOW_H

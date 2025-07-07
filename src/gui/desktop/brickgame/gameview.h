#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "common_libs.h"

#include "../../controller.h"
#include "../../i_view.h"

#include "mainwindow.h"
#include "internalwidgets.h"

namespace s21 {

class MainWindow;

////////////////////////////////////////
// QtGameView
////////////////////////////////////////

class QtGameView : public QWidget, public IGameView {
  Q_OBJECT
public:
  explicit QtGameView(QWidget *parent = nullptr);
  virtual ~QtGameView() = default;

protected:

  void connect_overlap(); //точно тут

  MainWindow *main_ = qobject_cast<MainWindow *>(parent()); 

  QHBoxLayout *general_layout_;
  MainGameWidget *main_game_w;

signals:
  void showMenu();

  void menu_clicked();

  void start_game_widget();
  void go_game_widget();
  void pause_widget();
  void game_over_widget();

public slots: 
  virtual void update_all() = 0;

  void emitShowMenu(){
      emit showMenu();
  }

};

//////////////////////////////////////////
//// QtTetrisGameView
//////////////////////////////////////////

class QtTetrisGameView : public QtGameView {
  Q_OBJECT
public:
  explicit QtTetrisGameView(QWidget *parent = nullptr);
  ~QtTetrisGameView();

  void draw(const GameInfo_t &stats) override;
  void updateCurrentState(const UserAction_t& signal); 

private:
  params_t prms_;
    void prms_init(params_t &prms);

  TetrisController *tetris_cont_;
  TetrisAdditionalGameWidget *additional_game_w;

public slots:
  void update_all() override;
};

////////////////////////////////////////
// QtSnakeGameView
////////////////////////////////////////

class QtSnakeGameView : public QtGameView {
  Q_OBJECT
public:
    explicit QtSnakeGameView(QWidget *parent);
  ~QtSnakeGameView();

  void draw(const GameInfo_t &stats) override;
  GameInfo_t updateCurrentState(const UserAction_t& signal); 

  void connect_overlap_victory();
private:
  SnakeController *snake_cont_;
  SnakeAdditionalGameWidget *additional_game_w;

signals:
  void victory_widget();

public slots:
  void update_all() override;
};

} // namespace s21

#endif // GAMEWIDGET_H

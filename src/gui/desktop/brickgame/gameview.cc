#include "gameview.h"

#include "mainwindow.h"

namespace s21 {

////////////////////////////////////////
// QtGameView
////////////////////////////////////////

QtGameView::QtGameView(QWidget *parent) : QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);

  general_layout_ = new QHBoxLayout(this);
  main_game_w = new MainGameWidget(this);
  general_layout_->addWidget(main_game_w);

  connect_overlap();
}

void QtGameView::connect_overlap() {
  connect(this, &QtGameView::start_game_widget, main_,
          &MainWindow::showGameStart);
  connect(this, &QtGameView::go_game_widget, main_,
          &MainWindow::showGameWidget);

  connect(this, &QtGameView::pause_widget, main_, &MainWindow::showPause);
  connect(this, &QtGameView::game_over_widget, main_,
          &MainWindow::showGameOver);
}

////////////////////////////////////////
// QtTetrisGameView
////////////////////////////////////////

QtTetrisGameView::QtTetrisGameView(QWidget *parent) : QtGameView(parent) {
  tetris_cont_ = new TetrisController();
  prms_init(prms_);

  additional_game_w = new TetrisAdditionalGameWidget(this);
  general_layout_->addWidget(additional_game_w);

  connect(additional_game_w, &TetrisAdditionalGameWidget::showMenu, this,
          &QtGameView::emitShowMenu);  // ща
}

QtTetrisGameView::~QtTetrisGameView() { delete tetris_cont_; };

void QtTetrisGameView ::prms_init(params_t &prms) {
  tetris_cont_->prms_init(prms);
}

void QtTetrisGameView::update_all() {
  updateCurrentState(Stub);
  draw(*prms_.stats_);
}

void QtTetrisGameView::draw(const GameInfo_t &stats) {
  if (stats.pause == BEGGINING) {
    emit start_game_widget();
  } else if (stats.pause == STOP) {
    emit pause_widget();
  } else if (stats.pause == GO) {
    emit go_game_widget();

    main_game_w->setStats(stats);
    additional_game_w->setStats(stats);
    update();

  } else if (stats.pause == EXIT) {
    emit game_over_widget();
  }
}

void QtTetrisGameView::updateCurrentState(const UserAction_t &signal) {
  tetris_cont_->updateCurrentState(prms_, signal);
}

////////////////////////////////////////
// QtSnakeGameView
////////////////////////////////////////

QtSnakeGameView::QtSnakeGameView(QWidget *parent) : QtGameView(parent) {
  snake_cont_ = new SnakeController;

  additional_game_w = new SnakeAdditionalGameWidget(this);
  general_layout_->addWidget(additional_game_w);

  connect_overlap_victory();
  connect(additional_game_w, &AdditionalGameWidget::showMenu, this,
          &QtGameView::emitShowMenu);  // ща
}

QtSnakeGameView::~QtSnakeGameView() { delete snake_cont_; };

void QtSnakeGameView::connect_overlap_victory() {
  connect(this, &QtSnakeGameView::victory_widget, main_,
          &MainWindow::showVictory);
}

GameInfo_t QtSnakeGameView::updateCurrentState(const UserAction_t &signal) {
  return snake_cont_->updateCurrentState(signal);
}

void QtSnakeGameView::update_all() {
  GameInfo_t stats = updateCurrentState(Stub);
  draw(stats);
}

void QtSnakeGameView::draw(const GameInfo_t &stats) {
  if (stats.pause == BEGGINING) {
    emit start_game_widget();
  } else if (stats.pause == STOP) {
    emit pause_widget();
  } else if (stats.pause == GO) {
    emit go_game_widget();

    main_game_w->setStats(stats);
    additional_game_w->setStats(stats);
    update();
  }
  if (stats.pause == WIN) {
    emit victory_widget();
  } else if (stats.pause == EXIT) {
    emit game_over_widget();
  }
}

}  // namespace s21

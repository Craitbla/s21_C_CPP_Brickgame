#include "mainwindow.h"

#include "gameview.h"

namespace s21 {

OverlappingWidget::OverlappingWidget(QWidget *parent) : QWidget(parent) {
  layout_ = new QVBoxLayout(this);
  layout_->setAlignment(Qt::AlignCenter);

  text_ = new QLabel("\0", this);
  layout_->addWidget(text_);

  (*this).setLayout(layout_);
}

void OverlappingWidget::set_text_pause() { text_->setText("PAUSE"); }
void OverlappingWidget::set_text_game_start() {
  text_->setText("PRESS ENTER\nTO START THE GAME");
}
void OverlappingWidget::set_text_game_over() { text_->setText("GAME OVER"); };
void OverlappingWidget::set_text_victory() { text_->setText("YOU WON!!!"); };

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setFixedSize(1200, 800);
  stackedWidget = new QStackedWidget(this);
  setCentralWidget(stackedWidget);

  menu = new MenuWidget(this);
  overlap = new OverlappingWidget(this);

  stackedWidget->addWidget(overlap);

  stackedWidget->addWidget(menu);

  stackedWidget->setCurrentWidget(menu);

  connect(menu, &MenuWidget::startGameTetris, this, &MainWindow::addGameTetris);
  connect(menu, &MenuWidget::startGameSnake, this, &MainWindow::addGameSnake);

  connect(menu, &MenuWidget::exit, this, &MainWindow::exitClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::showMenu() {
  if (name_of_the_game_ == TETRIS) {
    stackedWidget->removeWidget(tetris_game);
    delete tetris_game;

  } else if (name_of_the_game_ == SNAKE) {
    stackedWidget->removeWidget(snake_game);
    delete snake_game;
  }

  stackedWidget->setCurrentWidget(menu);
}

void MainWindow::addGameTetris() {
  name_of_the_game_ = TETRIS;
  if (tetris_game != nullptr) {
    tetris_game = new QtTetrisGameView(this);
    stackedWidget->addWidget(tetris_game);
    connect(tetris_game, &QtGameView::showMenu, this, &MainWindow::showMenu);

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, tetris_game,
            &QtTetrisGameView::update_all);
    update_timer->start(25);
  }
}

void MainWindow::addGameSnake() {
  name_of_the_game_ = SNAKE;
  if (snake_game != nullptr) {
    snake_game = new QtSnakeGameView(this);
    stackedWidget->addWidget(snake_game);
    connect(snake_game, &QtGameView::showMenu, this, &MainWindow::showMenu);

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, snake_game,
            &QtSnakeGameView::update_all);
    update_timer->start(25);
  }
}

void MainWindow::showGameWidget() {
  if (name_of_the_game_ == TETRIS) {
    stackedWidget->setCurrentWidget(tetris_game);
  } else if (name_of_the_game_ == SNAKE) {
    stackedWidget->setCurrentWidget(snake_game);
  }
}

void MainWindow::showGameStart() {
  overlap->set_text_game_start();
  stackedWidget->setCurrentWidget(overlap);
}

void MainWindow::showPause() {
  overlap->set_text_pause();
  stackedWidget->setCurrentWidget(overlap);
}

void MainWindow::showGameOver() {
  overlap->set_text_game_over();
  stackedWidget->setCurrentWidget(overlap);

  if (name_of_the_game_ == TETRIS) {
    delete tetris_game;

  } else if (name_of_the_game_ == SNAKE) {
    delete snake_game;
  }

  QTimer::singleShot(2000, this,
                     [this]() { stackedWidget->setCurrentWidget(menu); });
}

void MainWindow::showVictory() {
  overlap->set_text_victory();
  stackedWidget->setCurrentWidget(overlap);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  UserAction_t action = get_signal(event->key());

  if (name_of_the_game_ == TETRIS) {
    tetris_game->updateCurrentState(action);
  } else if (name_of_the_game_ == SNAKE) {
    snake_game->updateCurrentState(action);
  }
}

UserAction_t MainWindow::get_signal(const int user_input) {
  UserAction_t key = Stub;

  switch (user_input) {
    case Qt::Key_Down:
      key = Down;
      break;
    case Qt::Key_Up:
      key = Up;
      break;
    case Qt::Key_Left:
      key = Left;
      break;
    case Qt::Key_Right:
      key = Right;
      break;
    case Qt::Key_Escape:
      key = Terminate;
      break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
      key = Start;
      break;
    case Qt::Key_P:
      key = Pause;
      break;
    case Qt::Key_Space:
      key = Action;
      break;
    default:
      key = Stub;
      break;
  }

  return key;
}
}  // namespace s21

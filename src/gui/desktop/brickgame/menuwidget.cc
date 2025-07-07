

#include "menuwidget.h"

namespace s21 {

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent) {
  QLabel *label = new QLabel("Brickgame от craitbla!");
  QPushButton *startTetrisButton = new QPushButton("Тетрис");
  QPushButton *startSnakeButton = new QPushButton("Змейка");
  QPushButton *exitButton = new QPushButton("Выход");

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setAlignment(Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(label);
  layout->addWidget(startTetrisButton);
  layout->addWidget(startSnakeButton);

  layout->addWidget(exitButton);

  layout->addStretch();
  setLayout(layout);

  connect(startTetrisButton, &QPushButton::clicked, this,
          &MenuWidget::startTetrisClicked);
  connect(startSnakeButton, &QPushButton::clicked, this,
          &MenuWidget::startSnakeClicked);

  connect(exitButton, &QPushButton::clicked, this, &MenuWidget::exitClicked);
}

}  // namespace s21

#ifndef MENUWIDGET_H
#define MENUWIDGET_H


#include "common_libs.h"


namespace s21 {

class MenuWidget : public QWidget {
  Q_OBJECT
public:
  explicit MenuWidget(QWidget *parent = nullptr);

signals:
  void startGameTetris();
  void startGameSnake();

  void exit();

public slots: 
  void startTetrisClicked() { emit startGameTetris(); }
  void startSnakeClicked() { emit startGameSnake(); }

  void  exitClicked() { emit  exit();}
};

} // namespace s21

#endif // MENUWIDGET_H

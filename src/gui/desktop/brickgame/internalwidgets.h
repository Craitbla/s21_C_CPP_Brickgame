#ifndef INTERNALWIDGETS_H
#define INTERNALWIDGETS_H

#include "common_libs.h"

namespace s21 {

#define RECT_SIZE 35

#define NUM_OF_ROWS 12
#define NUM_OF_COLS 22

#define NUM_OF_COLS_ROWS_NEXT_FIG 4

#define QT_FIELD_X 100
#define QT_NEXT_FIGURE_Y 170

////////////////////////////////////////
// InternalWidget
////////////////////////////////////////

class InternalWidget : public QWidget {
  Q_OBJECT
public:
  explicit InternalWidget(QWidget *parent = nullptr);
  ~InternalWidget();

   void colors_init();
   void setStats(const GameInfo_t& newStats);
   void painter_init(QPainter &painter);
   virtual void paintEvent(QPaintEvent */*event*/) = 0;
protected:
   QVector<QColor> colors_vector;
   QVBoxLayout *layout_;
   GameInfo_t *stats_;
};

////////////////////////////////////////
// MainGameWidget
////////////////////////////////////////

class MainGameWidget : public InternalWidget {
  Q_OBJECT
public:
  explicit MainGameWidget(QWidget *parent = nullptr);

   void createRectsField();
    void draw_main_field(QPainter &painter);
    void paintEvent(QPaintEvent */*event*/) override;

private:
  QVector<QVector<QRect>> rects_field;
};

////////////////////////////////////////
// AdditionalGameWidget
////////////////////////////////////////

class AdditionalGameWidget : public InternalWidget {
  Q_OBJECT

protected:
    QLabel *high_score_text_;
    QLabel *score_text_;
    QLabel *level_text_;
    QPushButton *end_button;

public:
  explicit AdditionalGameWidget(QWidget *parent = nullptr);

   void updateAdditionalData();
    void createLabels();
    void createEndButton();

signals:
    void showMenu();
public slots:
    void emitShowMenu(){
        emit showMenu();
    };
};
////////////////////////////////////////
// SnakeAdditionalGameWidget
////////////////////////////////////////

class SnakeAdditionalGameWidget : public AdditionalGameWidget {
  Q_OBJECT
public:
  explicit SnakeAdditionalGameWidget(QWidget *parent = nullptr);

     void paintEvent(QPaintEvent */*event*/) override;
};

////////////////////////////////////////
// TetrisAdditionalGameWidget
////////////////////////////////////////

class TetrisAdditionalGameWidget : public AdditionalGameWidget {
  Q_OBJECT
public:
  explicit TetrisAdditionalGameWidget(QWidget *parent = nullptr);

    void createTextNextFig();
    void createRectsNextFig();

      void draw_next_fig(QPainter &painter);

          void paintEvent(QPaintEvent */*event*/) override;

private:
        QLabel *next_fig_text;
      QVector<QVector<QRect>> rects_new_fig;
};


}

#endif // INTERNALWIDGETS_H

#include "internalwidgets.h"

namespace s21 {

////////////////////////////////////////
// InternalWidget
////////////////////////////////////////

InternalWidget::InternalWidget(QWidget *parent) : QWidget(parent) {
  colors_init();
  stats_init(&stats_);
  layout_ = new QVBoxLayout(this);
}

InternalWidget::~InternalWidget() {
  free(stats_);
  stats_ = NULL;
}

void InternalWidget::colors_init() {
  colors_vector.resize(10);
  colors_vector[0] = Qt::black;                        // Void
  colors_vector[1] = QColorConstants::Svg::lightblue;  // I
  colors_vector[2] = Qt::yellow;                       // O
  colors_vector[3] = Qt::blue;                         // J
  colors_vector[4] = QColorConstants::Svg::orange;     // L
  colors_vector[5] = Qt::green;                        // S //  SNAKE_BODY
  colors_vector[6] = QColorConstants::Svg::purple;     // T
  colors_vector[7] = Qt::red;                          // Z //  APPLE
  colors_vector[8] = QColorConstants::Svg::darkgreen;  //   //  SNAKE_HEAD
  colors_vector[9] = Qt::white;                        // Wall
}

void InternalWidget::setStats(const GameInfo_t &new_stats) {
  *stats_ = new_stats;
  update();
}

void InternalWidget::painter_init(QPainter &painter) {
  painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
  painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
}

////////////////////////////////////////
// MainGameWidget
////////////////////////////////////////

MainGameWidget::MainGameWidget(QWidget *parent) : InternalWidget(parent) {
  createRectsField();
}

void MainGameWidget::paintEvent(QPaintEvent * /*event*/) {
  QPainter painter(this);
  painter_init(painter);
  draw_main_field(painter);
}

void MainGameWidget::createRectsField() {
  QVector<QRect> rectsLine;

  for (int col = 0; col < NUM_OF_COLS; col++) {
    rectsLine.clear();
    for (int row = 0; row < NUM_OF_ROWS; row++) {
      rectsLine.append(QRect(QT_FIELD_X + RECT_SIZE * row, RECT_SIZE * col,
                             RECT_SIZE, RECT_SIZE));
    }
    rects_field.append(rectsLine);
  }
};

void MainGameWidget::draw_main_field(QPainter &painter) {
  for (const auto &Rects : rects_field) {
    for (const QRect &rect : Rects) {
      painter.drawRect(rect);
    }
  }

  QColor color;
  QRect rect;
  int design = EMPTY_TILE;
  ;
  for (int col = 0; col < NUM_OF_COLS; col++) {
    for (int row = 0; row < NUM_OF_ROWS; row++) {
      design = stats_->field[col][row];
      color = colors_vector[design];

      rect = rects_field[col][row];
      painter.fillRect(rect, color);
    }
  }
}

////////////////////////////////////////
// AdditionalGameWidget
////////////////////////////////////////
AdditionalGameWidget::AdditionalGameWidget(QWidget *parent)
    : InternalWidget(parent) {
  createLabels();
}

void AdditionalGameWidget::createLabels() {
  high_score_text_ = new QLabel("\n\nHIGH SCORE\n", this);
  score_text_ = new QLabel("SCORE\n", this);
  level_text_ = new QLabel("LEVEL\n", this);

  layout_->addWidget(high_score_text_);
  layout_->addWidget(score_text_);
  layout_->addWidget(level_text_);
}

void AdditionalGameWidget::createEndButton() {
  end_button = new QPushButton("Завершить игру", this);

  layout_->addStretch();
  layout_->addWidget(end_button);
  connect(end_button, &QPushButton::clicked, this,
          &AdditionalGameWidget::emitShowMenu);
}

void AdditionalGameWidget::updateAdditionalData() {
  high_score_text_->setText(QString("HIGH SCORE\n%1").arg(stats_->high_score));
  score_text_->setText(QString("SCORE\n%1").arg(stats_->score));
  level_text_->setText(QString("LEVEL\n%1").arg(stats_->level));
}

////////////////////////////////////////
// SnakeAdditionalGameWidget
////////////////////////////////////////

SnakeAdditionalGameWidget::SnakeAdditionalGameWidget(QWidget *parent)
    : AdditionalGameWidget(parent) {
  createEndButton();
  setLayout(layout_);
}

void SnakeAdditionalGameWidget::paintEvent(QPaintEvent * /*event*/) {
  updateAdditionalData();
}

////////////////////////////////////////
// TetrisAdditionalGameWidget
////////////////////////////////////////

TetrisAdditionalGameWidget::TetrisAdditionalGameWidget(QWidget *parent)
    : AdditionalGameWidget(parent) {
  createTextNextFig();
  createRectsNextFig();
  createEndButton();
  setLayout(layout_);
}

void TetrisAdditionalGameWidget::createTextNextFig() {
  next_fig_text = new QLabel("NEXT FIGURE", this);
  layout_->addWidget(next_fig_text);
}

void TetrisAdditionalGameWidget::createRectsNextFig() {
  QVector<QRect> rectsLine;
  for (int col = 0; col < NUM_OF_COLS_ROWS_NEXT_FIG; col++) {
    rectsLine.clear();
    for (int row = 0; row < NUM_OF_COLS_ROWS_NEXT_FIG; row++) {
      rectsLine.append(QRect(RECT_SIZE * row,
                             QT_NEXT_FIGURE_Y + RECT_SIZE * col, RECT_SIZE,
                             RECT_SIZE));
    }
    rects_new_fig.append(rectsLine);
  }
}

void TetrisAdditionalGameWidget::draw_next_fig(QPainter &painter) {
  for (const auto &Rects : rects_new_fig) {
    for (const QRect &rect : Rects) {
      painter.drawRect(rect);
    }
  }

  QColor color;
  QRect rect;
  int design = EMPTY_TILE;

  for (int col = 0; col < NUM_OF_COLS_ROWS_NEXT_FIG; col++) {
    for (int row = 0; row < NUM_OF_COLS_ROWS_NEXT_FIG; row++) {
      design = stats_->next[col][row];
      color = colors_vector[design];

      rect = rects_new_fig[col][row];
      painter.fillRect(rect, color);
    }
  }
}

void TetrisAdditionalGameWidget::paintEvent(QPaintEvent * /*event*/) {
  QPainter painter(this);
  painter_init(painter);
  updateAdditionalData();
  draw_next_fig(painter);
}

}  // namespace s21

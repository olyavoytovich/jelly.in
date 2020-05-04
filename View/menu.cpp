#include "menu.h"

Menu::Menu(AbstractGameController* game_controller, QWidget* parent)
    : QWidget(parent), game_controller_(game_controller) {}

void Menu::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);

  scale_ =
      std::min(static_cast<double>(event->size().width()) / kVisiblePart.x(),
               static_cast<double>(event->size().height()) / kVisiblePart.y());

  shift_ = QPoint(event->size().width(), event->size().height());
  shift_ = (shift_ / scale_ - kVisiblePart) / 2.0;

  if (!background_.isNull()) {
    scaled_background_ =
        background_.scaled(event->size(), Qt::KeepAspectRatioByExpanding);
  }
  
  if (!main_part_.isNull()) {
    scaled_main_part_ = main_part_.scaled(event->size(), Qt::KeepAspectRatio);
  }
}

void Menu::paintEvent(QPaintEvent*) {
  QPainter painter(this);

  // отрисовывается фоновая часть меню
  painter.drawImage(QPoint(0, 0), scaled_background_);

  // отрисовывается основная часть меню
  painter.translate(shift_ * scale_);

  painter.drawImage(QPoint(0, 0), scaled_main_part_);
}

QRect Menu::PositionRectangle(int x, int y, int width, int height) {
  int new_width = static_cast<int>((kVisiblePart.x() * scale_) / 16);
  int new_height = static_cast<int>((kVisiblePart.y() * scale_) / 9);
  QPoint button_shift = shift_ * scale_;

  return QRect(x * new_width + button_shift.x(),
               y * new_height + button_shift.y(),
               width * new_width,
               height * new_height);
}

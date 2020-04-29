#include "menu.h"

Menu::Menu(QRect boundary_rect, AbstractGameController *game_controller,
           QWidget *parent)
    : QDialog(parent), game_controller_(game_controller) {
  setGeometry(boundary_rect);
  setMouseTracking(true);
}

void Menu::AddButton(std::shared_ptr<Button> button) {
  buttons_.push_back(button);
}

void Menu::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton) {
    return;
  }

  QPoint position = ((event->pos() - shift_) / scale_);
  for (auto button : buttons_) {
    if (button->GetRectangle().contains(position)) {
      button->Pressed();
      was_pressed_ = button;
      PressedButton(button);
      break;
    }
  }
  repaint();
}

void Menu::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton) {
    return;
  }

  if (was_pressed_ != nullptr) {
    was_pressed_->NotPressed();
    was_pressed_ = nullptr;
  }
  repaint();
}

void Menu::mouseMoveEvent(QMouseEvent *event) {
  QPoint position = ((event->pos() - shift_) / scale_);

  if (was_covered_ != nullptr &&
      !was_covered_->GetRectangle().contains(position)) {
    was_covered_->NotHovered();
    was_covered_ = nullptr;
  }

  for (auto button : buttons_) {
    if (button->GetRectangle().contains(position)) {
      button->Hovered();
      was_covered_ = button;
      break;
    }
  }
  repaint();
}

void Menu::resizeEvent(QResizeEvent *event) {
  scale_ =
      std::min(static_cast<double>(event->size().width()) / kVisiblePart.x(),
               static_cast<double>(event->size().height()) / kVisiblePart.y());
  shift_ =
      QPoint(static_cast<int>(
                 (event->size().width() - kVisiblePart.x() * scale_) / 2.0),
             static_cast<int>(
                 (event->size().height() - kVisiblePart.y() * scale_) / 2.0));
  small_background_ =
      big_background_.scaled(event->size(), Qt::KeepAspectRatioByExpanding);
}

void Menu::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  // отрисовывается фоновая часть меню
  painter.drawImage(QPoint(0, 0), small_background_);

  // отрисовывается основная часть меню
  painter.translate(shift_);
  painter.scale(scale_, scale_);
  painter.drawImage(QPoint(0, 0), main_part_);

  for (auto button : buttons_) {
    button->Draw(&painter);
  }
}

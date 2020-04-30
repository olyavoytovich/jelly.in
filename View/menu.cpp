#include "menu.h"

Menu::Menu(const QRect& boundary_rect,
           AbstractGameController* game_controller,
           QWidget* parent)
    : QDialog(parent),
      game_controller_(game_controller) {
  setGeometry(boundary_rect);
  setMouseTracking(true);
}

void Menu::AddButton(const std::shared_ptr<Button>& button) {
  buttons_.push_back(button);
}

void Menu::mousePressEvent(QMouseEvent* event) {
  if (event->button() != Qt::LeftButton) {
    return;
  }
  repaint();

  QPoint position = ((event->pos() - shift_) / scale_);
  for (const auto& button : buttons_) {
    if (button->GetRectangle().contains(position)) {
      button->Pressed();
      pressed_button_ = button;
      PressedButton(button);
      break;
    }
  }
}

void Menu::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() != Qt::LeftButton) {
    return;
  }
  repaint();

  if (pressed_button_ != nullptr) {
    pressed_button_->NotPressed();
    pressed_button_ = nullptr;
  }
}

void Menu::mouseMoveEvent(QMouseEvent* event) {
  QPoint position = ((event->pos() - shift_) / scale_);
  repaint();

  if (hovered_button_ != nullptr &&
      !hovered_button_->GetRectangle().contains(position)) {
    hovered_button_->NotHovered();
    hovered_button_ = nullptr;
  }

  for (const auto& button : buttons_) {
    if (button->GetRectangle().contains(position)) {
      button->Hovered();
      hovered_button_ = button;
      break;
    }
  }
}

void Menu::resizeEvent(QResizeEvent* event) {
  scale_ =
      std::min(static_cast<double>(event->size().width()) / kVisiblePart.x(),
               static_cast<double>(event->size().height()) / kVisiblePart.y());
  shift_ =
      QPoint(static_cast<int>(
                 (event->size().width() - kVisiblePart.x() * scale_) / 2.0),
             static_cast<int>(
                 (event->size().height() - kVisiblePart.y() * scale_) / 2.0));
  scaled_background_ =
      background_.scaled(event->size(), Qt::KeepAspectRatioByExpanding);
}

void Menu::paintEvent(QPaintEvent*) {
  QPainter painter(this);

  // отрисовывается фоновая часть меню
  painter.drawImage(QPoint(0, 0), scaled_background_);

  // отрисовывается основная часть меню
  painter.translate(shift_);
  painter.scale(scale_, scale_);
  painter.drawImage(QPoint(0, 0), main_part_);

  for (const auto& button : buttons_) {
    button->Draw(&painter);
  }
}

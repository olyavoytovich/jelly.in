#include "menu.h"

Menu::Menu(AbstractGameController* game_controller, QWidget* parent)
    : QWidget(parent), game_controller_(game_controller),
      audio_manager_(std::make_shared<AudioManager>()) {}

void Menu::SetGeneralVolume(int general_volume) {
  audio_manager_->SetGeneralVolume(general_volume);
}

void Menu::SetCurrentVolume(int current_volume) {
  audio_manager_->SetCurrentVolume(current_volume);
}

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

QRect Menu::PositionRectangle(int x, int y, int width, int height) const {
  double new_width = kVisiblePart.x() * scale_ / 16;
  double new_height = kVisiblePart.y() * scale_ / 9;
  QPoint button_shift = shift_ * scale_;

  return QRect(static_cast<int>(x * new_width) + button_shift.x(),
               static_cast<int>(y * new_height) + button_shift.y(),
               static_cast<int>(width * new_width),
               static_cast<int>(height * new_height));
}

void Menu::MakeEqualFontSize(const std::vector<Button*>& buttons) const {
  int font_size = height();
  for (const auto& button : buttons) {
    if (button->GetFontSize() > 0) {
      font_size = std::min(font_size, button->GetFontSize());
    }
  }
  for (const auto& button : buttons) {
    button->SetFontSize(font_size);
  }
}

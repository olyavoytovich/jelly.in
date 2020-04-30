#include "button.h"

Button::Button(const QRect& rectangle,
               const QImage& flat_image,
               const QImage& pressed_image,
               const QImage& hovered_image)
    : boundary_rectangle_(rectangle),
      flat_image_(flat_image),
      pressed_image_(pressed_image),
      hovered_image_(hovered_image),
      current_image_(flat_image) {}

QRect Button::GetRectangle() const {
  return boundary_rectangle_;
}

void Button::Draw(QPainter* painter) {
  painter->drawImage(boundary_rectangle_, current_image_);
}

void Button::Pressed() {
  current_image_ = pressed_image_;
}

void Button::NotPressed() {
  if (!is_hovered_) {
    current_image_ = flat_image_;
  } else {
    current_image_ = hovered_image_;
  }
}

void Button::Hovered() {
  current_image_ = hovered_image_;
  is_hovered_ = true;
}

void Button::NotHovered() {
  if (current_image_ != pressed_image_) {
    current_image_ = flat_image_;
  }
  is_hovered_ = false;
}

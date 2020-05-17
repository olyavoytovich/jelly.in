#include "label.h"

Label::Label(const QString& text, QWidget* parent)
    : QLabel(text, parent),
      current_font_("Bahnschrift"),
      bounding_rectangle_(rect()) {
  current_font_.setPixelSize(height());
  setFont(current_font_);
  setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

int Label::GetFontSize() const {
  double height = bounding_rectangle_.height();
  QRect text_bounding = QFontMetrics(current_font_).boundingRect(text());
  double scale_factor = height / text_bounding.height();
  int font_size = static_cast<int>(current_font_.pixelSize() * scale_factor);
  return FontSizeToFitWidth(bounding_rectangle_.width(), font_size);
}

void Label::SetFontColor(const QColor& color) {
  QPalette current_palette = palette();
  current_palette.setColor(foregroundRole(), color);
  setPalette(current_palette);
}

void Label::SetFontSize(int font_size) {
  if (font_size <= 0) {
    font_size = GetFontSize();
  }
  current_font_.setPixelSize(font_size);
  setFont(current_font_);
  repaint();
}

void Label::SetTextBounding(const QRect& bounding_rectangle) {
  bounding_rectangle_ = bounding_rectangle;
}

int Label::FontSizeToFitWidth(int target_width, int current_size) const {
  QFont font = current_font_;
  font.setPixelSize(current_size);
  while (QFontMetrics(font).boundingRect(text()).width() > target_width) {
    font.setPixelSize(font.pixelSize() - 1);
  }
  return font.pixelSize();
}

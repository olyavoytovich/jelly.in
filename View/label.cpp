#include "label.h"

Label::Label(const QString& text, QWidget* parent)
    : QLabel(text, parent),
      current_font_("Bahnschrift"),
      bounding_rectangle_(rect()) {
  current_font_.setPixelSize(height());
  setFont(current_font_);
}

int Label::GetFontSize() const {
  double width = bounding_rectangle_.width();
  double height = bounding_rectangle_.height();
  QRect text_bounding = QFontMetrics(current_font_).boundingRect(text());
  double scale_factor =
      std::min(width / text_bounding.width(), height / text_bounding.height());
  return static_cast<int>(current_font_.pixelSize() * scale_factor);
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

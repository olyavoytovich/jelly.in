#ifndef BUTTON_H_
#define BUTTON_H_

#include <QImage>
#include <functional>

class Button {
 public:
  Button(QRect rectangle, QImage flat_image, QImage pressed_image,
         QImage hovered_image);

  QRect GetRectangle();

  void Draw(QPainter* painter);

  void Pressed();
  void NotPressed();

  void Hovered();
  void NotHovered();

 private:
  QRect boundary_rectangle_;

  bool is_hovered_ = false;

  QImage flat_image_;
  QImage pressed_image_;
  QImage hovered_image_;
  QImage current_image_;
};

#endif  // BUTTON_H_

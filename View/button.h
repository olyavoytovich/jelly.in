#ifndef VIEW_BUTTON_H_
#define VIEW_BUTTON_H_

#include <QImage>
#include <QPainter>

class Button {
 public:
  Button(const QRect& rectangle,
         const QImage& flat_image,
         const QImage& pressed_image,
         const QImage& hovered_image);

  QRect GetRectangle() const;

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

#endif  // VIEW_BUTTON_H_

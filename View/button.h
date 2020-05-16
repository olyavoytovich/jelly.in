#ifndef VIEW_BUTTON_H_
#define VIEW_BUTTON_H_

#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <memory>
#include <utility>

#include "label.h"

struct ImageSet {
  explicit ImageSet(const QString& name);

  QImage flat;
  QImage clicked;
  QImage hovered;

  QImage flat_scaled;
  QImage clicked_scaled;
  QImage hovered_scaled;
};

class Button : public QPushButton {
 public:
  explicit Button(std::shared_ptr<ImageSet> image_set,
                  QWidget* parent = nullptr,
                  const QString& button_text = "");
  ~Button() override = default;

  int GetFontSize() const;

  void SetText(const QString& text);
  void SetFontSize(int text_size);
  void SetRectangle(const QRect& rectangle);
  void SetLabelColor(const QColor& color);

 private:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;

 private:
  // Размер отступа текста от краев кнопок в процентах от её ширины и высоты
  const double kTextPadding = 0.05;

 private:
  enum class Status {
    kFlat,
    kClicked,
    kHovered
  } status_;

  std::shared_ptr<ImageSet> image_set_;

  Label* text_ = nullptr;
};

#endif  // VIEW_BUTTON_H_

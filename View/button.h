#ifndef VIEW_BUTTON_H_
#define VIEW_BUTTON_H_

#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <memory>

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
                  QWidget* parent = nullptr);
  ~Button() override = default;

 private:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;

 private:
  enum class Status { kFlat, kClicked, kHovered } status_;
  std::shared_ptr<ImageSet> image_set_;
};

#endif  // VIEW_BUTTON_H_
#ifndef VIEW_LABEL_H_
#define VIEW_LABEL_H_

#include <QFont>
#include <QFontMetrics>
#include <QLabel>
#include <QPalette>
#include <algorithm>

class Label : public QLabel {
 public:
  explicit Label(const QString& text, QWidget* parent = nullptr);

  int GetFontSize() const;

  void SetFontColor(const QColor& color);
  void SetFontSize(int font_size);
  void SetTextBounding(const QRect& bounding_rectangle);

 private:
  QFont current_font_;
  QRect bounding_rectangle_;
};

#endif  // VIEW_LABEL_H_

#ifndef VIEW_MENU_H_
#define VIEW_MENU_H_

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>
#include <algorithm>
#include <memory>
#include <vector>

#include "Controller/abstract_game_controller.h"
#include "button.h"

class Menu : public QWidget {
 Q_OBJECT

 public:
  explicit Menu(AbstractGameController* game_controller,
                QWidget* parent = nullptr);
  ~Menu() override = default;

 protected:
  QImage background_;
  QImage scaled_background_;
  QImage main_part_;
  QImage scaled_main_part_;

  void resizeEvent(QResizeEvent*) override;
  void paintEvent(QPaintEvent*) override;

  QRect PositionRectangle(int x, int y, int width, int height);

  AbstractGameController* game_controller_ = nullptr;

 private:
  // Ширина и Высота основной части меню
  const QPoint kVisiblePart = QPoint(4096, 2304);

  double scale_ = 1;
  QPoint shift_;
};

#endif  // VIEW_MENU_H_

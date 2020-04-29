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

class Menu : public QDialog {
  Q_OBJECT

 public:
  Menu(QRect boundary_rectangle, AbstractGameController *game_controller,
       QWidget *parent = nullptr);

 protected:
  virtual void PressedButton(std::shared_ptr<Button> button) = 0;

  void AddButton(std::shared_ptr<Button> button);

 protected:
  QImage big_background_;
  QImage small_background_;
  QImage main_part_;

  AbstractGameController *game_controller_ = nullptr;

 private:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  void resizeEvent(QResizeEvent *) override;
  void paintEvent(QPaintEvent *) override;

 private:
  // Ширина и Высота основной части меню
  const QPoint kVisiblePart = QPoint(512, 288);

 private:
  std::shared_ptr<Button> was_pressed_ = nullptr;
  std::shared_ptr<Button> was_covered_ = nullptr;

  std::vector<std::shared_ptr<Button>> buttons_;

  double scale_;

  QPoint shift_;
};

#endif  // VIEW_MENU_H_

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
  explicit Menu(AbstractGameController* game_controller,
                QWidget* parent = nullptr);
  ~Menu() override = default;

 protected:
  virtual void PressedButton(const std::shared_ptr<Button>& button) = 0;

  std::shared_ptr<Button> CreateButton(const QString& name, int x, int y,
                                       int width, int height);

 protected:
  QImage background_;
  QImage scaled_background_;
  QImage main_part_;

  AbstractGameController* game_controller_ = nullptr;

 private:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;

  void resizeEvent(QResizeEvent*) override;
  void paintEvent(QPaintEvent*) override;

 private:
  // Ширина и Высота основной части меню
  const QPoint kVisiblePart = QPoint(512, 288);

 private:
  std::shared_ptr<Button> pressed_button_ = nullptr;
  std::shared_ptr<Button> hovered_button_ = nullptr;

  std::vector<std::shared_ptr<Button>> buttons_;

  double scale_;

  QPoint shift_;
};

#endif  // VIEW_MENU_H_

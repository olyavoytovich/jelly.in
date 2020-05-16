#ifndef VIEW_MENU_H_
#define VIEW_MENU_H_

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>
#include <algorithm>
#include <memory>
#include <vector>

#include "Controller/abstract_game_controller.h"
#include "Model/audio_manager.h"
#include "button.h"

class Menu : public QWidget {
  Q_OBJECT

 public:
  explicit Menu(AbstractGameController* game_controller,
                QWidget* parent = nullptr);
  ~Menu() override = default;

 protected:
  void resizeEvent(QResizeEvent*) override;
  void paintEvent(QPaintEvent*) override;

  QRect PositionRectangle(int x, int y, int width, int height) const;
  void MakeEqualFontSize(const std::vector<Button*>& buttons) const;

 protected:
  QImage background_;
  QImage scaled_background_;
  QImage main_part_;
  QImage scaled_main_part_;

  double scale_ = 1;
  QPoint shift_ = QPoint(0, 0);
  AbstractGameController* game_controller_ = nullptr;

  std::shared_ptr<AudioManager> audio_manager_;

 private:
  // Ширина и Высота основной части меню
  const QPoint kVisiblePart = QPoint(4096, 2304);
};

#endif  // VIEW_MENU_H_

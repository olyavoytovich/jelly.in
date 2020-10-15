#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QMainWindow>
#include <QObject>
#include <QPaintEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <memory>
#include <vector>

#include "Controller/abstract_game_controller.h"
#include "Model/entity.h"
#include "Model/map.h"
#include "View/abstract_view.h"

class View : public QMainWindow, public AbstractView {
  Q_OBJECT

 public:
  explicit View(AbstractGameController* game_controller);
  ~View() override = default;

 private:
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

  void Draw(QPainter* painter);
  void DrawObject(QPainter* painter,
                  std::shared_ptr<GameObject> game_objects,
                  std::shared_ptr<Map> map) const;

 private:
  const int kFrameInterval = 16;

  const float kSunflowerWidthPercent = 2.0;
  const float kSunflowerHeightPercent = 4.4;

 private:
  AbstractGameController* game_controller_ = nullptr;
  int timer_id_;
};

#endif  // VIEW_VIEW_H_

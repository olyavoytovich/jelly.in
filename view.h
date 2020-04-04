#ifndef VIEW_H_
#define VIEW_H_

#include <QMainWindow>
#include <QObject>
#include <QPaintEvent>
#include <QTimer>

#include "abstract_game_controller.h"
#include "abstract_view.h"

class View : public QMainWindow, public AbstractView {
  Q_OBJECT

 public:
  explicit View(AbstractGameController* game_controller);
  ~View() override = default;

 private:
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent* event) override;

 private:
  AbstractGameController* game_controller_ = nullptr;
  int timer_id_;
  const int kFrameInterval = 16;
};

#endif  // VIEW_H_

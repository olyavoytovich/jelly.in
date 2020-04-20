#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QMainWindow>
#include <QObject>
#include <QPaintEvent>
#include <QTimer>

#include "Controller/abstract_game_controller.h"
#include "View/abstract_view.h"

class View : public QMainWindow, public AbstractView {
  Q_OBJECT

 public:
  explicit View(AbstractGameController* game_controller);
  ~View() override = default;

 private:
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent* event) override;

 private:
  const int kFrameInterval = 16;

 private:
  AbstractGameController* game_controller_ = nullptr;
  int timer_id_;
};

#endif  // VIEW_VIEW_H_

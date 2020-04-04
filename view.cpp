#include "view.h"

View::View(AbstractGameController* game_controller)
    : game_controller_(game_controller),
      timer_(new QTimer(this)) {
  this->resize(800, 600);
  timer_->setInterval(16);
  timer_->start();
  timer_id_ = startTimer(16);
}

void View::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  game_controller_->Draw(&painter);
}

void View::timerEvent(QTimerEvent* event) {
  if (event->timerId() == timer_id_) {
    game_controller_->Update(timer_->interval());
  }
}

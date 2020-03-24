#include "view.h"

View::View(AbstractGameController* game_controller)
    : game_controller_(game_controller),
      timer_(new QTimer(this)) {
  this->resize(800, 600);
  timer_->setInterval(16);
  connect(timer_, &QTimer::timeout, this, [this]() {
    game_controller_->Update();
  });
  timer_->start();
}

void View::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  game_controller_->Draw(&painter);
}

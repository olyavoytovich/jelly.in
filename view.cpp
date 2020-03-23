#include "view.h"

View::View(AbstractGameController* game_controller)
    : game_controller_(game_controller),
      timer_(std::make_shared<QTimer>(this)) {
  this->resize(800, 600);
  timer_->setInterval(16);
  connect(timer_.get(), &QTimer::timeout, this,
          [&]() { game_controller_->Update(); });
  timer_->start();
}

void View::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  game_controller_->Draw(&painter);
}

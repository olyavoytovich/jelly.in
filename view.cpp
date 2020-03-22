#include "view.h"

#include <QObject>
#include <QPainter>

#include "game_controller.h"

View::View(GameController* game_controller)
    : game_controller_(game_controller) {
  timer_ = new QTimer(this);
  timer_->setInterval(16);
  connect(timer_, &QTimer::timeout, this,
          [=]() { game_controller_->Update(); });
  timer_->start();
}

void View::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  game_controller_->GetGameObject()->Draw(&painter);
}

View::~View() { delete timer_; }

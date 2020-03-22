#include "view.h"

#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

#include "game_controller.h"
#include "game_object.h"

View::View() : game_controller_(nullptr), timer_(nullptr) {}

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

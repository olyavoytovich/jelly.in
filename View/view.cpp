#include "view.h"

View::View(AbstractGameController* game_controller)
    : game_controller_(game_controller) {
  this->resize(800, 600);
  this->setMinimumSize(400, 300);
  timer_id_ = startTimer(kFrameInterval);
}

void View::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  this->Draw(&painter);
}

void View::timerEvent(QTimerEvent* event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  game_controller_->Update(kFrameInterval);
}

void View::keyPressEvent(QKeyEvent* event) {
  if (!event->isAutoRepeat()) {
    game_controller_->PressKey(event->key());
  }
  game_controller_->ClampKey(event->key());
}

void View::keyReleaseEvent(QKeyEvent* event) {
  game_controller_->ReleaseKey(event->key());
}

void View::Draw(QPainter* painter) {
  std::shared_ptr<Map> map = game_controller_->GetMap();
  if (map == nullptr) {
    return;
  }
  map->UpdateCamera(painter);

  painter->save();

  painter->translate(map->GetShift() * map->GetScale());
  painter->translate(-map->GetCurrentCamera().topLeft() * map->GetScale());

  painter->drawImage(0, 0, *map->GetScaledMapImage());

  painter->setBrush(QBrush(Qt::black, Qt::BrushStyle::BDiagPattern));
  map->DrawGameObjects(painter);

  painter->setBrush(QBrush(Qt::green, Qt::BrushStyle::SolidPattern));
  map->GetPlayer()->Draw(painter);

  painter->restore();
}

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

  painter->save();

  map->SetScale(std::max(
      static_cast<double>(painter->window().width())
          / map->GetVisibleSize().x(),
      static_cast<double>(painter->window().height())
          / map->GetVisibleSize().y()));
  map->SetShift(QPoint(painter->window().width(), painter->window().height()));
  map->SetShift(
      (map->GetShift() / map->GetScale() - map->GetVisibleSize()) / 2.0);

  map->UpdateCameraPosition();
  painter->translate(map->GetShift() * map->GetScale());
  painter->translate(-map->GetCurrentCamera().topLeft() * map->GetScale());

  map->UpdateImageScale(static_cast<int>(map->GetMapImage()->width()
                            * map->GetScale()),
                        static_cast<int>(map->GetMapImage()->height()
                            * map->GetScale()));
  painter->drawImage(0, 0, *map->GetScaledMapImage());
  painter->setBrush(QBrush(Qt::black, Qt::BrushStyle::BDiagPattern));

  std::shared_ptr<std::vector<std::shared_ptr<GameObject>>>
      game_objects = map->GetGameObjects();
  for (const auto& object : *game_objects) {
    object->Draw(painter);
  }

  painter->setBrush(QBrush(Qt::green, Qt::BrushStyle::SolidPattern));
  map->GetPlayer()->Draw(painter);

  painter->restore();
}

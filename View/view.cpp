#include "view.h"

View::View(AbstractGameController* game_controller)
    : game_controller_(game_controller) {
  this->resize(800, 600);
  this->setMinimumSize(400, 300);
  timer_id_ = startTimer(kFrameInterval);
}

void View::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  Draw(&painter);
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

void View::DrawObject(QPainter* painter,
                      std::shared_ptr<GameObject> entity,
                      std::shared_ptr<Map> map_) const {
  // Fixture используется, чтобы прикрепить форму к телу для обнаружения
  // коллизий. Содержит необходимые для отрисовки геометрические данные,
  // кроме них - трение, фильтр коллизий и др.
  std::shared_ptr<Animator> animator_ = entity->GetAnimator();
  if (animator_ != nullptr) {
    QRect rectangle_for_image = entity->GetBoundings();
    int width = static_cast<int>(entity->GetBoundingRectangle().width()
        * map_->GetScale());
    int height = static_cast<int>(entity->GetBoundingRectangle().height()
        * map_->GetScale());
    if (entity->GetEntityType() == EntityType::kSunflower) {
      width = static_cast<int>(width * entity->GetSunflowerWidthPercent());
      height = static_cast<int>(height * entity->GetSunflowerHeightPercent());
    }
    painter->drawImage(rectangle_for_image.topLeft() * map_->GetScale(),
                       *(animator_->GetCurrentImage(width, height)));
  }
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
  auto game_objects = map->GetGameObjects();
  for (auto game_object : game_objects) {
    DrawObject(painter, game_object, map);
  }

  painter->setBrush(QBrush(Qt::green, Qt::BrushStyle::SolidPattern));
  DrawObject(painter, map->GetPlayer(), map);

  painter->restore();
}

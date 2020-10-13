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
                      std::shared_ptr<GameObject> game_objects,
                      std::shared_ptr<Map> map) const {
  // Fixture используется, чтобы прикрепить форму к телу для обнаружения
  // коллизий. Содержит необходимые для отрисовки геометрические данные,
  // кроме них - трение, фильтр коллизий и др.
  std::shared_ptr<Animator> animator_ = game_objects->GetAnimator();
  if (animator_ != nullptr) {
    QRect rectangle_for_image = game_objects->GetBoundingRectangle();
    int width = static_cast<int>(rectangle_for_image.width()
        * map->GetScale());
    int height = static_cast<int>(rectangle_for_image.height()
        * map->GetScale());
    if (game_objects->GetEntityType() == EntityType::kSunflower) {
      const float kSunflowerWidthPercent = 2.0;
      const float kSunflowerHeightPercent = 4.4;
      width = static_cast<int>(width * kSunflowerWidthPercent);
      height = static_cast<int>(height * kSunflowerHeightPercent);
    }
    painter->drawImage(rectangle_for_image.topLeft() * map->GetScale(),
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

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
                      const std::shared_ptr<GameObject>& game_object) const {
  // Fixture используется, чтобы прикрепить форму к телу для обнаружения
  // коллизий. Содержит необходимые для отрисовки геометрические данные,
  // кроме них - трение, фильтр коллизий и др.
  std::shared_ptr<Animator> animator_ = game_object->GetAnimator();
  if (animator_ != nullptr) {
    QRect rectangle_for_image = game_object->GetBoundingRectangle();
    int width = static_cast<int>(rectangle_for_image.width()
        * camera_->GetScale());
    int height = static_cast<int>(rectangle_for_image.height()
        * camera_->GetScale());
    if (game_object->GetEntityType() == EntityType::kSunflower) {
      width = static_cast<int>(width * kSunflowerWidthPercent);
      height = static_cast<int>(height * kSunflowerHeightPercent);
    }
    painter->drawImage(rectangle_for_image.topLeft() * camera_->GetScale(),
                       *(animator_->GetCurrentImage(width, height)));
  }
}

void View::Draw(QPainter* painter) {
  map_ = game_controller_->GetMap();
  if (map_ == nullptr) {
    return;
  }
  camera_ = map_->GetCurrentCamera();
  map_->UpdateCamera(painter);

  painter->save();
  camera_->MovePainterToCamera(painter);

  painter->drawImage(0, 0, *map_->GetScaledMapImage());

  for (const auto& game_object : *map_->GetGameObjects()) {
    DrawObject(painter, game_object);
  }

  DrawObject(painter, map_->GetPlayer());

  painter->restore();
}

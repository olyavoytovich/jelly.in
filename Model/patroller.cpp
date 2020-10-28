#include "patroller.h"

Patroller::Patroller(std::weak_ptr<Map> map,
                     b2BodyType type,
                     const QPoint& body_position,
                     int radius,
                     const std::vector<QPoint>& way_points,
                     std::shared_ptr<Animator> animator,
                     int speed)
    : Entity(std::move(map), type, body_position, radius,
             EntityType::kChestnut) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  animator_->LoopAnimation();
  animator_->Play();
  SetWayPoints(way_points);
}

void Patroller::InitializeSound(std::shared_ptr<Sound> patroller_sound) {
  chestnut_sound_ = std::move(patroller_sound);
}

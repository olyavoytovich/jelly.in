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
  int flex = 1;
  SetWayPoints(way_points);
}

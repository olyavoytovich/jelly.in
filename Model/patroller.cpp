#include "patroller.h"

Patroller::Patroller(std::weak_ptr<Map> map,
                     b2BodyType type,
                     const QPoint& body_position,
                     const QPolygon& polygon,
                     const std::vector<QPoint>& way_points,
                     std::shared_ptr<Animator> animator,
                     int speed)
    : Entity(std::move(map), type, body_position, polygon,
             EntityType::kChestnut) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  animator_->LoopAnimation();
  animator_->Play();
  SetWayPoints(way_points);
}

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

Patroller::Patroller(std::weak_ptr<Map> map,
                     b2BodyType body_type,
                     const QPoint& body_position,
                     const std::vector<CircleShape>& circles,
                     const std::vector<PolygonShape>& polygons,
                     const std::vector<QPoint>& way_points,
                     std::shared_ptr<Animator> animator,
                     int speed)
    : Entity(std::move(map), body_type, body_position, circles, polygons,
             EntityType::kChestnut) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  animator_->LoopAnimation();
  animator_->Play();
  SetWayPoints(way_points);
}

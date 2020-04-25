#include "patroller.h"

Patroller::Patroller(std::shared_ptr<Map> map,
                     b2BodyType type,
                     const QPoint& body_position,
                     const QPolygon& polygon,
                     const std::vector<QPoint>& way_points,
                     std::shared_ptr<Animator> animator,
                     int speed)
    : Entity(std::move(map), type, body_position, polygon) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  SetWayPoints(way_points);
}

Patroller::Patroller(std::shared_ptr<Map> map,
                     b2BodyType type,
                     const QPoint& body_position,
                     int radius,
                     const std::vector<QPoint>& way_points,
                     std::shared_ptr<Animator> animator,
                     int speed)
    : Entity(std::move(map), type, body_position, radius) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  SetWayPoints(way_points);
}

Patroller::Patroller(std::shared_ptr<Map> map,
                     b2BodyType body_type,
                     const QPoint& body_position,
                     const std::vector<CircleShape>& circles,
                     const std::vector<PolygonShape>& polygons,
                     const std::vector<QPoint>& way_points,
                     std::shared_ptr<Animator> animator,
                     int speed)
    : Entity(std::move(map), body_type, body_position, circles, polygons) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  SetWayPoints(way_points);
}

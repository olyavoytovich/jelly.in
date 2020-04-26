#include "patroller.h"

Patroller::Patroller(std::shared_ptr<Map> map,
                     b2BodyType type,
                     const QPoint& body_position,
                     const QPolygon& polygon,
                     const std::vector<QPoint>& way_points, int speed)
    : Entity(std::move(map), type, body_position, polygon,
             EntityType::kPatroller) {
  SetSpeed(speed);
  SetWayPoints(way_points);
}

Patroller::Patroller(std::shared_ptr<Map> map,
                     b2BodyType type,
                     const QPoint& body_position,
                     int radius,
                     const std::vector<QPoint>& way_points, int speed)
    : Entity(std::move(map), type, body_position, radius,
             EntityType::kPatroller) {
  SetSpeed(speed);
  SetWayPoints(way_points);
}

Patroller::Patroller(std::shared_ptr<Map> map,
                     b2BodyType body_type,
                     const QPoint& body_position,
                     const std::vector<CircleShape>& circles,
                     const std::vector<PolygonShape>& polygons,
                     const std::vector<QPoint>& way_points, int speed)
    : Entity(std::move(map), body_type, body_position, circles, polygons,
             EntityType::kPatroller) {
  SetSpeed(speed);
  SetWayPoints(way_points);
}

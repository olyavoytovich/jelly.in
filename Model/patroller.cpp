#include "patroller.h"

Patroller::Patroller(std::shared_ptr<b2World> world,
                     b2BodyType type,
                     const Point& body_position,
                     const QPolygon& polygon,
                     const std::vector<Point>& way_points, float speed)
    : Entity(std::move(world), type, body_position, polygon) {
  SetSpeed(speed);
  SetWayPoints(way_points);
}

Patroller::Patroller(std::shared_ptr<b2World> world,
                     b2BodyType type,
                     const Point& body_position,
                     float radius,
                     const std::vector<Point>& way_points, float speed)
    : Entity(std::move(world), type, body_position, radius) {
  SetSpeed(speed);
  SetWayPoints(way_points);
}

Patroller::Patroller(std::shared_ptr<b2World> world,
                     b2BodyType body_type,
                     const Point& body_position,
                     const std::vector<CircleShape>& circles,
                     const std::vector<PolygonShape>& polygons,
                     const std::vector<Point>& way_points, float speed)
    : Entity(std::move(world), body_type, body_position, circles, polygons) {
  SetSpeed(speed);
  SetWayPoints(way_points);
}

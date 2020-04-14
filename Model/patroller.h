#ifndef PATROLLER_H_
#define PATROLLER_H_

#include <utility>

#include "entity.h"

class Patroller : public Entity {
 public:
  Patroller(std::shared_ptr<b2World> world, b2BodyType type,
            const Point& body_position, const QPolygon& polygon,
            const std::vector<Point>& way_points, float speed);

  Patroller(std::shared_ptr<b2World> world, b2BodyType type,
            const Point& body_position, float radius,
            const std::vector<Point>& way_points, float speed);

  Patroller(std::shared_ptr<b2World> world, b2BodyType body_type,
            const Point& body_position, const std::vector<CircleShape>& circles,
            const std::vector<PolygonShape>& polygons,
            const std::vector<Point>& way_points, float speed);
};

#endif  // PATROLLER_H_

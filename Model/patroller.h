#ifndef MODEL_PATROLLER_H_
#define MODEL_PATROLLER_H_

#include <memory>
#include <vector>
#include <utility>

#include "entity.h"

class Patroller : public Entity {
 public:
  Patroller(std::shared_ptr<Map> map, b2BodyType type,
            const QPoint& body_position, const QPolygon& polygon,
            const std::vector<QPoint>& way_points, int speed);

  Patroller(std::shared_ptr<Map> map, b2BodyType type,
            const QPoint& body_position, int radius,
            const std::vector<QPoint>& way_points, int speed);

  Patroller(std::shared_ptr<Map> map, b2BodyType body_type,
            const QPoint& body_position,
            const std::vector<CircleShape>& circles,
            const std::vector<PolygonShape>& polygons,
            const std::vector<QPoint>& way_points, int speed);

  ~Patroller() override = default;
};

#endif  // MODEL_PATROLLER_H_

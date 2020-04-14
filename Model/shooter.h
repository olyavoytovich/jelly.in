#ifndef MODEL_SHOOTER_H_
#define MODEL_SHOOTER_H_

#include <QPainter>
#include <algorithm>
#include <memory>
#include <vector>
#include <utility>

#include "entity.h"

enum class BulletDirection {
  kBottom,
  kLeftRight
};

class Shooter : public Entity {
 public:
  Shooter(std::shared_ptr<b2World> world, b2BodyType type,
          const Point& body_position, const QPolygon& polygon,
          const std::vector<Point>& way_points,
          BulletDirection bullet_direction, int period, float bullet_steed,
          float speed = 0);

  Shooter(std::shared_ptr<b2World> world, b2BodyType type,
          const Point& body_position, float radius,
          const std::vector<Point>& way_points,
          BulletDirection bullet_direction, int period, float bullet_steed,
          float speed = 0);

  Shooter(std::shared_ptr<b2World> world, b2BodyType body_type,
          const Point& body_position, const std::vector<CircleShape>& circles,
          const std::vector<PolygonShape>& polygons,
          const std::vector<Point>& way_points,
          BulletDirection bullet_direction, int period, float bullet_steed,
          float speed = 0);

  void Draw(QPainter* painter) const override;

  void Update(int time) override;

 private:
  void AddBullet(const Point& bullet_position, const b2Vec2& velocity);
  void CheckPolygonPoints(b2Fixture* fixture, float* left_side,
                          float* right_side, float* bottom, float* top) const;

 private:
  BulletDirection bullet_direction_;

 private:
  std::vector<std::shared_ptr<Entity>> bullets_;

  // Крайняя левая точка отрисовки пули, относительно тела
  b2Vec2 left_point_;

  // Крайняя права точка отрисовки пули, относительно тела
  b2Vec2 right_point_;

  // Время, которое прошло с начала прошлого встрела пули
  int time_ = 0;

  // Период, с которым стреляет монстр
  int period_;
  float bullet_speed_;
};

#endif  // MODEL_SHOOTER_H_

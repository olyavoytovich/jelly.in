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
          BulletDirection bullet_direction,
          int shoot_period, float bullet_speed, float bullet_radius,
          float speed = 0);

  Shooter(std::shared_ptr<b2World> world, b2BodyType type,
          const Point& body_position, float radius,
          const std::vector<Point>& way_points,
          BulletDirection bullet_direction,
          int shoot_period, float bullet_speed, float bullet_radius,
          float speed = 0);

  Shooter(std::shared_ptr<b2World> world, b2BodyType body_type,
          const Point& body_position,
          const std::vector<CircleShape>& circles,
          const std::vector<PolygonShape>& polygons,
          const std::vector<Point>& way_points,
          BulletDirection bullet_direction,
          int shoot_period, float bullet_speed, float bullet_radius,
          float speed = 0);

  void Draw(QPainter* painter) const override;

  void Update(int time) override;

 private:
  void AddBullet(const Point& bullet_position);

  // Проверяет точки-вершины полигона. Устанавливает в
  //    left_point_.x - наименьшее значение координаты x среди точек полигона
  //    left_point_.y - наибольшее значение координаты y среди точек полигона
  //    right_point_.x - наибольшее значение координаты x среди точек полигона
  //    right_point_.y - наименьшее значение координаты y среди точек полигона
  void CheckPolygonPoints(b2PolygonShape* polygon_shape);

 private:
  BulletDirection bullet_direction_;

  std::vector<std::shared_ptr<Entity>> bullets_;

  // Крайняя левая точка отрисовки пули, относительно тела
  b2Vec2 left_point_ = b2Vec2(20000, -20000);

  // Крайняя правая точка отрисовки пули, относительно тела
  b2Vec2 right_point_ = b2Vec2(-20000, 20000);

  // Время, которое прошло с начала прошлого выстрела пули
  int last_shoot_time_ = 0;

  // Период, с которым стреляет монстр
  int shoot_period_;

  float bullet_speed_;

  // Радиус пули
  float bullet_radius_;
};

#endif  // MODEL_SHOOTER_H_

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
  Shooter(std::shared_ptr<Map> map, b2BodyType type,
          const QPoint& body_position, const QPolygon& polygon,
          const std::vector<QPoint>& way_points,
          BulletDirection bullet_direction,
          int shoot_period, int bullet_speed, int bullet_radius,
          int speed = 0);

  Shooter(std::shared_ptr<Map> map, b2BodyType type,
          const QPoint& body_position, int radius,
          const std::vector<QPoint>& way_points,
          BulletDirection bullet_direction,
          int shoot_period, int bullet_speed, int bullet_radius,
          int speed = 0);

  Shooter(std::shared_ptr<Map> map, b2BodyType body_type,
          const QPoint& body_position,
          const std::vector<CircleShape>& circles,
          const std::vector<PolygonShape>& polygons,
          const std::vector<QPoint>& way_points,
          BulletDirection bullet_direction,
          int shoot_period, int bullet_speed, int bullet_radius,
          int speed = 0);

  ~Shooter() override = default;

  void Draw(QPainter* painter) const override;

  void Update(int time) override;

 private:
  void AddBullet(const b2Vec2& bullet_position);

  // Инициализирует left_point_, right_point_
  void InitializeBoundaryPoints();

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

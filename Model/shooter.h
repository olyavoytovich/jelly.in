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
  Shooter(std::weak_ptr<Map> map,
          b2BodyType type,
          const QPoint& body_position,
          const QPolygon& polygon,
          const std::vector<QPoint>& way_points,
          BulletDirection bullet_direction,
          int shoot_period,
          int bullet_speed,
          int bullet_radius,
          std::shared_ptr<Animator> animator,
          std::shared_ptr<Animator> bullet_animator,
          EntityType entity_type,
          int speed = 0);

  Shooter(std::weak_ptr<Map> map,
          b2BodyType type,
          const QPoint& body_position,
          int radius,
          const std::vector<QPoint>& way_points,
          BulletDirection bullet_direction,
          int shoot_period,
          int bullet_speed,
          int bullet_radius,
          std::shared_ptr<Animator> animator,
          std::shared_ptr<Animator> bullet_animator,
          EntityType entity_type,
          int speed = 0);

  Shooter(std::weak_ptr<Map> map,
          b2BodyType body_type,
          const QPoint& body_position,
          const std::vector<CircleShape>& circles,
          const std::vector<PolygonShape>& polygons,
          const std::vector<QPoint>& way_points,
          BulletDirection bullet_direction,
          int shoot_period,
          int bullet_speed,
          int bullet_radius,
          std::shared_ptr<Animator> animator,
          std::shared_ptr<Animator> bullet_animator,
          EntityType entity_type,
          int speed = 0);

  ~Shooter() override = default;

  void Update(int time) override;

 private:
  std::shared_ptr<Entity> CreateBullet(const QPoint& bullet_position);

 private:
  BulletDirection bullet_direction_;

  // Время, которое прошло с начала прошлого выстрела пули
  int last_shoot_time_ = 0;

  // Период, с которым стреляет монстр
  int shoot_period_;

  float bullet_speed_;

  // Радиус пули
  int bullet_radius_;

  std::shared_ptr<Animator> bullet_animator_;
};

#endif  // MODEL_SHOOTER_H_

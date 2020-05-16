#ifndef MODEL_ENTITY_H_
#define MODEL_ENTITY_H_

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "animator.h"
#include "audio_manager.h"
#include "game_object.h"
#include "map.h"

struct PolygonShape {
  PolygonShape(QPolygon polygon, const QPoint& position)
      : polygon(std::move(polygon)), position(position) {}

  QPolygon polygon;
  QPoint position;
};

struct CircleShape {
  CircleShape(int radius, const QPoint& position)
      : radius(radius), position(position) {}

  int radius;
  QPoint position;
};

class Entity : public GameObject {
 public:
  // Конструктор, создающий тело из одной формы - полигон.
  Entity(std::weak_ptr<Map> map,
         b2BodyType type,
         const QPoint& body_position,
         const QPolygon& polygon,
         EntityType entity_type);

  // Конструктор, создающий тело из одной формы - круг.
  Entity(std::weak_ptr<Map> map,
         b2BodyType type,
         const QPoint& body_position,
         int radius,
         EntityType entity_type);

  // Конструктор, благодаря которому можно создать тело из нескольких форм со
  // своими локальными координатами. Третий параметр (body_position) - это
  // координаты всего тела, следующий параметр - вектор структур CircleShape.
  // В каждой структуре хранятся данные о радиусе и о локальных координатах
  // данного круга. Пятый параметр так же вектор структур. В одной структуре
  // PolygonShape хранятся данные о форме QPolygon и о локальных координатах.
  Entity(std::weak_ptr<Map> map,
         b2BodyType body_type,
         const QPoint& body_position,
         const std::vector<CircleShape>& circles,
         const std::vector<PolygonShape>& polygons,
         EntityType entity_type);

  ~Entity() override = default;

  // Отрисовывает все формы тела.
  void Draw(QPainter* painter) const override;

  b2PolygonShape CreatePolygonShape(const QPolygon& polygon,
                                    const QPoint& shape_position =
                                        {0, 0}) const;
  b2CircleShape CreateCircleShape(int radius,
                                  const QPoint& shape_position = {0, 0}) const;

  void SetAnimator(std::shared_ptr<Animator> animator);
  void SetWayPoints(const std::vector<QPoint>& way_points);
  void SetSpeed(int speed);
  void SetVelocity(b2Vec2 velocity, bool apply_once = false);
  void SetVelocity(b2Vec2 target_position, b2Vec2 current_position,
                   float speed, bool apply_once = false);
  void SetEntityType(EntityType entity_type);

  void Update(int time) override;

  void Activate();
  void Stop();

  b2Body* GetB2Body() const override;
  QPoint GetPositionInPixels() const override;
  QRect GetBoundings() const;

  virtual void BeginCollision(b2Fixture* my_fixture,
                              EntityType my_type,
                              EntityType other_type);
  virtual void EndCollision(b2Fixture* my_fixture, EntityType other_type);

  EntityType GetEntityType() const;

 protected:
  int MetersToPixels(float value) const;
  QPoint MetersToPixels(b2Vec2 vector) const;
  float PixelsToMeters(int value) const;
  b2Vec2 PixelsToMeters(QPoint vector) const;

  b2Fixture* CreateFixture(const b2Shape& shape);
  void SetNoCollisionMask(uint16_t mask);

  // Выбирает громкость в зависимости от расстояния до игрока
  int CountVolumeFromDistance();

 protected:
  const float kEpsilon = 1e-5;

 protected:
  b2Body* body_ = nullptr;

  std::vector<b2Vec2> way_points_;

  // Индекс текущего way_point в векторе way_points_
  int way_point_index_ = 0;

  float speed_ = 0;

  // Направление, по которому итерируемся по way_points:
  //    -- direction_ = 1 - из начала в конец
  //    -- direction_ = -1 - из конца в начало
  int direction_ = 1;

  QRect bounding_rectangle_;

  std::weak_ptr<Map> map_;

  std::shared_ptr<Animator> animator_ = nullptr;

 private:
  void InitializeBody(b2BodyType body_type, const QPoint& body_position);

  void ApplyImpulse();

  void InitializeBoundaryRectangle();

  void ApplyEntityType(b2Fixture* fixture);

 private:
  const float kBodyDensity = 1;
  const float kPixelsPerMeter = 100;

  const float kSunflowerWidthPercent = 2.0;
  const float kSunflowerHeightPercent = 4.4;

 private:
  b2Vec2 target_velocity = {0, 0};
  bool is_active_ = true;
  EntityType entity_type_;
};

#endif  // MODEL_ENTITY_H_

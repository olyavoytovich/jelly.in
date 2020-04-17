#ifndef ENTITY_H_
#define ENTITY_H_

#include <memory>
#include <utility>
#include <vector>

#include "box2d/box2d.h"
#include "game_object.h"

struct Point {
  explicit Point(const b2Vec2& position) : x(position.x), y(position.y) {}
  Point(float x, float y) : x(x), y(y) {}

  QPoint ToQPoint() {
    return {static_cast<int>(x), static_cast<int>(y)};
  }

  float x;
  float y;
};

struct PolygonShape {
  PolygonShape(QPolygon polygon, const Point& position)
      : polygon(std::move(polygon)), position(position) {}

  QPolygon polygon;
  Point position;
};

struct CircleShape {
  CircleShape(float radius, const Point& position)
      : radius(radius), position(position) {}

  float radius;
  Point position;
};

class Entity : public GameObject {
 public:
  // Конструктор, создающий тело из одной формы - полигон.
  Entity(std::shared_ptr<b2World> world,
         b2BodyType type,
         const Point& body_position,
         const QPolygon& polygon);

  // Конструктор, создающий тело из одной формы - круг.
  Entity(std::shared_ptr<b2World> world,
         b2BodyType type,
         const Point& body_position,
         float radius);

  // Конструктор, благодаря которому можно создать тело из нескольких форм со
  // своими локальными координатами. Третий параметр (body_position) - это
  // координаты всего тела, следующий параметр - вектор структур CircleShape.
  // В каждой структуре хранятся данные о радиусе и о локальных координатах
  // данного круга. Пятый параметр так же вектор структур. В одной структуре
  // PolygonShape хранятся данные о форме QPolygon и о локальных координатах.
  Entity(std::shared_ptr<b2World> world,
         b2BodyType body_type,
         const Point& body_position,
         const std::vector<CircleShape>& circles,
         const std::vector<PolygonShape>& polygons);

  ~Entity() override;

  // Отрисовывает все формы тела.
  void Draw(QPainter* painter) const override;

  b2PolygonShape CreatePolygonShape(const QPolygon& polygon,
                                    const Point& shape_position = {0, 0}) const;
  b2CircleShape CreateCircleShape(float radius,
                                  const Point& shape_position = {0, 0}) const;

  void SetLinearVelocity(b2Vec2 velocity);
  void SetApplyLinearImpulse(b2Vec2 force);

 protected:
  const float kEps = static_cast<float>(0.00001);
  b2Body* body_ = nullptr;

 private:
  // Рисует формы в зависимости от типа их фигуры. Вторым параметром передается
  // форму, которую будет отрисовывать эта функция.
  void DrawShape(QPainter* painter, b2Fixture* shape) const;

  void InitializeBody(b2BodyType body_type, const Point& body_position);

  qreal MetersToPixels(float value) const;
  QPointF MetersToPixels(b2Vec2 vector) const;
  float PixelsToMeters(qreal value) const;
  b2Vec2 PixelsToMeters(QPointF vector) const;

 private:
  const float kBodyDensity = 1;
  const float kPixelsPerMeter = 100;

 private:
  std::shared_ptr<b2World> world_;
};

#endif  // ENTITY_H_

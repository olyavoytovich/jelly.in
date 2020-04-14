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

  QPoint ToQPoint() { return {static_cast<int>(x), static_cast<int>(y)}; }

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
  Entity(std::shared_ptr<b2World> world, b2BodyType type,
         const Point& body_position, const QPolygon& polygon);

  // Конструктор, создающий тело из одной формы - круг.
  Entity(std::shared_ptr<b2World> world, b2BodyType type,
         const Point& body_position, float radius);

  // Конструктор, благодаря которому можно создать тело из нескольких форм со
  // своими локальными координатами. Третий параметр (body_position) - это
  // координаты всего тела, следующий параметр - вектор структур CircleShape.
  // В каждой структуре хранятся данные о радиусе и о локальных координатах
  // данного круга. Пятый параметр так же вектор структур. В одной структуре
  // PolygonShape хранятся данные о форме QPolygon и о локальных координатах.
  Entity(std::shared_ptr<b2World> world, b2BodyType body_type,
         const Point& body_position, const std::vector<CircleShape>& circles,
         const std::vector<PolygonShape>& polygons);

  // Конструктор, создающий тело из одной формы (полигон)
  // а также сразу задающий вектор скорости
  Entity(const std::shared_ptr<b2World>& world, b2BodyType type,
         const Point& body_position, const QPolygon& polygon,
         const b2Vec2& velocity);

  ~Entity() override;

  // Отрисовывает все формы тела.
  void Draw(QPainter* painter) const override;

  b2PolygonShape CreatePolygonShape(const QPolygon& polygon,
                                    const Point& shape_position = {0, 0}) const;
  b2CircleShape CreateCircleShape(float radius,
                                  const Point& shape_position = {0, 0}) const;

  void SetWayPoints(const std::vector<Point>& way_points);
  void SetSpeed(float speed);

  void Update(int time) override;

 protected:
  float CalculateSpeed(const Point& point1, const Point& point2) const;

 protected:
  b2Body* body_ = nullptr;
  std::vector<Point> way_points_;
  int index_of_current_point_ = 0;

  float speed_ = 0;
  // Направление, в котором идет Entity:
  //    -- direction_ = 1 - из начала в конец
  //    -- direction_ = -1 - из конца в начало
  int direction_ = 1;

  std::shared_ptr<b2World> world_;

 private:
  // Рисует формы в зависимости от типа их фигуры. Вторым параметром передается
  // форму, которую будет отрисовывать эта функция.
  void DrawShape(QPainter* painter, b2Fixture* shape) const;

  void InitializeBody(b2BodyType body_type, const Point& body_position);

 private:
  const float kBodyDensity = 1;
};

#endif  // ENTITY_H_

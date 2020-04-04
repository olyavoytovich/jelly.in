#ifndef ENTITY_H_
#define ENTITY_H_

#include <memory>
#include <utility>
#include <vector>

#include "box2d/box2d.h"
#include "game_object.h"

struct PolygonShape {
  QPolygon polygon;
  QPoint shape_position;

  PolygonShape(QPolygon polygon, QPoint shape_position) :
      polygon(std::move(polygon)), shape_position(shape_position) {}
};

struct CircleShape {
  int radius;
  QPoint shape_position;

  CircleShape(int radius, QPoint shape_position)
      : radius(radius), shape_position(shape_position) {}
};

class Entity : public GameObject {
 public:
  // Конструктор, создающий тело из одной формы - полигон.
  Entity(std::shared_ptr<b2World> world,
         b2BodyType type,
         QPoint body_position,
         const QPolygon& polygon);

  // Конструктор, создающий тело из одной формы - круг.
  Entity(std::shared_ptr<b2World> world,
         b2BodyType type,
         QPoint body_position,
         int radius);

  // Конструктор, благодаря которому можно создать тело из нескольких форм со
  // своими локальными координатами. Третий параметр (body_position) - это
  // координаты всего тела, следующий параметр - вектор структур CircleShape.
  // В каждой структуре хранятся данные о радиусе и о локальных координатах
  // данного круга. Пятый параметр так же вектор структур. В одной структуре
  // PolygonShape хранятся данные о форме QPolygon и о локальных координатах.
  Entity(std::shared_ptr<b2World> world,
         b2BodyType body_type,
         const QPoint& body_position,
         const std::vector<CircleShape>& circles,
         const std::vector<PolygonShape>& polygons);

  ~Entity() override;

  // Отрисовывает все формы тела.
  void Draw(QPainter* painter) const override;

  b2PolygonShape CreatePolygonShape(const QPolygon& polygon,
                                    QPoint shape_position = {0, 0}) const;
  b2CircleShape CreateCircleShape(int radius,
                                  QPoint shape_position = {0, 0}) const;

 private:
  // Рисует формы в зависимости от типа их фигуры. Вторым параметром передается
  // форму, которую будет отрисовывать эта функция.
  void DrawShape(QPainter* painter, b2Fixture* shape) const;

  void InitializeBody(b2BodyType body_type, QPoint body_position);

  // Переводит позицию из типа b2Vec2  в удобный нам тип QPoint.
  QPoint Box2dPointToQPoint(b2Vec2 position) const;

 private:
  const float kBodyDensity = 1;

 private:
  std::shared_ptr<b2World> world_;
  b2Body* body_ = nullptr;
};

#endif  // ENTITY_H_

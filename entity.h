#ifndef ENTITY_H_
#define ENTITY_H_

#include <memory>
#include <utility>
#include <vector>

#include "box2d/box2d.h"
#include "game_object.h"

class Entity : public GameObject {
 public:
  // конструктор, создающий тело из одной формы - полигон
  Entity(const QPolygon& polygon,
         const std::shared_ptr<b2World>& world,
         b2BodyType type,
         QPoint position);

  // конструктор, создающий тело из одной формы - круг
  Entity(int radius,
         const std::shared_ptr<b2World>& world,
         b2BodyType type,
         QPoint position);

  // конструктор, благодаря которому можно создать тело из нескольких форм со
  // своими локальными координатами.
  Entity(const std::shared_ptr<b2World>& world,
         b2BodyType type,
         QPoint position,
         const std::vector<std::pair<int, QPoint>>& vec_of_circs,
         const std::vector<std::pair<QPolygon,
                                     QPoint>>& vec_of_polygons);
  ~Entity() override;

  // отрисовывает все формы тела
  void Draw(QPainter* painter) const override;
  // рисует объекты в зависимости от типа их фигуры
  void DrawShape(QPainter* paint, b2Fixture* shape, b2Body* body) const;

  b2PolygonShape AddShape(const QPolygon& polygon,
                          QPoint position = QPoint(0, 0)) const;
  b2CircleShape AddShape(int radius, QPoint position = QPoint(0, 0)) const;

 private:
  void MakeObject(b2BodyType type, const QPolygon& polygon, QPoint position);
  void MakeObject(b2BodyType type, int radius, QPoint position);

  void MakeBody(b2BodyType type, QPoint position);

  QPoint ToCoords(b2Vec2 position) const;

 private:
  std::shared_ptr<b2World> world_;
  b2Body* body_ = nullptr;
};

#endif  // ENTITY_H_

#ifndef ENTITY_H_
#define ENTITY_H_

#include "box2d/box2d.h"
#include "game_object.h"

class Entity : public GameObject {
 public:
  Entity(const QPolygon& polygon, int dx, b2World* world, b2BodyType type);
  ~Entity() = default;

  void Draw(QPainter* painter) const override;
  void DrawShape(QPainter* paint, b2Fixture* shape, b2Body* body) const;
  void MakeObject(b2BodyType type);

  QPoint ToCoords(b2Vec2 position) const;
  QVector<QPoint> ToCoords(QVector<b2Vec2> points) const;

 private:
  b2World* world_;
  b2Body* body_;
};

#endif  // ENTITY_H_

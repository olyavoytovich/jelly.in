#include "entity.h"

Entity::Entity(const QPolygon& polygon, int dx, b2World* world, b2BodyType type)
    : world_(world), GameObject(polygon, dx) {
  MakeObject(type);
}

void Entity::DrawShape(QPainter* painter,
                       b2Fixture* shape, b2Body* body) const {
  painter->save();
  painter->rotate(180);
  switch (shape->GetShape()->GetType()) {
    case b2Shape::e_polygon: {
      QVector<QPoint> points;
      QPoint point;
      auto* poly = dynamic_cast<b2PolygonShape*>(shape->GetShape());
      for (int i = 0; i < poly->m_count; i++) {
        points.push_back(
            ToCoords(body->GetWorldPoint(poly->m_vertices[i])));
      }
      painter->drawPolygon(QPolygon(points));
      break;
    }
    case b2Shape::e_circle: {
      auto* circle = dynamic_cast<b2CircleShape*>(shape->GetShape());
      painter->drawEllipse(ToCoords(body->GetWorldCenter()),
                           static_cast<int>(circle->m_radius),
                           static_cast<int>(circle->m_radius));
    }
  }
  painter->restore();
}

void Entity::Draw(QPainter* painter) const {
  for (b2Fixture* x = body_->GetFixtureList(); x; x = x->GetNext()) {
    DrawShape(painter, x, body_);
  }
}

void Entity::MakeObject(b2BodyType type) {
  b2BodyDef bdef;
  bdef.type = type;
  if (type == b2_dynamicBody) {
    bdef.position.Set(-100, -100);

    b2PolygonShape shape;
    shape.SetAsBox(24, 40);

    b2PolygonShape shape2;
    shape2.SetAsBox(20, 50, b2Vec2(0, 80), 20);

    body_ = world_->CreateBody(&bdef);

    body_->CreateFixture(&shape, 1);
    body_->CreateFixture(&shape2, 4);
  } else {
    bdef.position.Set(-200, -100);

    b2CircleShape shape;
    shape.m_radius = 30;

    b2PolygonShape shape2;
    shape2.SetAsBox(24, 30, b2Vec2(0, 60), 0);

    body_ = world_->CreateBody(&bdef);

    body_->CreateFixture(&shape2, 1);
    body_->CreateFixture(&shape, 1);
  }
}

QPoint Entity::ToCoords(b2Vec2 position) const {
  return QPoint(static_cast<int>(position.x), static_cast<int>(position.y));
}

QVector<QPoint> Entity::ToCoords(QVector<b2Vec2> points) const {
  QVector<QPoint> result(points.size());
  for (int i = 0; i < points.size(); i++) {
    result[i] = ToCoords(points[i]);
  }
  return result;
}

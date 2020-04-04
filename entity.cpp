#include "entity.h"

#include <utility>

// про координаты. Координаты в Box2D такие же, как у математиков. То есть
// ось абсцисс направлена вправо, ось ординат - вверх.
// Но рисуем объекты мы в координатах окна. При этом центр КС бокса ставим в
// центр КС окна.
// В то время как у окна центр - это левый верхний угол, ось Ox направлена
// вправо, Oy - влево. Поэтому, чтобы динамические объекты летели к земле,
// а не в небо, делаем painter->scale(1, -1);, тогда направление оси ординат
// меняется, и мы получаем нужный результат.
// Далее position - координаты данного тела в координатной системе окна.
// А own_position - это координаты данной формы(!не тела) в системе координат
// тела. Можно сказать, что у каждого тела есть свой мини-мир со своими
// локальными координатами, и если нужно создать тело из нескольких форм,
// то каждой форме можно задавать свои координаты (own_position).

Entity::Entity(const QPolygon& polygon,
               std::shared_ptr<b2World> world,
               b2BodyType type,
               QPoint position)
    : world_(std::move(world)) {
  MakeBody(type, position);
  b2PolygonShape shape = AddShape(polygon);

  body_->CreateFixture(&shape, 1);
}

Entity::Entity(int radius,
               std::shared_ptr<b2World> world,
               b2BodyType type,
               QPoint position) : world_(std::move(world)) {
  MakeBody(type, position);
  b2CircleShape shape = AddShape(radius);

  body_->CreateFixture(&shape, 1);
}

Entity::Entity(std::shared_ptr<b2World> world,
               b2BodyType type,
               const QPoint& position,
               const std::vector<std::pair<int, QPoint>>& vec_of_circs,
               const std::vector<std::pair<QPolygon,
                                           QPoint>>& vec_of_polygons) :
    world_(std::move(world)) {
  MakeBody(type, position);

  for (const auto& circ : vec_of_circs) {
    b2CircleShape shape = AddShape(circ.first, circ.second);
    body_->CreateFixture(&shape, 1);
  }

  for (const auto& polygon : vec_of_polygons) {
    b2PolygonShape shape = AddShape(polygon.first, polygon.second);
    body_->CreateFixture(&shape, 1);
  }
}

Entity::~Entity() {
  world_->DestroyBody(body_);
}

void Entity::Draw(QPainter* painter) const {
  // fixture используется, чтобы прикрепить форму к телу для обнаружения
  // коллизий. Содержит необходимые для отрисовки геометрические данные,
  // кроме них - трение, фильтр коллизий и др.
  for (b2Fixture* fixture = body_->GetFixtureList(); fixture;
       fixture = fixture->GetNext()) {
    DrawShape(painter, fixture, body_);
  }
}

void Entity::DrawShape(QPainter* painter,
                       b2Fixture* shape, b2Body* body) const {
  painter->save();
  painter->scale(1, -1);

  switch (shape->GetShape()->GetType()) {
    case b2Shape::e_polygon: {
      QVector<QPoint> points;
      auto* polygon = dynamic_cast<b2PolygonShape*>(shape->GetShape());
      for (int i = 0; i < polygon->m_count; i++) {
        points.push_back(
            ToCoords(body->GetWorldPoint(polygon->m_vertices[i])));
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

b2PolygonShape Entity::AddShape(const QPolygon& polygon,
                                QPoint own_position) const {
  b2PolygonShape shape;
  std::vector<b2Vec2> poly_points;

  for (auto points : polygon) {
    poly_points.emplace_back(static_cast<float>(points.x()),
                             static_cast<float>(points.y()));
  }

  shape.m_centroid.Set(static_cast<float>(own_position.x()),
                       static_cast<float>(own_position.y()));
  shape.Set(poly_points.data(), polygon.size());
  return shape;
}

b2CircleShape Entity::AddShape(int radius, QPoint own_position) const {
  b2CircleShape shape;
  shape.m_radius = static_cast<float>(radius);
  shape.m_p.Set(static_cast<float>(own_position.x()),
                static_cast<float>(own_position.y()));

  return shape;
}

void Entity::MakeBody(b2BodyType type, QPoint position) {
  b2BodyDef bdef;
  bdef.position.Set(static_cast<float>(position.x()),
                    static_cast<float>(position.y()));
  bdef.type = type;

  body_ = world_->CreateBody(&bdef);
}

QPoint Entity::ToCoords(b2Vec2 position) const {
  return QPoint(static_cast<int>(position.x), static_cast<int>(position.y));
}

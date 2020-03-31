#include "entity.h"

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
               b2World* world,
               b2BodyType type,
               QPoint position)
    : world_(world) {
  MakeObject(type, polygon, position);
}

Entity::Entity(int radius,
               b2World* world,
               b2BodyType type,
               QPoint position) : world_(world) {
  MakeObject(type, radius, position);
}

Entity::Entity(b2World* world,
               b2BodyType type,
               QPoint position, const
               std::vector<std::pair<int, QPoint>>& vec_of_circs,
               const std::vector<std::pair<QPolygon,
                                           QPoint>>& vec_of_polygons) {
  world_ = world;
  b2BodyDef bdef;
  bdef.position.Set(static_cast<float >(position.x()),
                    static_cast<float >(position.y()));
  bdef.type = type;
  body_ = world_->CreateBody(&bdef);

  for (auto const& circ : vec_of_circs) {
    b2CircleShape shape = AddShape(circ.first, circ.second);
    body_->CreateFixture(&shape, 1);
  }

  for (auto const& polygon : vec_of_polygons) {
    b2PolygonShape shape = AddShape(polygon.first, polygon.second);
    body_->CreateFixture(&shape, 1);
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

void Entity::Draw(QPainter* painter) const {
  // fixter используется, чтобы прикрепить форму к телу для обнаружения коллизий.
  // Содержит необходимые для отрисовки геометрические данные, кроме них -
  // трение, фильтр коллизий и др.
  for (b2Fixture* f_list = body_->GetFixtureList(); f_list;
       f_list = f_list->GetNext()) {
    DrawShape(painter, f_list, body_);
  }
}

QPoint Entity::ToCoords(b2Vec2 position) const {
  return QPoint(static_cast<int>(position.x), static_cast<int>(position.y));
}

b2PolygonShape Entity::AddShape(const QPolygon& polygon) const {
  b2PolygonShape shape;
  std::vector<QPoint> poly_points = polygon.toStdVector();
  int n = poly_points.size();

  b2Vec2 points[n];
  for (int i = 0; i < n; i++) {
    points[i].Set(static_cast<float>(poly_points[i].x()),
                  static_cast<float>(poly_points[i].y()));
  }

  shape.Set(points, n);
  return shape;
}

b2PolygonShape Entity::AddShape(const QPolygon& polygon,
                                QPoint own_position) const {
  b2PolygonShape shape = AddShape(polygon);
  shape.m_centroid.Set(static_cast<float >(own_position.x()),
                       static_cast<float >(own_position.y()));

  return shape;
}

b2CircleShape Entity::AddShape(int radius) const {
  b2CircleShape shape;
  shape.m_radius = static_cast<float>(radius);

  return shape;
}

b2CircleShape Entity::AddShape(int radius, QPoint own_position) const {
  b2CircleShape shape;
  shape.m_radius = static_cast<float>(radius);
  shape.m_p.Set(static_cast<float >(own_position.x()),
                static_cast<float >(own_position.y()));

  return shape;
}

void Entity::MakeObject(b2BodyType type,
                        const QPolygon& polygon,
                        QPoint position) {
  b2BodyDef bdef;
  bdef.position.Set(static_cast<float >(position.x()),
                    static_cast<float >(position.y()));
  bdef.type = type;

  body_ = world_->CreateBody(&bdef);

  b2PolygonShape shape = AddShape(polygon);
  body_->CreateFixture(&shape, 1);
}

void Entity::MakeObject(b2BodyType type, int radius, QPoint position) {
  b2BodyDef bdef;
  bdef.position.Set(static_cast<float >(position.x()),
                    static_cast<float >(position.y()));
  bdef.type = type;

  body_ = world_->CreateBody(&bdef);

  b2CircleShape shape = AddShape(radius);
  body_->CreateFixture(&shape, 1);
}

Entity::~Entity() {
  world_->DestroyBody(body_);
}

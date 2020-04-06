#include "entity.h"

#include <utility>

// Про координаты. Координаты в Box2D такие же, как у математиков. То есть
// ось абсцисс направлена вправо, ось ординат - вверх.
// Но рисуем объекты мы в координатах окна. При этом центр КС бокса ставим в
// центр КС окна.
// В то время как у окна центр - это левый верхний угол, ось Ox направлена
// вправо, Oy - влево. Поэтому, чтобы динамические объекты летели к земле,
// а не в небо, делаем painter->scale(1, -1);, тогда направление оси ординат
// меняется, и мы получаем нужный результат.
// Далее body_position - координаты данного тела в координатной системе окна.
// А shape_position - это координаты данной формы(!не тела) в системе координат
// тела. Можно сказать, что у каждого тела есть свой мини-мир со своими
// локальными координатами, и если нужно создать тело из нескольких форм,
// то каждой форме можно задавать свои координаты (shape_position).

QPoint Point::ToQPoint() {
  return {static_cast<int>(x), static_cast<int>(y)};
}

Entity::Entity(std::shared_ptr<b2World> world,
               b2BodyType body_type,
               const Point& body_position,
               const QPolygon& polygon)
    : world_(std::move(world)) {
  InitializeBody(body_type, body_position);
  b2PolygonShape shape = CreatePolygonShape(polygon);
  body_->CreateFixture(&shape, kBodyDensity);
}

Entity::Entity(std::shared_ptr<b2World> world,
               b2BodyType body_type,
               const Point& body_position,
               int radius) : world_(std::move(world)) {
  InitializeBody(body_type, body_position);
  b2CircleShape shape = CreateCircleShape(radius);
  body_->CreateFixture(&shape, kBodyDensity);
}

Entity::Entity(std::shared_ptr<b2World> world,
               b2BodyType body_type,
               const Point& body_position,
               const std::vector<CircleShape>& circles,
               const std::vector<PolygonShape>& polygons) :
    world_(std::move(world)) {
  InitializeBody(body_type, body_position);

  for (auto&&[radius, shape_position] : circles) {
    b2CircleShape shape = CreateCircleShape(radius, shape_position);
    body_->CreateFixture(&shape, kBodyDensity);
  }

  for (auto&&[polygon, shape_position] : polygons) {
    b2PolygonShape shape = CreatePolygonShape(polygon, shape_position);
    body_->CreateFixture(&shape, kBodyDensity);
  }
}

Entity::~Entity() {
  world_->DestroyBody(body_);
}

void Entity::Draw(QPainter* painter) const {
  // Fixture используется, чтобы прикрепить форму к телу для обнаружения
  // коллизий. Содержит необходимые для отрисовки геометрические данные,
  // кроме них - трение, фильтр коллизий и др.
  for (auto fixture = body_->GetFixtureList(); fixture != nullptr;
       fixture = fixture->GetNext()) {
    DrawShape(painter, fixture);
  }
}

void Entity::DrawShape(QPainter* painter, b2Fixture* shape) const {
  painter->save();
  painter->scale(1, -1);

  switch (shape->GetShape()->GetType()) {
    case b2Shape::e_polygon: {
      QVector<QPoint> points;
      auto polygon = dynamic_cast<b2PolygonShape*>(shape->GetShape());
      for (int i = 0; i < polygon->m_count; i++) {
        points.push_back(Point(body_->GetWorldPoint(polygon->m_vertices[i])).ToQPoint());
      }
      painter->drawPolygon(QPolygon(points));
      break;
    }

    case b2Shape::e_circle: {
      auto circle = dynamic_cast<b2CircleShape*>(shape->GetShape());
      painter->drawEllipse(Point(body_->GetWorldCenter()).ToQPoint(),
                           static_cast<int>(circle->m_radius),
                           static_cast<int>(circle->m_radius));
      break;
    }

    default: {
      break;
    }
  }
  painter->restore();
}

b2PolygonShape Entity::CreatePolygonShape(const QPolygon& polygon,
                                          const Point& shape_position) const {
  std::vector<b2Vec2> polygon_points;

  for (auto point : polygon) {
    polygon_points.emplace_back(point.x(), point.y());
  }

  b2PolygonShape shape;
  shape.m_centroid.Set(shape_position.x, shape_position.y);
  shape.Set(polygon_points.data(), polygon_points.size());

  return shape;
}

b2CircleShape Entity::CreateCircleShape(int radius,
                                        const Point& shape_position) const {
  b2CircleShape shape;
  shape.m_radius = static_cast<float>(radius);
  shape.m_p.Set(shape_position.x, shape_position.y);

  return shape;
}

void Entity::InitializeBody(b2BodyType body_type, const Point& body_position) {
  b2BodyDef body_definition;
  body_definition.position.Set(body_position.x, body_position.y);
  body_definition.type = body_type;
  body_ = world_->CreateBody(&body_definition);
}

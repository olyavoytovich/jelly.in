#include "entity.h"


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
               float radius)
    : world_(std::move(world)) {
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

Entity::Entity(const std::shared_ptr<b2World>& world,
               b2BodyType type,
               const Point& body_position,
               const QPolygon& polygon,
               const b2Vec2& velocity)
    : Entity(world, type, body_position, polygon) {
  body_->SetLinearVelocity(velocity);
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
      QPolygon polygon;
      auto polygon_shape = dynamic_cast<b2PolygonShape*>(shape->GetShape());
      for (int i = 0; i < polygon_shape->m_count; i++) {
        b2Vec2 point = body_->GetWorldPoint(polygon_shape->m_vertices[i]);
        polygon.putPoints(i, 1,
                          static_cast<int>(point.x),
                          static_cast<int>(point.y));
      }
      painter->drawPolygon(polygon);
      break;
    }

    case b2Shape::e_circle: {
      auto circle = dynamic_cast<b2CircleShape*>(shape->GetShape());
      painter->drawEllipse(static_cast<int>(body_->GetWorldCenter().x
                               - circle->m_radius),
                           static_cast<int>(body_->GetWorldCenter().y
                               - circle->m_radius),
                           2 * static_cast<int>(circle->m_radius),
                           2 * static_cast<int>(circle->m_radius));
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

b2CircleShape Entity::CreateCircleShape(float radius,
                                        const Point& shape_position) const {
  b2CircleShape shape;
  shape.m_radius = radius;
  shape.m_p.Set(shape_position.x, shape_position.y);
  return shape;
}

void Entity::SetWayPoints(const std::vector<Point>& way_points) {
  way_points_ = way_points;
  float current_speed = CalculateSpeed(way_points_[1], way_points_[0]);
  body_->SetLinearVelocity(b2Vec2(
      (way_points[1].x - way_points[0].x) * speed_ / current_speed,
      (way_points[1].y - way_points[0].y) * speed_ / current_speed));
}

void Entity::SetSpeed(float speed) {
  speed_ = speed;
}

void Entity::Update(int) {
  b2Vec2 current_position = body_->GetPosition();
  if (abs(
      current_position.x
          - way_points_[index_of_current_point_ + direction_].x) <= 2 &&
      abs(current_position.y
              - way_points_[index_of_current_point_ + direction_].y <= 2)) {
    index_of_current_point_ += direction_;
    if (index_of_current_point_ == way_points_.size() - 1
        || index_of_current_point_ == 0) {
      direction_ *= (-1);
    }
    b2Vec2 next_point(way_points_[index_of_current_point_ + direction_].x,
                      way_points_[index_of_current_point_ + direction_].y);
    float current_speed =
        CalculateSpeed(Point(current_position), Point(next_point));
    body_->SetLinearVelocity(b2Vec2(
        (next_point.x - current_position.x) * speed_ / current_speed,
        (next_point.y - current_position.y) * speed_ / current_speed));
  }
}

float Entity::CalculateSpeed(const Point& point1, const Point& point2) const {
  float speed = sqrt(
      (point1.x - point2.x) * (point1.x - point2.x)
          + (point1.y - point2.y) * (point1.y - point2.y));
  return speed;
}

void Entity::InitializeBody(b2BodyType body_type, const Point& body_position) {
  b2BodyDef body_definition;
  body_definition.position.Set(body_position.x, body_position.y);
  body_definition.type = body_type;
  body_definition.fixedRotation = true;
  body_ = world_->CreateBody(&body_definition);
}

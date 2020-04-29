#include "entity.h"

// Про координаты. Координаты в Box2D такие же, как у математиков. То есть
// ось абсцисс направлена вправо, ось ординат - вверх.
// Но рисуем объекты мы в координатах окна. При этом центр КС бокса ставим в
// центр КС окна.
// Далее body_position - координаты данного тела в координатной системе окна.
// А shape_position - это координаты данной формы(!не тела) в системе координат
// тела. Можно сказать, что у каждого тела есть свой мини-мир со своими
// локальными координатами, и если нужно создать тело из нескольких форм,
// то каждой форме можно задавать свои координаты (shape_position).

Entity::Entity(std::shared_ptr<Map> map,
               b2BodyType body_type,
               const QPoint& body_position,
               const QPolygon& polygon,
               EntityType entity_type)
    : map_(std::move(map)), entity_type_(entity_type) {
  InitializeBody(body_type, body_position);
  b2PolygonShape shape = CreatePolygonShape(polygon);
  CreateFixture(shape);
  InitializeBoundaryRectangle();
}

Entity::Entity(std::shared_ptr<Map> map,
               b2BodyType body_type,
               const QPoint& body_position,
               int radius,
               EntityType entity_type)
    : map_(std::move(map)), entity_type_(entity_type) {
  InitializeBody(body_type, body_position);
  b2CircleShape shape = CreateCircleShape(radius);
  CreateFixture(shape);
  InitializeBoundaryRectangle();
}

Entity::Entity(std::shared_ptr<Map> map,
               b2BodyType body_type,
               const QPoint& body_position,
               const std::vector<CircleShape>& circles,
               const std::vector<PolygonShape>& polygons,
               EntityType entity_type)
    : map_(std::move(map)), entity_type_(entity_type) {
  InitializeBody(body_type, body_position);

  for (auto&&[radius, shape_position] : circles) {
    b2CircleShape shape = CreateCircleShape(radius, shape_position);
    CreateFixture(shape);
  }

  for (auto&&[polygon, shape_position] : polygons) {
    b2PolygonShape shape = CreatePolygonShape(polygon, shape_position);
    CreateFixture(shape);
  }
  InitializeBoundaryRectangle();
}

void Entity::Draw(QPainter* painter) const {
  // Fixture используется, чтобы прикрепить форму к телу для обнаружения
  // коллизий. Содержит необходимые для отрисовки геометрические данные,
  // кроме них - трение, фильтр коллизий и др.
  if (animator_ != nullptr) {
    QRect rectangle_for_image = bounding_rectangle_;
    rectangle_for_image.translate(GetPositionInPixels());
    int width =
        static_cast<int>(bounding_rectangle_.width() * map_->GetScale());
    int height =
        static_cast<int>(bounding_rectangle_.height() * map_->GetScale());
    painter->drawImage(rectangle_for_image.topLeft() * map_->GetScale(),
                       *(animator_->GetCurrentImage(width, height)));
  }
}

void Entity::DrawShape(QPainter* painter, b2Fixture* shape) const {
  if (shape->IsSensor()) {
    return;
  }
  painter->save();

  switch (shape->GetShape()->GetType()) {
    case b2Shape::e_polygon: {
      QPolygon polygon;
      auto polygon_shape = dynamic_cast<b2PolygonShape*>(shape->GetShape());
      for (int i = 0; i < polygon_shape->m_count; i++) {
        b2Vec2 point = body_->GetWorldPoint(polygon_shape->m_vertices[i]);
        polygon.putPoints(i, 1,
                          MetersToPixels(point.x),
                          MetersToPixels(point.y));
      }
      painter->drawPolygon(polygon);
      break;
    }

    case b2Shape::e_circle: {
      auto circle = dynamic_cast<b2CircleShape*>(shape->GetShape());
      int radius = MetersToPixels(circle->m_radius);
      QPoint center = MetersToPixels(body_->GetWorldCenter());
      painter->drawEllipse(center.x() - radius,
                           center.y() - radius,
                           2 * radius,
                           2 * radius);
      break;
    }

    default: {
      break;
    }
  }
  painter->restore();
}

b2PolygonShape Entity::CreatePolygonShape(const QPolygon& polygon,
                                          const QPoint& shape_position) const {
  std::vector<b2Vec2> polygon_points;

  for (auto point : polygon) {
    polygon_points.emplace_back(PixelsToMeters(point));
  }

  b2PolygonShape shape;
  shape.m_centroid = PixelsToMeters(shape_position);
  shape.Set(polygon_points.data(), polygon_points.size());

  return shape;
}

b2CircleShape Entity::CreateCircleShape(int radius,
                                        const QPoint& shape_position) const {
  b2CircleShape shape;
  shape.m_radius = PixelsToMeters(radius);
  shape.m_p = PixelsToMeters(shape_position);
  return shape;
}

void Entity::SetAnimator(std::shared_ptr<Animator> animator) {
  animator_ = std::move(animator);
}

void Entity::SetWayPoints(const std::vector<QPoint>& way_points) {
  way_points_.clear();
  if (way_points.empty()) {
    return;
  }

  b2Vec2 body_position = body_->GetWorldCenter();
  if ((body_position - PixelsToMeters(way_points[0])).Length() > 0.02) {
    way_points_.emplace_back(body_position);
  }
  for (const auto& point : way_points) {
    way_points_.emplace_back(PixelsToMeters(point));
  }
}

void Entity::SetSpeed(int speed) {
  speed_ = PixelsToMeters(speed);
}

void Entity::SetVelocity(b2Vec2 velocity, bool apply_once) {
  if (body_->GetType() == b2_kinematicBody) {
    body_->SetLinearVelocity(velocity);
  }
  target_velocity = velocity;
  if (apply_once) {
    ApplyImpulse();
    target_velocity = b2Vec2(0, 0);
  }
}

void Entity::SetVelocity(b2Vec2 target_position,
                         b2Vec2 current_position,
                         float speed, bool apply_once) {
  b2Vec2 velocity = target_position - current_position;
  velocity.Normalize();
  velocity *= speed;
  SetVelocity(velocity, apply_once);
}

void Entity::SetEntityType(EntityType entity_type) {
  entity_type_ = entity_type;
  for (auto fixture = body_->GetFixtureList(); fixture != nullptr;
       fixture = fixture->GetNext()) {
    ApplyEntityType(fixture);
  }
}

void Entity::ApplyEntityType(b2Fixture* fixture) {
  fixture->SetUserData(static_cast<void*>(&entity_type_));
  b2Filter filter = fixture->GetFilterData();
  filter.categoryBits = static_cast<int>(entity_type_);
  fixture->SetFilterData(filter);
}

void Entity::Update(int time) {
  if (animator_ != nullptr) {
    animator_->Update(time);
  }
  if (way_points_.size() <= 1) {
    return;
  }
  ApplyImpulse();

  b2Vec2 body_position = body_->GetWorldCenter();
  if ((body_position - way_points_[way_point_index_]).Length() <= 0.02) {
    way_point_index_ += direction_;
    if (way_point_index_ == way_points_.size() - 1 || way_point_index_ == 0) {
      direction_ *= -1;
    }
    SetVelocity(way_points_[way_point_index_], body_position, speed_);
  }
}

void Entity::InitializeBody(b2BodyType body_type, const QPoint& body_position) {
  b2BodyDef body_definition;
  body_definition.position = PixelsToMeters(body_position);
  body_definition.type = body_type;
  body_definition.fixedRotation = true;
  body_definition.userData = static_cast<void*>(this);
  body_ = map_->CreateBody(&body_definition);
}

int Entity::MetersToPixels(float value) const {
  return static_cast<int>(value * kPixelsPerMeter);
}

QPoint Entity::MetersToPixels(b2Vec2 vector) const {
  return QPoint(MetersToPixels(vector.x), MetersToPixels(vector.y));
}

float Entity::PixelsToMeters(int value) const {
  return static_cast<float>(value) / kPixelsPerMeter;
}

b2Vec2 Entity::PixelsToMeters(QPoint vector) const {
  return b2Vec2(PixelsToMeters(vector.x()), PixelsToMeters(vector.y()));
}

b2Fixture* Entity::CreateFixture(const b2Shape& shape) {
  b2Fixture* fixture = body_->CreateFixture(&shape, kBodyDensity);
  ApplyEntityType(fixture);
  return fixture;
}

void Entity::SetNoCollisionMask(uint16_t mask) {
  for (auto fixture = body_->GetFixtureList(); fixture != nullptr;
       fixture = fixture->GetNext()) {
    b2Filter filter = fixture->GetFilterData();
    filter.maskBits = ~mask;
    fixture->SetFilterData(filter);
  }
}

b2Body* Entity::GetB2Body() const {
  return body_;
}

void Entity::ApplyImpulse() {
  b2Vec2 target_impulse = target_velocity - body_->GetLinearVelocity();
  target_impulse *= body_->GetMass();
  body_->ApplyLinearImpulseToCenter(target_impulse, true);
}

void Entity::InitializeBoundaryRectangle() {
  QPoint left_point = QPoint(20000, 20000);
  QPoint right_point = QPoint(-20000, -20000);
  for (auto fixture = body_->GetFixtureList(); fixture != nullptr;
       fixture = fixture->GetNext()) {
    switch (fixture->GetShape()->GetType()) {
      case b2Shape::e_polygon: {
        auto polygon_shape = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
        for (int i = 0; i < polygon_shape->m_count; i++) {
          QPoint vertex = MetersToPixels(polygon_shape->m_vertices[i]);
          left_point.rx() = std::min(left_point.x(), vertex.x());
          right_point.rx() = std::max(right_point.x(), vertex.x());
          left_point.ry() = std::min(left_point.y(), vertex.y());
          right_point.ry() = std::max(right_point.y(), vertex.y());
        }
        break;
      }

      case b2Shape::e_circle: {
        auto circle_shape = dynamic_cast<b2CircleShape*>(fixture->GetShape());
        int radius = MetersToPixels(circle_shape->m_radius);
        left_point.rx() = std::min(left_point.x(), -radius);
        right_point.rx() = std::max(right_point.x(), radius);
        left_point.ry() = std::min(left_point.y(), -radius);
        right_point.ry() = std::max(right_point.y(), radius);
        break;
      }

      default: {
        break;
      }
    }
  }
  bounding_rectangle_ = QRect(left_point, right_point);
}

QPoint Entity::GetPositionInPixels() const {
  return MetersToPixels(body_->GetWorldCenter());
}

QRect Entity::GetBoundings() const {
  return bounding_rectangle_.translated(GetPositionInPixels());
}

void Entity::BeginCollision(b2Fixture*,
                            EntityType my_type,
                            EntityType other_type) {
  if (my_type == EntityType::kBullet && other_type == EntityType::kGround) {
    MarkAsDeleted();
  }
}

void Entity::EndCollision(b2Fixture*) {}

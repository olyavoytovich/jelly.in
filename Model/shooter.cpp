#include "shooter.h"

Shooter::Shooter(std::shared_ptr<b2World> world,
                 b2BodyType type,
                 const Point& body_position,
                 const QPolygon& polygon,
                 const std::vector<Point>& way_points,
                 BulletDirection bullet_direction,
                 int period,
                 float bullet_speed,
                 float speed)
    : Entity(std::move(world), type, body_position, polygon) {
  SetSpeed(speed);
  SetWayPoints(way_points);
  period_ = period;
  bullet_speed_ = bullet_speed;
  bullet_direction_ = bullet_direction;
  auto polygon_shape =
      dynamic_cast<b2PolygonShape*>(body_->GetFixtureList()->GetShape());
  float left_side = polygon_shape->m_vertices[0].x,
      right_side = polygon_shape->m_vertices[0].x,
      bottom = polygon_shape->m_vertices[0].y,
      top = polygon_shape->m_vertices[0].y;
  CheckPolygonPoints(body_->GetFixtureList(),
                     &left_side,
                     &right_side,
                     &bottom,
                     &top);
  if (bullet_direction_ == BulletDirection::kLeftRight) {
    left_point_.Set(left_side - 20, (top - bottom) / 2);
    right_point_.Set(right_side + 10, (top - bottom) / 2);
  } else {
    left_point_.Set(left_side, bottom - 10);
    right_point_.Set(right_side, bottom - 10);
  }
}

Shooter::Shooter(std::shared_ptr<b2World> world,
                 b2BodyType type,
                 const Point& body_position,
                 float radius,
                 const std::vector<Point>& way_points,
                 BulletDirection bullet_direction,
                 int period, float bullet_speed,
                 float speed)
    : Entity(std::move(world), type, body_position, radius) {
  SetSpeed(speed);
  SetWayPoints(way_points);
  period_ = period;
  bullet_speed_ = bullet_speed;
  bullet_direction_ = bullet_direction;
  if (bullet_direction_ == BulletDirection::kLeftRight) {
    left_point_.Set(-radius - 20, 0);
    right_point_.Set(radius + 10, 0);
  } else {
    left_point_.Set(-radius, -10);
    right_point_.Set(radius, -10);
  }
}

Shooter::Shooter(std::shared_ptr<b2World> world,
                 b2BodyType body_type,
                 const Point& body_position,
                 const std::vector<CircleShape>& circles,
                 const std::vector<PolygonShape>& polygons,
                 const std::vector<Point>& way_points,
                 BulletDirection bullet_direction,
                 int period, float bullet_speed,
                 float speed)
    : Entity(std::move(world), body_type, body_position, circles, polygons) {
  SetSpeed(speed);
  SetWayPoints(way_points);
  period_ = period;
  bullet_speed_ = bullet_speed;
  bullet_direction_ = bullet_direction;

  float left_side = 20000, right_side = -20000, bottom = 20000, top = -20000;
  for (auto fixture = body_->GetFixtureList(); fixture != nullptr;
       fixture = fixture->GetNext()) {
    switch (fixture->GetShape()->GetType()) {
      case b2Shape::e_polygon: {
        CheckPolygonPoints(fixture, &left_side, &right_side, &bottom, &top);
        break;
      }

      case b2Shape::e_circle: {
        auto circle_shape = dynamic_cast<b2CircleShape*>(fixture->GetShape());
        left_side = std::min(left_side, -(circle_shape->m_radius));
        right_side = std::max(right_side, circle_shape->m_radius);
        bottom = std::min(bottom, -(circle_shape->m_radius));
        top = std::max(top, circle_shape->m_radius);
        break;
      }

      default: {
        break;
      }
    }
  }
  if (bullet_direction == BulletDirection::kLeftRight) {
    left_point_.Set(left_side - 20, (top - bottom) / 2);
    right_point_.Set(right_side + 10, (top - bottom) / 2);
  } else {
    left_point_.Set(left_side, bottom - 10);
    right_point_.Set(right_side, bottom - 10);
  }
}

void Shooter::Draw(QPainter* painter) const {
  Entity::Draw(painter);
  for (const auto& bullet : bullets_) {
    bullet->Draw(painter);
  }
}

void Shooter::Update(int time) {
  Entity::Update(time);
  time_ += time;
  if (time_ >= period_) {
    time_ = 0;
    if (bullets_.size() > 50) {
      bullets_.erase(bullets_.begin(),
                     bullets_.begin()
                         + static_cast<int>(right_point_.x - left_point_.x)
                             / 15 + 1);
    }
    b2Vec2 velocity(way_points_[index_of_current_point_ + direction_].x
                        - body_->GetPosition().x,
                    way_points_[index_of_current_point_ + direction_].y
                        - body_->GetPosition().y);
    float current_speed =
        CalculateSpeed(way_points_[index_of_current_point_ + direction_],
                       Point(body_->GetPosition()));
    velocity.x = velocity.x * bullet_speed_ / current_speed;
    velocity.y = velocity.y * bullet_speed_ / current_speed;

    if (bullet_direction_ == BulletDirection::kLeftRight) {
      if (velocity.x > 0) {
        AddBullet(Point(body_->GetWorldPoint(right_point_)), velocity);
      } else {
        AddBullet(Point(body_->GetWorldPoint(left_point_)), velocity);
      }
    } else {
      for (int i = 0; i <= static_cast<int>(right_point_.x - left_point_.x);
           i += 15) {
        b2Vec2 bullet_position
            (left_point_.x + static_cast<float>(i), left_point_.y);
        AddBullet(Point(body_->GetWorldPoint(bullet_position)),
                  b2Vec2(velocity.x, -speed_));
      }
    }
  }
}

void Shooter::AddBullet(const Point& bullet_position, const b2Vec2& velocity) {
  bullets_.emplace_back(std::make_shared<Entity>(world_, b2_kinematicBody,
                                                 bullet_position,
                                                 QPolygon(QRect(0,
                                                                -5,
                                                                10,
                                                                10)),
                                                 velocity));
}

void Shooter::CheckPolygonPoints(b2Fixture* fixture,
                                 float* left_side,
                                 float* right_side,
                                 float* bottom,
                                 float* top) const {
  auto polygon_shape = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
  for (int i = 1; i < polygon_shape->m_count; i++) {
    *left_side = std::min(*left_side, polygon_shape->m_vertices[i].x);
    *right_side = std::max(*right_side, polygon_shape->m_vertices[i].x);
    *bottom = std::min(*bottom, polygon_shape->m_vertices[i].y);
    *top = std::max(*top, polygon_shape->m_vertices[i].y);
  }
}


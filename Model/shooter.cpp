#include "shooter.h"

Shooter::Shooter(std::shared_ptr<b2World> world, b2BodyType type,
                 const Point& body_position, const QPolygon& polygon,
                 const std::vector<Point>& way_points,
                 BulletDirection bullet_direction,
                 int shoot_period, float bullet_speed,
                 float bullet_radius, float speed)
    : Entity(std::move(world), type, body_position, polygon),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(bullet_speed),
      bullet_radius_(bullet_radius) {
  SetSpeed(speed);
  SetWayPoints(way_points);
  InitializeBoundaryPoints();
}

Shooter::Shooter(std::shared_ptr<b2World> world, b2BodyType type,
                 const Point& body_position,
                 float radius, const std::vector<Point>& way_points,
                 BulletDirection bullet_direction,
                 int shoot_period, float bullet_speed,
                 float bullet_radius, float speed)
    : Entity(std::move(world), type, body_position, radius),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(bullet_speed),
      bullet_radius_(bullet_radius) {
  SetSpeed(speed);
  SetWayPoints(way_points);
  InitializeBoundaryPoints();
}

Shooter::Shooter(std::shared_ptr<b2World> world, b2BodyType body_type,
                 const Point& body_position,
                 const std::vector<CircleShape>& circles,
                 const std::vector<PolygonShape>& polygons,
                 const std::vector<Point>& way_points,
                 BulletDirection bullet_direction,
                 int shoot_period, float bullet_speed,
                 float bullet_radius, float speed)
    : Entity(std::move(world), body_type, body_position, circles, polygons),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(bullet_speed),
      bullet_radius_(bullet_radius) {
  SetSpeed(speed);
  SetWayPoints(way_points);
  InitializeBoundaryPoints();
}

void Shooter::Draw(QPainter* painter) const {
  Entity::Draw(painter);
  for (const auto& bullet : bullets_) {
    bullet->Draw(painter);
  }
}

void Shooter::Update(int time) {
  Entity::Update(time);
  last_shoot_time_ += time;
  if (last_shoot_time_ < shoot_period_) {
    return;
  }
  if (way_points_.size() <= 1) {
    return;
  }
  last_shoot_time_ = 0;
  if (bullets_.size() > 50) {
    bullets_.erase(bullets_.begin(),
                   bullets_.begin()
                       + static_cast<int>(right_point_.x - left_point_.x)
                           / (3 * bullet_radius_) + 1);
  }
  if (bullet_direction_ == BulletDirection::kLeftRight) {
    if (way_points_[way_point_index_].ToB2Vec2().x - body_->GetPosition().x
        >= 0) {
      AddBullet(Point(body_->GetWorldPoint(right_point_)));
    } else {
      AddBullet(Point(body_->GetWorldPoint(left_point_)));
    }
    bullets_.back()->SetVelocity(way_points_[way_point_index_].ToB2Vec2(),
                                 body_->GetPosition(),
                                 bullet_speed_);
  } else {
    int width = static_cast<int>(right_point_.x - left_point_.x);
    for (int i = 0; i <= width; i += 3 * bullet_radius_) {
      b2Vec2 bullet_position
          (left_point_.x + static_cast<float>(i),
           right_point_.y - bullet_radius_);
      AddBullet(Point(body_->GetWorldPoint(bullet_position)));
      bullets_.back()->SetVelocity(b2Vec2(0, -bullet_speed_));
    }
  }

}

void Shooter::AddBullet(const Point& bullet_position) {
  bullets_.emplace_back(std::make_shared<Entity>(world_,
                                                 b2_kinematicBody,
                                                 bullet_position,
                                                 bullet_radius_));
}

void Shooter::InitializeBoundaryPoints() {
  for (auto fixture = body_->GetFixtureList(); fixture != nullptr;
       fixture = fixture->GetNext()) {
    switch (fixture->GetShape()->GetType()) {
      case b2Shape::e_polygon: {
        b2PolygonShape* polygon_shape =
            dynamic_cast<b2PolygonShape*>(fixture->GetShape());
        for (int i = 0; i < polygon_shape->m_count; i++) {
          left_point_.x =
              std::min(left_point_.x, polygon_shape->m_vertices[i].x);
          right_point_.x =
              std::max(right_point_.x, polygon_shape->m_vertices[i].x);
          left_point_.y =
              std::max(left_point_.y, polygon_shape->m_vertices[i].y);
          right_point_.y =
              std::min(right_point_.y, polygon_shape->m_vertices[i].y);
        }
        break;
      }

      case b2Shape::e_circle: {
        auto circle_shape = dynamic_cast<b2CircleShape*>(fixture->GetShape());
        left_point_.x = std::min(left_point_.x, -circle_shape->m_radius);
        right_point_.x = std::max(right_point_.x, circle_shape->m_radius);
        left_point_.y = std::max(left_point_.y, circle_shape->m_radius);
        right_point_.y = std::min(right_point_.y, -circle_shape->m_radius);
        break;
      }

      default: {
        break;
      }
    }
  }
  if (bullet_direction_ == BulletDirection::kLeftRight) {
    float coordinate_y = (left_point_.y + right_point_.y) / 2;
    left_point_.Set(left_point_.x - bullet_radius_, coordinate_y);
    right_point_.Set(right_point_.x + bullet_radius_, coordinate_y);
  } else {
    left_point_.Set(left_point_.x,
                    right_point_.y - bullet_radius_);
    right_point_.Set(right_point_.x,
                     right_point_.y - bullet_radius_);
  }
}

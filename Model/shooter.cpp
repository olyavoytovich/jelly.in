#include "shooter.h"

#include <utility>

Shooter::Shooter(std::shared_ptr<Map> map, b2BodyType type,
                 const QPoint& body_position, const QPolygon& polygon,
                 const std::vector<QPoint>& way_points,
                 BulletDirection bullet_direction,
                 int shoot_period, int bullet_speed,
                 int bullet_radius, std::shared_ptr<Animator> animator,
                 std::shared_ptr<Animator> bullet_animator, int speed)
    : Entity(std::move(map), type, body_position, polygon),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(PixelsToMeters(bullet_speed)),
      bullet_radius_(PixelsToMeters(bullet_radius)),
      bullet_animator_(std::move(bullet_animator)) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  SetWayPoints(way_points);
  InitializeBoundaryPoints();
}

Shooter::Shooter(std::shared_ptr<Map> map, b2BodyType type,
                 const QPoint& body_position,
                 int radius, const std::vector<QPoint>& way_points,
                 BulletDirection bullet_direction,
                 int shoot_period, int bullet_speed,
                 int bullet_radius, std::shared_ptr<Animator> animator,
                 std::shared_ptr<Animator> bullet_animator, int speed)
    : Entity(std::move(map), type, body_position, radius),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(PixelsToMeters(bullet_speed)),
      bullet_radius_(PixelsToMeters(bullet_radius)),
      bullet_animator_(std::move(bullet_animator)) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  SetWayPoints(way_points);
  InitializeBoundaryPoints();
}

Shooter::Shooter(std::shared_ptr<Map> map, b2BodyType body_type,
                 const QPoint& body_position,
                 const std::vector<CircleShape>& circles,
                 const std::vector<PolygonShape>& polygons,
                 const std::vector<QPoint>& way_points,
                 BulletDirection bullet_direction,
                 int shoot_period, int bullet_speed,
                 int bullet_radius,
                 std::shared_ptr<Animator> animator,
                 std::shared_ptr<Animator> bullet_animator,
                 int speed)
    : Entity(std::move(map), body_type, body_position, circles, polygons),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(PixelsToMeters(bullet_speed)),
      bullet_radius_(PixelsToMeters(bullet_radius)),
      bullet_animator_(std::move(bullet_animator)) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
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
    int bullets_amount =
        static_cast<int>((right_point_.x - left_point_.x) / 3 / bullet_radius_);
    for (int i = 0; i <= bullets_amount; i++) {
      bullets_[i]->MarkAsDeleted();
    }
    bullets_.erase(bullets_.begin(), bullets_.begin() + bullets_amount + 1);
  }
  if (bullet_direction_ == BulletDirection::kLeftRight) {
    if (way_points_[way_point_index_].x - body_->GetPosition().x >= 0) {
      AddBullet(body_->GetWorldPoint(right_point_));
    } else {
      AddBullet(body_->GetWorldPoint(left_point_));
    }
    bullets_.back()->SetVelocity(way_points_[way_point_index_],
                                 body_->GetPosition(),
                                 bullet_speed_, true);
  } else {
    int bullets_amount =
        static_cast<int>((right_point_.x - left_point_.x) / bullet_radius_ / 3);
    for (int i = 0; i <= bullets_amount; i++) {
      b2Vec2 bullet_position
          (left_point_.x + bullet_radius_ * static_cast<float>(i * 3),
           left_point_.y + bullet_radius_);
      AddBullet(body_->GetWorldPoint(bullet_position));
      bullets_.back()->SetVelocity(b2Vec2(0, bullet_speed_), true);
    }
  }
}

void Shooter::AddBullet(const b2Vec2& bullet_position) {
  bullets_.push_back(std::make_shared<Entity>(map_,
                                              b2_dynamicBody,
                                              MetersToPixels(bullet_position),
                                              MetersToPixels(bullet_radius_)));
  bullets_.back()->SetAnimator(bullet_animator_);
  map_->AddGameObject(bullets_.back());
}

void Shooter::InitializeBoundaryPoints() {
  for (auto fixture = body_->GetFixtureList(); fixture != nullptr;
       fixture = fixture->GetNext()) {
    switch (fixture->GetShape()->GetType()) {
      case b2Shape::e_polygon: {
        auto polygon_shape = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
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
    left_point_.Set(left_point_.x - bullet_radius_, 0);
    right_point_.Set(right_point_.x + bullet_radius_, 0);
  } else {
    left_point_.Set(left_point_.x,
                    left_point_.y + bullet_radius_);
    right_point_.Set(right_point_.x,
                     left_point_.y + bullet_radius_);
  }
}


#include "shooter.h"

Shooter::Shooter(std::shared_ptr<Map> map,
                 b2BodyType type,
                 const QPoint& body_position,
                 const QPolygon& polygon,
                 const std::vector<QPoint>& way_points,
                 BulletDirection bullet_direction,
                 int shoot_period,
                 int bullet_speed,
                 int bullet_radius,
                 std::shared_ptr<Animator> animator,
                 std::shared_ptr<Animator> bullet_animator,
                 int speed)
    : Entity(std::move(map), type, body_position, polygon,
        EntityType::kShooter),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(PixelsToMeters(bullet_speed)),
      bullet_radius_(bullet_radius),
      bullet_animator_(std::move(bullet_animator)) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  animator_->RepeatInReverseOrder();
  SetWayPoints(way_points);
}

Shooter::Shooter(std::shared_ptr<Map> map,
                 b2BodyType type,
                 const QPoint& body_position,
                 int radius,
                 const std::vector<QPoint>& way_points,
                 BulletDirection bullet_direction,
                 int shoot_period,
                 int bullet_speed,
                 int bullet_radius,
                 std::shared_ptr<Animator> animator,
                 std::shared_ptr<Animator> bullet_animator,
                 int speed)
    : Entity(std::move(map), type, body_position, radius, EntityType::kShooter),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(PixelsToMeters(bullet_speed)),
      bullet_radius_(bullet_radius),
      bullet_animator_(std::move(bullet_animator)) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  SetWayPoints(way_points);
}

Shooter::Shooter(std::shared_ptr<Map> map,
                 b2BodyType body_type,
                 const QPoint& body_position,
                 const std::vector<CircleShape>& circles,
                 const std::vector<PolygonShape>& polygons,
                 const std::vector<QPoint>& way_points,
                 BulletDirection bullet_direction,
                 int shoot_period,
                 int bullet_speed,
                 int bullet_radius,
                 std::shared_ptr<Animator> animator,
                 std::shared_ptr<Animator> bullet_animator,
                 int speed)
    : Entity(std::move(map), body_type, body_position, circles, polygons,
             EntityType::kShooter),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(PixelsToMeters(bullet_speed)),
      bullet_radius_(bullet_radius),
      bullet_animator_(std::move(bullet_animator)) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  SetWayPoints(way_points);
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
  int bullets_amount = bounding_rectangle_.width() / 3 / bullet_radius_;
  if (bullets_.size() > 50) {
    for (int i = 0; i <= bullets_amount; i++) {
      bullets_[i]->MarkAsDeleted();
    }
    bullets_.erase(bullets_.begin(), bullets_.begin() + bullets_amount + 1);
  }
  if (bullet_direction_ == BulletDirection::kLeftRight) {
    if (way_points_[way_point_index_].x - body_->GetPosition().x >= 0) {
      AddBullet(QPoint(bounding_rectangle_.right() + bullet_radius_, 0));
    } else {
      AddBullet(QPoint(bounding_rectangle_.left() - bullet_radius_, 0));
    }
    bullets_.back()->SetVelocity(way_points_[way_point_index_],
                                 body_->GetPosition(),
                                 bullet_speed_, true);
  } else {
    for (int i = 0; i <= bullets_amount; i++) {
      QPoint bullet_position
          (bounding_rectangle_.left() + bullet_radius_ * i * 3,
           bounding_rectangle_.bottom() + bullet_radius_);
      AddBullet(bullet_position);
      bullets_.back()->SetVelocity(b2Vec2(0, bullet_speed_), true);
    }
  }
}

void Shooter::AddBullet(const QPoint& bullet_position) {
  bullets_.push_back(std::make_shared<Entity>(map_,
                                              b2_dynamicBody,
                                              bullet_position +
                                              GetPositionInPixels(),
                                              bullet_radius_,
                                              EntityType::kBullet));
  bullets_.back()->SetAnimator(bullet_animator_);
  animator_->Play();
  map_->AddGameObject(bullets_.back());
}

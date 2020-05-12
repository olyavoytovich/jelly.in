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
                 EntityType entity_type,
                 int speed)
    : Entity(std::move(map), type, body_position, polygon, entity_type),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(PixelsToMeters(bullet_speed)),
      bullet_radius_(bullet_radius),
      bullet_animator_(std::move(bullet_animator)) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  animator_->RepeatInReverseOrder();
  SetWayPoints(way_points);
  if (entity_type == EntityType::kBurdock) {
    bullet_sound_.AddMedia("qrc:/sound/enemy/burdock_bullet.mp3");
    sounds_.AddSon(std::make_shared<SoundManager>(bullet_sound_));
  }
  if (entity_type == EntityType::kCloud) {
    permanent_sound_.AddMedia("qrc:/sound/enemy/cloud.mp3");
    sounds_.AddSon(std::make_shared<SoundManager>(permanent_sound_));
  }
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
                 EntityType entity_type,
                 int speed)
    : Entity(std::move(map), type, body_position, radius, entity_type),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(PixelsToMeters(bullet_speed)),
      bullet_radius_(bullet_radius),
      bullet_animator_(std::move(bullet_animator)) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  SetWayPoints(way_points);
  if (entity_type == EntityType::kBurdock) {
    bullet_sound_.AddMedia("qrc:/sound/enemy/burdock_bullet.mp3");
    sounds_.AddSon(std::make_shared<SoundManager>(bullet_sound_));
  }
  if (entity_type == EntityType::kCloud) {
    permanent_sound_.AddMedia("qrc:/sound/enemy/cloud.mp3");
    sounds_.AddSon(std::make_shared<SoundManager>(permanent_sound_));
  }
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
                 EntityType entity_type,
                 int speed)
    : Entity(std::move(map), body_type, body_position, circles, polygons,
             entity_type),
      bullet_direction_(bullet_direction),
      shoot_period_(shoot_period),
      bullet_speed_(PixelsToMeters(bullet_speed)),
      bullet_radius_(bullet_radius),
      bullet_animator_(std::move(bullet_animator)) {
  SetSpeed(speed);
  SetAnimator(std::move(animator));
  SetWayPoints(way_points);
  if (entity_type == EntityType::kBurdock) {
    bullet_sound_.AddMedia("qrc:/sound/enemy/burdock_bullet.mp3");
    sounds_.AddSon(std::make_shared<SoundManager>(bullet_sound_));
  }
  if (entity_type == EntityType::kCloud) {
    permanent_sound_.AddMedia("qrc:/sound/enemy/cloud.mp3");
    sounds_.AddSon(std::make_shared<SoundManager>(permanent_sound_));
  }
}

void Shooter::Update(int time) {
  permanent_sound_.SetVolumeByDistance(CountDistance());
  permanent_sound_.Play();
  Entity::Update(time);
  last_shoot_time_ += time;
  if (last_shoot_time_ < shoot_period_) {
    return;
  }
  last_shoot_time_ = 0;
  if (bullet_direction_ == BulletDirection::kLeftRight) {
    if (way_points_.size() <= 1) {
      return;
    }

    if (way_points_[way_point_index_].x - body_->GetPosition().x >= 0) {
      std::shared_ptr<Entity> bullet =
          CreateBullet(QPoint(bounding_rectangle_.right() + bullet_radius_, 0));
      bullet->SetVelocity(way_points_[way_point_index_],
                          body_->GetPosition(),
                          bullet_speed_, true);
    } else {
      std::shared_ptr<Entity> bullet =
          CreateBullet(QPoint(bounding_rectangle_.left() - bullet_radius_, 0));
      bullet->SetVelocity(way_points_[way_point_index_],
                          body_->GetPosition(),
                          bullet_speed_, true);
    }
  } else {
    int bullets_amount = bounding_rectangle_.width() / 3 / bullet_radius_;
    for (int i = 0; i <= bullets_amount; i++) {
      QPoint bullet_position
          (bounding_rectangle_.left() + bullet_radius_ * i * 3,
           bounding_rectangle_.bottom() + bullet_radius_);
      std::shared_ptr<Entity> bullet = CreateBullet(bullet_position);
      bullet->SetVelocity(b2Vec2(0, bullet_speed_), true);
    }
  }
}

std::shared_ptr<Entity> Shooter::CreateBullet(const QPoint& bullet_position) {
  bullet_sound_.SetVolumeByDistance(CountDistance());
  bullet_sound_.Play();
  auto bullet = std::make_shared<Entity>(map_,
                                         b2_dynamicBody,
                                         bullet_position +
                                             GetPositionInPixels(),
                                         bullet_radius_,
                                         EntityType::kBullet);
  bullet->SetAnimator(bullet_animator_);
  map_->AddGameObject(bullet);
  animator_->Play();
  return bullet;
}

int Shooter::CountDistance() {
  if (player_ == nullptr) {
    player_ = map_->GetPlayer();
  }
  if (player_ == nullptr) {
      return 100000000;
  }
  QPoint begin = player_->GetPositionInPixels();
  QPoint end = this->GetPositionInPixels();
  return static_cast<int>(sqrt((begin.x() - end.x()) * (begin.x() - end.x()) +
                               (begin.y() - end.y()) * (begin.y() - end.y())));
}

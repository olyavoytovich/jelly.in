#include "shooter.h"

Shooter::Shooter(std::weak_ptr<Map> map,
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
  InitializeAudio();
}

void Shooter::Update(int time) {
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
          CreateBullet(QPoint(bounding_rectangle_.right() + 2 * bullet_radius_,
                              0));
      if (GetEntityType() == EntityType::kBurdock) {
          map_.lock()->GetAudioManager()->SetVolume(
                      thorn_audio_key_, CountVolumeFromDistance());
          map_.lock()->GetAudioManager()->PlayAudioPlayer(thorn_audio_key_);
      }
      bullet->SetVelocity(way_points_[way_point_index_],
                          body_->GetPosition(),
                          bullet_speed_, true);
    } else {
      std::shared_ptr<Entity> bullet =
          CreateBullet(QPoint(bounding_rectangle_.left() - 2 * bullet_radius_,
                              0));
      if (GetEntityType() == EntityType::kBurdock) {
          map_.lock()->GetAudioManager()->SetVolume(
                      thorn_audio_key_, CountVolumeFromDistance());
          map_.lock()->GetAudioManager()->PlayAudioPlayer(thorn_audio_key_);
      }
      bullet->SetVelocity(way_points_[way_point_index_],
                          body_->GetPosition(),
                          bullet_speed_, true);
    }
  } else {
    int bullets_amount = bounding_rectangle_.width() / 3 / bullet_radius_;
    for (int i = 0; i <= bullets_amount; i++) {
      QPoint bullet_position
          (bounding_rectangle_.left() + bullet_radius_ * i * 3,
           bounding_rectangle_.bottom() + 2 * bullet_radius_);
      std::shared_ptr<Entity> bullet = CreateBullet(bullet_position);
      if (GetEntityType() == EntityType::kCloud) {
          map_.lock()->GetAudioManager()->SetVolume(
                      drop_audio_keys_[i], CountVolumeFromDistance());
          map_.lock()->GetAudioManager()->PlayAudioPlayer(drop_audio_keys_[i]);
      }
      bullet->SetVelocity(b2Vec2(0, bullet_speed_), true);
    }
  }
}

std::shared_ptr<Entity> Shooter::CreateBullet(const QPoint& bullet_position) {
  auto bullet = std::make_shared<Entity>(map_,
                                         b2_dynamicBody,
                                         bullet_position +
                                             GetPositionInPixels(),
                                         bullet_radius_,
                                         EntityType::kBullet);
  bullet->SetAnimator(bullet_animator_);
  map_.lock()->AddGameObject(bullet);
  animator_->Play();
  return bullet;
}

void Shooter::InitializeAudio() {
  if (GetEntityType() == EntityType::kBurdock) {
    thorn_audio_key_ = map_.lock()->GetAudioManager()->
            CreateAudioPlayer(AudioName::kThorn);
  }
  if (GetEntityType() == EntityType::kCloud) {
    drop_audio_keys_.resize(bounding_rectangle_.width() / 3 / bullet_radius_+1);
    for (auto& drop_audio_key : drop_audio_keys_) {
      drop_audio_key = map_.lock()->GetAudioManager()->
              CreateAudioPlayer(AudioName::kDrop);
    }
  }
}

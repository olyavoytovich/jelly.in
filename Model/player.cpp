#include "player.h"

Player::Player(std::weak_ptr<Map> map,
               const QPoint& body_position,
               const QRect& rectangle,
               std::shared_ptr<Animator> animator)
    : Entity(std::move(map), b2_dynamicBody, body_position, rectangle,
             EntityType::kPlayer) {
  SetAnimator(std::move(animator));
  animator_->RepeatInReverseOrder();

  QPolygon bottom_rectangle = QRect(rectangle.left() + 1,
                                    rectangle.bottom() - 1,
                                    rectangle.width() - 2,
                                    2);
  b2PolygonShape bottom_shape = CreatePolygonShape(bottom_rectangle, QPoint());
  bottom_sensor_ = CreateFixture(bottom_shape);
  bottom_sensor_->SetSensor(true);

  QPolygon side_rectangle = QRect(rectangle.left() - 1,
                                  rectangle.top() + 1,
                                  2,
                                  rectangle.height() - 2);
  b2PolygonShape left_shape = CreatePolygonShape(side_rectangle, QPoint());
  left_sensor_ = CreateFixture(left_shape);
  left_sensor_->SetSensor(true);

  side_rectangle = QRect(rectangle.right() - 1,
                         rectangle.top() + 1,
                         2,
                         rectangle.height() - 2);
  b2PolygonShape right_shape = CreatePolygonShape(side_rectangle, QPoint());
  right_sensor_ = CreateFixture(right_shape);
  right_sensor_->SetSensor(true);

  SetNoCollisionMask(static_cast<uint16_t>(EntityType::kPlayer));

  player_jump_audio_key_ = map_.lock()->GetAudioManager()->
      CreateAudioPlayer(AudioName::kPlayerJump);
  player_separation_audio_key_ = map_.lock()->GetAudioManager()->
      CreateAudioPlayer(AudioName::kPlayerSeparation);
  player_receive_damage_audio_key_ = map_.lock()->GetAudioManager()->
      CreateAudioPlayer(AudioName::kPlayerTakingDamage);
}

void Player::Update(int time) {
  Entity::Update(time);
  if (current_health_ == 0) {
    return;
  }

  if (no_damage_time_left_ > 0) {
    no_damage_time_left_ -= time;
  }

  if (monsters_count_ != 0) {
    TakeDamage();
  }

  if (map_.lock()->IsKeyPressed(Key::kSpace) && player_part_ == nullptr
      && level_number_ > 2) {
    player_part_ = std::make_shared<Entity>(map_,
                                            b2_dynamicBody,
                                            GetPositionInPixels(),
                                            QPolygon(bounding_rectangle_),
                                            EntityType::kPlayer);
    b2Vec2 clone_velocity(body_->GetLinearVelocity().x, -kPlayerJumpSpeed);
    clone_velocity.Normalize();
    clone_velocity *= kCloneSpeed;
    player_part_->SetVelocity(clone_velocity, true);
    player_part_->SetAnimator(std::make_shared<Animator>(*animator_));
    map_.lock()->AddGameObject(player_part_);

    map_.lock()->GetAudioManager()->
        PlayAudioPlayer(player_separation_audio_key_);
  }

  if (player_part_ != nullptr
      && !player_part_->GetBoundingRectangle().intersects(
          GetBoundingRectangle())) {
    player_part_->SetEntityType(EntityType::kPlayerPart);
  }

  if (map_.lock()->IsKeyPressed(Key::kUp) && jumps_remaining_ > 0) {
    animator_->SetCurrentAnimation(kJump + "_" + animation_name_, false);
    jumps_remaining_--;
    body_->SetLinearVelocity(b2Vec2(body_->GetLinearVelocity().x, 0));
    body_->ApplyLinearImpulseToCenter(
        b2Vec2(0, -kPlayerJumpSpeed * body_->GetMass()), true);

    map_.lock()->GetAudioManager()->ReplayAudioPlayer(player_jump_audio_key_);
  }
  float target_speed = -body_->GetLinearVelocity().x;
  if (map_.lock()->IsKeyClamped(Key::kLeft) && left_collisions_ == 0) {
    target_speed -= kPlayerSpeed;
  } else if (map_.lock()->IsKeyClamped(Key::kRight) && right_collisions_ == 0) {
    target_speed += kPlayerSpeed;
  }
  body_->ApplyLinearImpulseToCenter(b2Vec2(target_speed * body_->GetMass(), 0),
                                    true);
}

void Player::BeginCollision(b2Fixture* fixture,
                            EntityType,
                            EntityType other_type) {
  if (player_part_ != nullptr && other_type == EntityType::kPlayerPart) {
    player_part_->MarkAsDeleted();
    player_part_ = nullptr;
  }
  if (other_type == EntityType::kExit) {
    reached_exit_ = true;
  }

  if (fixture == bottom_sensor_) {
    jumps_remaining_ = jump_count_;
  } else if (fixture == left_sensor_) {
    left_collisions_++;
  } else if (fixture == right_sensor_) {
    right_collisions_++;
  } else if (other_type == EntityType::kBullet
      || other_type == EntityType::kChestnut
      || other_type == EntityType::kSpikes
      || other_type == EntityType::kBurdock) {
    monsters_count_++;
  }
}

void Player::EndCollision(b2Fixture* my_fixture, EntityType other_type) {
  if (my_fixture == bottom_sensor_) {
    return;
  }
  if (my_fixture == left_sensor_) {
    left_collisions_--;
  } else if (my_fixture == right_sensor_) {
    right_collisions_--;
  } else if (other_type == EntityType::kChestnut
      || other_type == EntityType::kBurdock
      || other_type == EntityType::kBullet
      || other_type == EntityType::kSpikes) {
    monsters_count_--;
  }
}

int Player::GetCurrentHealth() const {
  return current_health_;
}

bool Player::ReachedExit() const {
  return reached_exit_;
}

void Player::TakeDamage() {
  if (no_damage_time_left_ > 0) {
    return;
  }
  animator_->SetCurrentAnimation(kDamage + "_" + animation_name_);
  no_damage_time_left_ = kNoDamageTime;
  current_health_--;

  map_.lock()->GetAudioManager()->
      PlayAudioPlayer(player_receive_damage_audio_key_);
}

void Player::SetAnimationName(const QString& animation_name) {
  animation_name_ = animation_name;
  animator_->SetCurrentAnimation(animation_name_);
}

void Player::SetCurrentLevel(int level_number) {
  level_number_ = level_number;
  if (level_number_ > 2) {
    jump_count_ = 2;
  }
}

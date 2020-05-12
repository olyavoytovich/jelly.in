#include "player.h"

Player::Player(std::shared_ptr<Map> map,
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
}

void Player::Update(int time) {
  Entity::Update(time);
  if (current_health_ == 0) {
    return;
  }

  if (no_damage_time_left_ > 0) {
    no_damage_time_left_ -= time;
  }

  if (map_->IsKeyPressed(Key::kSpace) && player_part_ == nullptr) {
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
    map_->AddGameObject(player_part_);
  }

  if (player_part_ != nullptr
      && !player_part_->GetBoundings().intersects(GetBoundings())) {
    player_part_->SetEntityType(EntityType::kPlayerPart);
  }

  if (map_->IsKeyPressed(Key::kUp) && jumps_remaining_ > 0) {
    jumps_remaining_--;
    body_->SetLinearVelocity(b2Vec2(body_->GetLinearVelocity().x, 0));
    body_->ApplyLinearImpulseToCenter(
        b2Vec2(0, -kPlayerJumpSpeed * body_->GetMass()), true);
  }
  float target_speed = -body_->GetLinearVelocity().x;
  if (map_->IsKeyClamped(Key::kLeft) && left_collisions_ == 0) {
    target_speed -= kPlayerSpeed;
  } else if (map_->IsKeyClamped(Key::kRight) && right_collisions_ == 0) {
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
    jumps_remaining_ = kPlayerJumpCount;
  } else if (fixture == left_sensor_) {
    left_collisions_++;
  } else if (fixture == right_sensor_) {
    right_collisions_++;
  } else if (other_type == EntityType::kBullet
      || other_type == EntityType::kChestnut
      || other_type == EntityType::kSpikes
      || other_type == EntityType::kBurdock) {
    TakeDamage();
    animator_->SetCurrentAnimation("damage");
    animator_->Play();
  }
}

void Player::EndCollision(b2Fixture* my_fixture) {
  if (my_fixture == left_sensor_) {
    left_collisions_--;
  } else if (my_fixture == right_sensor_) {
    right_collisions_--;
  }
  animator_->SetCurrentAnimation("jump");
  animator_->Play();
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
  animator_->Play();
  no_damage_time_left_ = kNoDamageTime;
  current_health_--;
}

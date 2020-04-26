#include "player.h"

Player::Player(std::shared_ptr<Map> map,
               const QPoint& body_position,
               const QRect& rectangle)
    : Entity(std::move(map), b2_dynamicBody, body_position, rectangle,
             EntityType::kPlayer) {
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
}

void Player::Update(int) {
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

void Player::BeginCollision(b2Fixture* fixture, EntityType,
                            EntityType other_type) {
  if (fixture == bottom_sensor_) {
    jumps_remaining_ = kPlayerJumpCount;
  } else if (fixture == left_sensor_) {
    left_collisions_++;
  } else if (fixture == right_sensor_) {
    right_collisions_++;
  } else if (other_type == EntityType::kBullet) {
    MarkAsDeleted();
  }
}

void Player::EndCollision(b2Fixture* my_fixture) {
  if (my_fixture == left_sensor_) {
    left_collisions_--;
  } else if (my_fixture == right_sensor_) {
    right_collisions_--;
  }
}
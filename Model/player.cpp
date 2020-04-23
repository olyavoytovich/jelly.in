#include "player.h"

Player::Player(std::shared_ptr<Map> map,
               const QPoint& body_position,
               const QPolygon& polygon)
    : Entity(std::move(map), b2_dynamicBody, body_position, polygon) {}

void Player::Update(int) {
  if (map_->IsKeyPressed(Key::kUp)) {
    body_->SetLinearVelocity(b2Vec2(body_->GetLinearVelocity().x, 0));
    body_->ApplyLinearImpulseToCenter(
        b2Vec2(0, -kPlayerJumpSpeed * body_->GetMass()), true);
  }
  float target_speed = -body_->GetLinearVelocity().x;
  if (map_->IsKeyClamped(Key::kLeft)) {
    target_speed -= kPlayerSpeed;
  } else if (map_->IsKeyClamped(Key::kRight)) {
    target_speed += kPlayerSpeed;
  }
  body_->ApplyLinearImpulseToCenter(b2Vec2(target_speed * body_->GetMass(), 0),
                                    true);
}

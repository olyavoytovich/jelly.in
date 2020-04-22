#include "player.h"

Player::Player(std::shared_ptr<Map> map,
               const QPoint& body_position,
               const QPolygon& polygon,
               AbstractGameController* game_controller)
    : Entity(std::move(map), b2_dynamicBody, body_position, polygon),
      game_controller_(game_controller) {}

void Player::Update(int) {
  if (game_controller_->GetPressedKeyStatus(Key::UP)) {
    body_->SetLinearVelocity(b2Vec2(body_->GetLinearVelocity().x, 0));
    body_->ApplyLinearImpulseToCenter(
        b2Vec2(0, -kPlayerJumpSpeed * body_->GetMass()), true);
  }
  float target_speed = -body_->GetLinearVelocity().x;
  if (game_controller_->GetClampedKeyStatus(Key::LEFT)) {
    target_speed -= kPlayerSpeed;
  }
  if (game_controller_->GetClampedKeyStatus(Key::RIGHT)) {
    target_speed += kPlayerSpeed;
  }
  body_->ApplyLinearImpulseToCenter(b2Vec2(target_speed * body_->GetMass(), 0),
                                    true);
}

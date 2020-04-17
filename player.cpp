#include "player.h"

Player::Player(std::shared_ptr<b2World> world,
               b2BodyType type,
               const Point& body_position,
               const QPolygon& polygon,
               AbstractGameController* game_controller)
    : Entity(world, type, body_position, polygon),
      game_controller_(game_controller) {}

void Player::Update() {
  if (game_controller_->GetPressedKeyStatus(Key::UP) &&
      abs(body_->GetLinearVelocity().y) < kEps) {
    body_->ApplyLinearImpulseToCenter(b2Vec2(0, kJumpSpeed * body_->GetMass()),
                                      true);
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

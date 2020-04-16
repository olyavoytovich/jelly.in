#include "player.h"

Player::Player(std::shared_ptr<b2World> world,
               b2BodyType type,
               const Point& body_position,
               const QPolygon& polygon,
               AbstractGameController* game_controller)
    : Entity(world, type, body_position, polygon),
      game_controller_(game_controller) {}

void Player::Update() {
  b2Vec2 velocity = body_->GetLinearVelocity();
  if (game_controller_->GetKey(Keys::UP) == true) {
    velocity.y = 1000;
  }
  if (game_controller_->GetKey(Keys::LEFT) == true) {
    velocity.x = -300;
  } else if (game_controller_->GetKey(Keys::RIGHT) == true) {
    velocity.x = 300;
  } else {
    velocity.x = 0;
  }
  SetLinearVelocity(velocity);
}

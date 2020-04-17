#ifndef PLAYER_H_
#define PLAYER_H_

#include "abstract_game_controller.h"
#include "entity.h"
#include <memory>

class Player : public Entity {
 public:
  Player(std::shared_ptr<b2World> world,
         b2BodyType type,
         const Point& body_position,
         const QPolygon& polygon,
         AbstractGameController* game_controller);
  ~Player() = default;

  void Update();

 private:
  const int kPlayerSpeed = 3;
  const int kJumpSpeed = 4;

 private:
  AbstractGameController* game_controller_ = nullptr;
};

#endif  // PLAYER_H_

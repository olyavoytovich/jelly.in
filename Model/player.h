#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <memory>
#include <utility>

#include "Controller/abstract_game_controller.h"
#include "entity.h"

class Player : public Entity {
 public:
  Player(std::shared_ptr<Map> map,
         b2BodyType type,
         const QPoint& body_position,
         const QPolygon& polygon,
         AbstractGameController* game_controller);
  ~Player() override = default;

  void Update(int time) override;

 private:
  const float kPlayerSpeed = 3;
  const float kJumpSpeed = 4;

 private:
  AbstractGameController* game_controller_ = nullptr;
};

#endif  // MODEL_PLAYER_H_

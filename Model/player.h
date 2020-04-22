#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <memory>
#include <utility>

#include "Controller/abstract_game_controller.h"
#include "entity.h"

class Player : public Entity {
 public:
  Player(std::shared_ptr<Map> map,
         const QPoint& body_position,
         const QPolygon& polygon,
         AbstractGameController* game_controller);
  ~Player() override = default;

  void Update(int time) override;

 public:
  static const int kPlayerWidth = 20;
  static const int kPlayerHeight = 30;

 private:
  const float kPlayerSpeed = 3;
  const float kPlayerJumpSpeed = 4;

 private:
  AbstractGameController* game_controller_ = nullptr;
};

#endif  // MODEL_PLAYER_H_

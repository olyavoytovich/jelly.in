#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <memory>
#include <utility>

#include "entity.h"

class Player : public Entity {
 public:
  Player(std::shared_ptr<Map> map,
         const QPoint& body_position,
         const QPolygon& polygon);
  ~Player() override = default;

  void Update(int time) override;

 public:
  static const int kPlayerWidth = 20;
  static const int kPlayerHeight = 30;

 private:
  const float kPlayerSpeed = 3;
  const float kPlayerJumpSpeed = 4;
};

#endif  // MODEL_PLAYER_H_

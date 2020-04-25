#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <memory>
#include <utility>

#include "entity.h"

class Player : public Entity {
 public:
  Player(std::shared_ptr<Map> map,
         const QPoint& body_position,
         const QRect& rectangle);
  ~Player() override = default;

  void Update(int time) override;
  void BeginCollision(EntityType my_type, b2Fixture* fixture,
                      EntityType other_type) override;
  void EndCollision(EntityType my_type, b2Fixture* my_fixture,
                    EntityType other_type) override;

 public:
  static const int kPlayerWidth = 20;
  static const int kPlayerHeight = 30;

 private:
  const float kPlayerSpeed = 3;
  const float kPlayerJumpSpeed = 4;
  const int kPlayerJumpCount = 2;

 private:
  int jumps_left_ = 0;
  int left_collisions = 0;
  int right_collisions = 0;

  b2Fixture* bottom_sensor_ = nullptr;
  b2Fixture* left_sensor_ = nullptr;
  b2Fixture* right_sensor_ = nullptr;
};

#endif  // MODEL_PLAYER_H_

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
  void BeginCollision(b2Fixture* fixture,
                      EntityType my_type,
                      EntityType other_type) override;
  void EndCollision(b2Fixture* my_fixture) override;

 public:
  static const int kPlayerWidth = 20;
  static const int kPlayerHeight = 30;

 private:
  void TakeDamage();

 private:
  const float kPlayerSpeed = 3;
  const float kPlayerJumpSpeed = 4;
  const float kCloneSpeed = 5;
  const int kPlayerJumpCount = 2;
  const int kNoDamageTime = 1000;
  const int kMaxHealth = 3;

 private:
  int current_health_ = kMaxHealth;
  int no_damage_time_left_ = 0;

  int jumps_remaining_ = 0;
  int left_collisions_ = 0;
  int right_collisions_ = 0;

  b2Fixture* bottom_sensor_ = nullptr;
  b2Fixture* left_sensor_ = nullptr;
  b2Fixture* right_sensor_ = nullptr;

  std::shared_ptr<Entity> player_part_ = nullptr;
};

#endif  // MODEL_PLAYER_H_

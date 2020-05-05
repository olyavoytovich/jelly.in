#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <memory>
#include <utility>

#include "animator.h"
#include "entity.h"

class Player : public Entity {
 public:
  Player(std::shared_ptr<Map> map,
         const QPoint& body_position,
         const QRect& rectangle,
         std::shared_ptr<Animator> animator);

  ~Player() override = default;

  void Update(int time) override;
  void BeginCollision(b2Fixture* fixture,
                      EntityType my_type,
                      EntityType other_type) override;
  void EndCollision(b2Fixture* my_fixture) override;

  int GetCurrentHealth() const;
  bool ReachedExit() const;

 public:
  static const int kPlayerWidth = 64;
  static const int kPlayerHeight = 64;

 private:
  void TakeDamage();

 private:
  const float kPlayerSpeed = 6;
  const float kPlayerJumpSpeed = 8;
  const float kCloneSpeed = 10;
  const int kPlayerJumpCount = 2;
  const int kNoDamageTime = 1000;
  const int kMaxHealth = 3;

 private:
  bool reached_exit_ = false;

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

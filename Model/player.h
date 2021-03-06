#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <memory>
#include <utility>

#include "animator.h"
#include "constants.h"
#include "entity.h"

class Player : public Entity {
 public:
  Player(std::weak_ptr<Map> map,
         const QPoint& body_position,
         const QRect& rectangle,
         std::shared_ptr<Animator> animator);

  ~Player() override = default;

  void Update(int time) override;
  void BeginCollision(b2Fixture* fixture,
                      EntityType,
                      EntityType other_type) override;
  void EndCollision(b2Fixture* my_fixture, EntityType other_type) override;

  int GetCurrentHealth() const;
  bool ReachedExit() const;

  void SetAnimationName(const QString& animation_name);
  void SetCurrentLevel(int level_number);

 public:
  static const int kPlayerWidth = 64;
  static const int kPlayerHeight = 64;

 private:
  void TakeDamage();

 private:
  const float kPlayerSpeed = 6;
  const float kPlayerJumpSpeed = 8;
  const float kCloneSpeed = 10;
  const int kNoDamageTime = 1000;
  const int kMaxHealth = 3;

 private:
  bool reached_exit_ = false;

  int current_health_ = kMaxHealth;
  int no_damage_time_left_ = 0;

  int jumps_remaining_ = 0;
  int left_collisions_ = 0;
  int right_collisions_ = 0;
  int monsters_count_ = 0;
  int jump_count_ = 1;

  int level_number_;

  QString animation_name_;

  b2Fixture* bottom_sensor_ = nullptr;
  b2Fixture* left_sensor_ = nullptr;
  b2Fixture* right_sensor_ = nullptr;

  std::shared_ptr<Entity> player_part_ = nullptr;

  int player_jump_audio_key_;
  int player_separation_audio_key_;
  int player_receive_damage_audio_key_;
};

#endif  // MODEL_PLAYER_H_

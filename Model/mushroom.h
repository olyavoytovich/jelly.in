#ifndef MODEL_MUSHROOM_H_
#define MODEL_MUSHROOM_H_

#include <memory>
#include <utility>

#include "animator.h"
#include "entity.h"

class Mushroom : public Entity {
 public:
  Mushroom(std::weak_ptr<Map> map,
           b2BodyType type,
           const QPoint& body_position,
           const QPolygon& polygon);

  ~Mushroom() override = default;

  void BeginCollision(b2Fixture* my_fixture,
                      EntityType my_type,
                      EntityType other_type) override;

  bool IsPicked() const override;

 private:
  bool is_picked_;
  int player_get_mushroom_audio_key_;
};

#endif  // MODEL_MUSHROOM_H_

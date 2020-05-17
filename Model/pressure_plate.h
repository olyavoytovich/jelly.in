#ifndef MODEL_PRESSURE_PLATE_H_
#define MODEL_PRESSURE_PLATE_H_

#include <memory>
#include <utility>
#include <vector>

#include "animator.h"
#include "constants.h"
#include "entity.h"

class PressurePlate : public Entity {
 public:
  PressurePlate(std::weak_ptr<Map> map,
                b2BodyType type,
                const QPoint& body_position,
                const QPolygon& polygon,
                std::shared_ptr<Animator> animator);

  void AddPlatform(const std::shared_ptr<Entity>& platform);

  void ActivatePlatforms();
  void StopPlatforms();

  void BeginCollision(b2Fixture*, EntityType, EntityType other_type) override;
  void EndCollision(b2Fixture*, EntityType other_type) override;

 private:
  std::vector<std::shared_ptr<Entity>> platforms_;

  int collisions_count_ = 0;
};

#endif  // MODEL_PRESSURE_PLATE_H_

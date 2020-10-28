#ifndef MODEL_PATROLLER_H_
#define MODEL_PATROLLER_H_

#include <memory>
#include <vector>
#include <utility>

#include "animator.h"
#include "entity.h"

class Patroller : public Entity {
 public:
  Patroller(std::weak_ptr<Map> map,
            b2BodyType type,
            const QPoint& body_position,
            int radius,
            const std::vector<QPoint>& way_points,
            std::shared_ptr<Animator> animator,
            int speed);

  void InitializeSound(std::shared_ptr<Sound> patroller_sound);

  ~Patroller() override = default;
};

#endif  // MODEL_PATROLLER_H_

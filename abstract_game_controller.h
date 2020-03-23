#ifndef ABSTRACT_GAME_CONTROLLER_H_
#define ABSTRACT_GAME_CONTROLLER_H_

#include <memory>
#include <QPainter>

#include "game_object.h"

class AbstractGameController {
 public:
  virtual ~AbstractGameController() = default;

  virtual void Update() = 0;
  virtual void Draw(QPainter* painter) const = 0;
};

#endif  // ABSTRACT_GAME_CONTROLLER_H_

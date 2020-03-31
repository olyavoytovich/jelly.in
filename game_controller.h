#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include <memory>
#include <utility>
#include <vector>

#include "abstract_game_controller.h"
#include "abstract_view.h"
#include "box2d/box2d.h"
#include "entity.h"
#include "game_object.h"
#include "view.h"

class GameController : public AbstractGameController {
 public:
  GameController();
  ~GameController() override;

  void Update(int time) override;
  void Draw(QPainter* painter) const override;

 private:
  std::shared_ptr<View> view_;
  b2World* world_;
  Entity* entity_;
  Entity* entity2_;
  Entity* entity3_;
};

#endif  // GAME_CONTROLLER_H_

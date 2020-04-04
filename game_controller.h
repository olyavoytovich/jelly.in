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
#include "map.h"
#include "map_loader.h"
#include "view.h"

class GameController : public AbstractGameController {
 public:
  GameController();
  ~GameController() override = default;

  void Update(int time) override;
  void Draw(QPainter* painter) const override;

 private:
  const int kVelocityIterations = 6;
  const int kPositionIterations = 2;

 private:
  std::shared_ptr<View> view_;

  std::shared_ptr<b2World> world_;

  std::shared_ptr<Entity> entity_;
  std::shared_ptr<Entity> entity2_;
  std::shared_ptr<Entity> entity3_;

  std::shared_ptr<Map> map_;
};

#endif  // GAME_CONTROLLER_H_

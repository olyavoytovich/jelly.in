#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include <memory>

#include "abstract_game_controller.h"
#include "abstract_view.h"
#include "game_object.h"
#include "map.h"
#include "map_loader.h"
#include "view.h"

class GameController : public AbstractGameController {
 public:
  GameController();
  ~GameController() override = default;

  void Update() override;
  void Draw(QPainter* painter) const override;

 private:
  std::shared_ptr<View> view_;
  std::shared_ptr<Map> map_;
};

#endif  // GAME_CONTROLLER_H_

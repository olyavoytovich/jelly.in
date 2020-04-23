#ifndef CONTROLLER_GAME_CONTROLLER_H_
#define CONTROLLER_GAME_CONTROLLER_H_

#include <memory>
#include <utility>
#include <vector>

#include "Model/entity.h"
#include "Model/game_object.h"
#include "Model/map.h"
#include "Model/map_loader.h"
#include "Model/patroller.h"
#include "Model/shooter.h"
#include "View/abstract_view.h"
#include "View/view.h"
#include "abstract_game_controller.h"

class GameController : public AbstractGameController {
 public:
  GameController();
  ~GameController() override = default;

  void Update(int time) override;
  void Draw(QPainter* painter) const override;
  void PressKey(int key_code);
  void ClampKey(int key_code);
  void ReleaseKey(int key_code);
  void ReleaseKey(Key key);

 private:
  std::shared_ptr<View> view_;

  std::shared_ptr<Map> map_;
};

#endif  // CONTROLLER_GAME_CONTROLLER_H_

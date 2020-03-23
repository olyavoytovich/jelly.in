#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include "abstract_game_controller.h"
#include "abstract_view.h"
#include "view.h"

class GameController : public AbstractGameController {
 public:
  GameController();
  ~GameController() override = default;

  void Update() override;
  void Draw(QPainter* painter) const override;

 private:
  std::shared_ptr<View> view_;
  std::shared_ptr<GameObject> game_object_;
};

#endif  // GAME_CONTROLLER_H_

#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include "game_object.h"
#include "view.h"

class GameController {
 public:
  GameController();
  ~GameController();

  GameObject* GetGameObject() const;
  void Update();

 private:
  View* view_;
  GameObject* game_object_;
};

#endif  // GAME_CONTROLLER_H_

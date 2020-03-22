#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

class View;
class GameObject;

class GameController {
 public:
  GameController();
  ~GameController();

  GameObject* GetGameObject();
  void Update();

 private:
  View* view_;
  GameObject* game_object_;
};

#endif  // GAME_CONTROLLER_H_

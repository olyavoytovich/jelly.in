#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"
#include "abstract_game_controller.h"

class Player : public Entity {
  public:
    Player(std::shared_ptr<b2World> world,
           b2BodyType type,
           const Point& body_position,
           const QPolygon& polygon,
          AbstractGameController* game_controller);
    ~Player() = default;

    void Update();

  private:
    AbstractGameController* game_controller_ = nullptr;
};

#endif  // PLAYER_H_

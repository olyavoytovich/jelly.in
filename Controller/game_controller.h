#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

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
#include "box2d/box2d.h"

class GameController : public AbstractGameController {
 public:
  GameController();
  ~GameController() override = default;

  void Update(int time) override;
  void Draw(QPainter* painter) const override;

 private:
  // Данные константы передаются в функцию Step(), которая используется при
  // реализации метода Update(int time).
  // kVelocityAccuracy и kPositionAccuracy отвечают за точность вычисления
  // импульсов, необходимых для правильного движения и положения тел.
  // Предлагаемое количество итераций для Box2D равно 6 и 2 соответственно.
  // Использование меньшего количества повышают производительность, но страдает
  // точность. Аналогично, использование большего количества итераций снижает
  // производительность, но улучшает качество симуляции.
  const int kVelocityAccuracy = 6;
  const int kPositionAccuracy = 2;

 private:
  std::shared_ptr<View> view_;

  std::shared_ptr<b2World> world_;

  std::shared_ptr<Entity> platform1_;
  std::shared_ptr<Entity> platform2_;
  std::shared_ptr<Entity> platform3_;

  std::shared_ptr<Patroller> patroller_;
  std::shared_ptr<Shooter> shooter1_;
  std::shared_ptr<Shooter> shooter2_;
  std::shared_ptr<Shooter> shooter3_;
  std::shared_ptr<Shooter> shooter4_;

  std::shared_ptr<Map> map_;
};

#endif  // GAME_CONTROLLER_H_

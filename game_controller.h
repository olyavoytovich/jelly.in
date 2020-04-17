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
#include "player.h"
#include "view.h"

class GameController : public AbstractGameController {
 public:
  GameController();
  ~GameController() override = default;

  void Update(int time) override;
  void Draw(QPainter* painter) const override;
  void PressKey(int event) override;
  void ClampKey(int event) override;
  void ReleaseKey(int event) override;
  bool GetPressedKeyStatus(Key key) override;
  bool GetClampedKeyStatus(Key key) override;

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

  std::vector<bool> is_key_pressed_;
  std::vector<bool> is_key_clamped_;

 private:
  std::shared_ptr<View> view_;

  std::shared_ptr<b2World> world_;

  std::shared_ptr<Entity> entity_;
  std::shared_ptr<Player> player_;

  std::shared_ptr<Map> map_;
};

#endif  // GAME_CONTROLLER_H_

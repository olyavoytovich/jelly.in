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
#include "View/choose_level_menu.h"
#include "View/main_menu.h"
#include "View/menu.h"
#include "View/view.h"
#include "abstract_game_controller.h"

class GameController : public AbstractGameController {
 public:
  GameController();
  ~GameController() override = default;

  void Update(int time) override;
  void Draw(QPainter* painter) const override;
  void PressKey(int key_code) override;
  void ClampKey(int key_code) override;
  void ReleaseKey(int key_code) override;

  void OpenChooseLevelMenu() override;
  void CloseChooseLevelMenu() override;

 private:
  Key GetKeyFromCode(int key_code);

 private:
  std::shared_ptr<View> view_;

  std::shared_ptr<MainMenu> main_menu_;

  std::shared_ptr<ChooseLevelMenu> choose_level_menu_;

  std::shared_ptr<Map> map_;
};

#endif  // CONTROLLER_GAME_CONTROLLER_H_

#ifndef CONTROLLER_GAME_CONTROLLER_H_
#define CONTROLLER_GAME_CONTROLLER_H_

#include <memory>
#include <utility>
#include <vector>

#include "Model/audio_manager.h"
#include "Model/entity.h"
#include "Model/game_object.h"
#include "Model/map.h"
#include "Model/map_loader.h"
#include "Model/patroller.h"
#include "Model/shooter.h"
#include "View/abstract_view.h"
#include "View/choose_level_menu.h"
#include "View/game_interface.h"
#include "View/intermediate_menu.h"
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
  void OpenMainMenu() override;
  void OpenPauseMenu() override;
  void OpenVictoryMenu() override;
  void OpenFailMenu() override;
  void ResumeGame() override;
  void RestartGame() override;
  void StartNextLevel() override;

  void StartLevel(int level_number) override;
  void OpenMenu(std::shared_ptr<Menu> menu);

 private:
  Key GetKeyFromCode(int key_code);
  void CloseCurrentLevel();

 private:
  int level_number_ = 0;

  std::shared_ptr<View> view_;

  // Указатель на открытое в данный момент меню
  std::shared_ptr<Menu> menu_ = nullptr;
  std::shared_ptr<GameInterface> interface_ = nullptr;

  std::shared_ptr<Map> map_ = nullptr;
  std::shared_ptr<Player> player_ = nullptr;

  std::shared_ptr<AudioManager> audio_manager_;
  int level_audio_key_;
};

#endif  // CONTROLLER_GAME_CONTROLLER_H_

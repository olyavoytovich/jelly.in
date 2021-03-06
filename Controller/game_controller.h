#ifndef CONTROLLER_GAME_CONTROLLER_H_
#define CONTROLLER_GAME_CONTROLLER_H_

#include <QSettings>
#include <algorithm>
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
#include "View/settings_menu.h"
#include "View/settings_volume.h"
#include "View/view.h"
#include "abstract_game_controller.h"

class GameController : public AbstractGameController {
 public:
  GameController();
  ~GameController() override = default;

  void Update(int time) override;
  void PressKey(int key_code) override;
  void ClampKey(int key_code) override;
  void ReleaseKey(int key_code) override;

  void OpenChooseLevelMenu() override;
  void OpenMainMenu() override;
  void OpenPauseMenu() override;
  void OpenVictoryMenu() override;
  void OpenFailMenu() override;
  void OpenSettingsMenu() override;
  void OpenSettingsVolume() override;
  void OpenSettingsControls() override;
  void ResumeGame() override;
  void RestartGame() override;
  void StartNextLevel() override;

  void Reset() override;

  QString GetPlayerAnimation() const override;
  void SetPlayerAnimation(const QString& animation_name) override;

  int GetLastLevelMushrooms() const override;
  int GetLevelMushrooms(int level_number) const override;

  void LoadVolume() override;

  int GetVolume(Volume volume) override;
  void SetVolume(Volume volume, int power) override;

  void StartLevel(int level_number) override;
  void OpenMenu(std::shared_ptr<Menu> menu);

  std::shared_ptr<Map> GetMap() override;

 private:
  Key GetKeyFromCode(int key_code);
  void CloseCurrentLevel();

 private:
  int level_number_ = 0;
  int last_level_mushrooms_ = 0;
  std::vector<int> level_mushrooms_;

  QString player_animation_name_ = "";

  std::shared_ptr<View> view_;

  // Указатель на открытое в данный момент меню
  std::shared_ptr<Menu> menu_ = nullptr;
  std::shared_ptr<GameInterface> interface_ = nullptr;

  std::shared_ptr<Map> map_ = nullptr;
  std::shared_ptr<Player> player_ = nullptr;

  std::shared_ptr<AudioManager> audio_manager_;
  int level_audio_key_;
  int menu_audio_key_;

  int general_volume_ = 40;
  int music_volume_ = 40;
  int sound_volume_ = 40;

  QSettings settings_;
};

#endif  // CONTROLLER_GAME_CONTROLLER_H_

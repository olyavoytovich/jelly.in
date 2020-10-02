#include "game_controller.h"

GameController::GameController()
    : level_mushrooms_(12, -1),
      view_(std::make_shared<View>(this)),
      menu_(std::make_shared<MainMenu>(this)),
      audio_manager_(std::make_shared<AudioManager>()) {
  view_->show();
  view_->setCentralWidget(menu_.get());
  view_->setWindowIcon(QIcon(":/images/menu/icon.png"));
  player_animation_name_ = settings_.value("animation_name").toString();

  level_mushrooms_[0] = settings_.value("level_1", 0).toInt();
  for (int i = 1; i <= 11; i++) {
    level_mushrooms_[i] =
        settings_.value("level_" + QString::number(i + 1), -1).toInt();
  }

  LoadVolume();

  level_audio_key_ = audio_manager_->
      CreateAudioPlayerByPlayList(AudioName::kBackground);
  audio_manager_->SetPlayBackMode(level_audio_key_, QMediaPlaylist::Loop);
  menu_audio_key_ = audio_manager_->
      CreateAudioPlayerByPlayList(AudioName::kMenuAudio);
  audio_manager_->SetPlayBackMode(menu_audio_key_, QMediaPlaylist::Loop);
  audio_manager_->PlayAudioPlayer(menu_audio_key_);
}

void GameController::Update(int time) {
  if (view_->centralWidget() != interface_.get()) {
    return;
  }
  view_->repaint();
  map_->Update(time);
  if (player_->ReachedExit()) {
    OpenVictoryMenu();
  }
  if (interface_ != nullptr) {
    last_level_mushrooms_ = map_->GetPickedMushroomsCount();
    interface_->SetMushrooms(last_level_mushrooms_);
    interface_->SetHealth(player_->GetCurrentHealth());
  }
}

void GameController::PressKey(int key_code) {
  if (key_code == Qt::Key_Escape) {
    if (view_->centralWidget() == interface_.get()) {
      OpenPauseMenu();
    }
  }

  if (map_ == nullptr) {
    return;
  }
  map_->SetPressedKeyStatus(GetKeyFromCode(key_code), true);
}

void GameController::ClampKey(int key_code) {
  if (map_ == nullptr) {
    return;
  }
  map_->SetClampedKeyStatus(GetKeyFromCode(key_code), true);
}

void GameController::ReleaseKey(int key_code) {
  if (map_ == nullptr) {
    return;
  }
  Key key = GetKeyFromCode(key_code);
  map_->SetPressedKeyStatus(key, false);
  map_->SetClampedKeyStatus(key, false);
}

Key GameController::GetKeyFromCode(int key_code) {
  if (key_code == Qt::Key_Left || key_code == Qt::Key_A || key_code == 1060) {
    return Key::kLeft;
  }
  if (key_code == Qt::Key_Right || key_code == Qt::Key_D || key_code == 1042) {
    return Key::kRight;
  }
  if (key_code == Qt::Key_Up || key_code == Qt::Key_W || key_code == 1062) {
    return Key::kUp;
  }
  if (key_code == Qt::Key_Space) {
    return Key::kSpace;
  }
  return Key::kAnyKey;
}

void GameController::CloseCurrentLevel() {
  interface_ = nullptr;
  map_ = nullptr;
  player_ = nullptr;
}

void GameController::OpenChooseLevelMenu() {
  OpenMenu(std::make_shared<ChooseLevelMenu>(this));
  audio_manager_->PlayAudioPlayer(menu_audio_key_);
  CloseCurrentLevel();
}

void GameController::OpenMainMenu() {
  OpenMenu(std::make_shared<MainMenu>(this));
  audio_manager_->PlayAudioPlayer(menu_audio_key_);
  CloseCurrentLevel();
}

void GameController::OpenPauseMenu() {
  audio_manager_->PauseAudioPlayer(level_audio_key_);
  audio_manager_->PlayAudioPlayer(menu_audio_key_);
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kPause));
}

void GameController::OpenVictoryMenu() {
  if (map_ != nullptr) {
    level_mushrooms_[level_number_] = std::max(level_mushrooms_[level_number_],
                                               map_->GetPickedMushroomsCount());
    // Если собрали не меньше 2х грибов, то открываем следующий уровень
    if (level_number_ + 1 != level_mushrooms_.size()
        && level_mushrooms_[level_number_] >= 2
        && level_mushrooms_[level_number_ + 1] == -1) {
      level_mushrooms_[level_number_ + 1] = 0;
      settings_.setValue("level_" + QString::number(level_number_ + 2), 0);
    }
  }
  settings_.setValue("level_" + QString::number(level_number_ + 1),
                     level_mushrooms_[level_number_]);

  audio_manager_->StopAudioPlayer(level_audio_key_);
  audio_manager_->PlayAudioPlayer(menu_audio_key_);
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kVictory));
  CloseCurrentLevel();
}

void GameController::OpenFailMenu() {
  audio_manager_->StopAudioPlayer(level_audio_key_);
  audio_manager_->PlayAudioPlayer(menu_audio_key_);
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kFail));
  CloseCurrentLevel();
}

void GameController::OpenSettingsControls() {
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kControls));
  CloseCurrentLevel();
}

void GameController::OpenSettingsMenu() {
  OpenMenu(std::make_shared<SettingsMenu>(this));
  CloseCurrentLevel();
}

void GameController::OpenSettingsVolume() {
  OpenMenu(std::make_shared<SettingsVolume>(this));
  CloseCurrentLevel();
}

void GameController::ResumeGame() {
  view_->takeCentralWidget();
  view_->setCentralWidget(interface_.get());
  audio_manager_->StopAudioPlayer(menu_audio_key_);
  audio_manager_->PlayAudioPlayer(level_audio_key_);
}

void GameController::RestartGame() {
  StartLevel(level_number_);
}

void GameController::StartNextLevel() {
  StartLevel(level_number_ + 1);
}

void GameController::Reset() {
  std::fill(level_mushrooms_.begin(), level_mushrooms_.end(), -1);
  level_mushrooms_[0] = 0;
  settings_.setValue("level_1", 0);
  for (int i = 1; i <= 11; i++) {
    settings_.setValue("level_" + QString::number(i + 1), -1);
  }
}

void GameController::OpenMenu(std::shared_ptr<Menu> menu) {
  menu_ = std::move(menu);
  if (menu_ != nullptr) {
    menu_->SetGeneralVolume(general_volume_);
    menu_->SetCurrentVolume(sound_volume_);
    view_->takeCentralWidget();
    view_->setCentralWidget(menu_.get());
  }
}

void GameController::StartLevel(int level_number) {
  if (level_mushrooms_[level_number] == -1) {
    // Уровень заблокирован, нельзя его загружать. Уходим
    return;
  }

  map_ = MapLoader::LoadMap("level_" + QString::number(level_number + 1));
  if (map_ == nullptr) {
    return;
  }
  map_->SetGeneralVolume(general_volume_);
  map_->SetCurrentVolume(sound_volume_);
  audio_manager_->StopAudioPlayer(menu_audio_key_);
  audio_manager_->ReplayAudioPlayer(level_audio_key_);
  level_number_ = level_number;
  player_ = std::dynamic_pointer_cast<Player>(map_->GetPlayer());
  player_->SetAnimationName(player_animation_name_);
  player_->SetCurrentLevel(level_number + 1);
  interface_ = std::make_shared<GameInterface>(this);
  view_->takeCentralWidget();
  view_->setCentralWidget(interface_.get());
}

QString GameController::GetPlayerAnimation() const {
  return player_animation_name_;
}

void GameController::SetPlayerAnimation(const QString& animation_name) {
  player_animation_name_ = animation_name;
  settings_.setValue("animation_name", player_animation_name_);
}

int GameController::GetLastLevelMushrooms() const {
  return last_level_mushrooms_;
}

int GameController::GetLevelMushrooms(int level_number) const {
  return level_mushrooms_[level_number];
}

void GameController::LoadVolume() {
  general_volume_ = settings_.value("volume/general", 40).toInt();
  music_volume_ = settings_.value("volume/music", 40).toInt();
  sound_volume_ = settings_.value("volume/sound", 40).toInt();
  SetVolume(Volume::kGeneral, general_volume_);
  SetVolume(Volume::kMusic, music_volume_);
  SetVolume(Volume::kSound, sound_volume_);
}

void GameController::SetVolume(Volume volume, int power) {
  switch (volume) {
    case Volume::kGeneral: {
      general_volume_ = power;
      audio_manager_->SetGeneralVolume(general_volume_);
      menu_->SetGeneralVolume(general_volume_);
      settings_.setValue("volume/general", general_volume_);
      break;
    }
    case Volume::kMusic: {
      music_volume_ = power;
      audio_manager_->SetCurrentVolume(music_volume_);
      settings_.setValue("volume/music", music_volume_);
      break;
    }
    case Volume::kSound: {
      sound_volume_ = power;
      menu_->SetCurrentVolume(sound_volume_);
      settings_.setValue("volume/sound", sound_volume_);
      break;
    }
  }
}

int GameController::GetVolume(Volume volume) {
  switch (volume) {
    case Volume::kGeneral: {
      return general_volume_;
    }
    case Volume::kMusic: {
      return music_volume_;
    }
    case Volume::kSound: {
      return sound_volume_;
    }

    default: {
      return 0;
    }
  }
}

std::shared_ptr<Map> GameController::GetMap() {
  return map_;
}

void GameController::UpdateCamera(QPainter* painter) {
  if (map_ == nullptr) {
    return;
  }
  map_->UpdateCamera(painter);
}

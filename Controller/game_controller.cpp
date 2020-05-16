#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      menu_(std::make_shared<MainMenu>(this)),
      audio_manager_(std::make_shared<AudioManager>()) {
  view_->show();
  view_->setCentralWidget(menu_.get());

  level_audio_key_ = audio_manager_->CreateAudioPlayer(AudioName::kBackground);
  audio_manager_->SetPlayBackMode(level_audio_key_, QMediaPlaylist::Loop);
  menu_audio_key_ = audio_manager_->CreateAudioPlayer(AudioName::kMenuAudio);
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
    interface_->SetHealth(player_->GetCurrentHealth());
  }
}

void GameController::Draw(QPainter* painter) const {
  if (map_ == nullptr) {
    return;
  }
  map_->Draw(painter);
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
  if (key_code == Qt::Key_Left) {
    return Key::kLeft;
  }
  if (key_code == Qt::Key_Right) {
    return Key::kRight;
  }
  if (key_code == Qt::Key_Up) {
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
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kPause));
}

void GameController::OpenVictoryMenu() {
  audio_manager_->StopAudioPlayer(level_audio_key_);
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kVictory));
  CloseCurrentLevel();
}

void GameController::OpenFailMenu() {
  audio_manager_->StopAudioPlayer(level_audio_key_);
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kFail));
  CloseCurrentLevel();
}

void GameController::ResumeGame() {
  view_->takeCentralWidget();
  view_->setCentralWidget(interface_.get());

  audio_manager_->PlayAudioPlayer(level_audio_key_);
}

void GameController::RestartGame() {
  StartLevel(level_number_);
}

void GameController::StartNextLevel() {
  StartLevel(level_number_ + 1);
}

void GameController::OpenMenu(std::shared_ptr<Menu> menu) {
  menu_ = std::move(menu);
  if (menu_ != nullptr) {
    view_->takeCentralWidget();
    view_->setCentralWidget(menu_.get());
  }
}

void GameController::StartLevel(int level_number) {
  map_ = MapLoader::LoadMap("level_" + QString::number(level_number));
  if (map_ == nullptr) {
    return;
  }
  audio_manager_->StopAudioPlayer(menu_audio_key_);
  audio_manager_->ReplayAudioPlayer(level_audio_key_);
  level_number_ = level_number;
  player_ = std::dynamic_pointer_cast<Player>(map_->GetPlayer());
  interface_ = std::make_shared<GameInterface>(this);
  view_->takeCentralWidget();
  view_->setCentralWidget(interface_.get());
}

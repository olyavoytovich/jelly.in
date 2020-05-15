#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      menu_(std::make_shared<MainMenu>(this)) {
  view_->show();
  view_->setCentralWidget(menu_.get());
  settings_ = new QSettings("View, Controller and Models", "Jelly.in");
  player_animation_name_ = settings_->value("animation_name").toString();
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
  CloseCurrentLevel();
}

void GameController::OpenMainMenu() {
  OpenMenu(std::make_shared<MainMenu>(this));
  CloseCurrentLevel();
}

void GameController::OpenPauseMenu() {
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kPause));
}

void GameController::OpenVictoryMenu() {
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kVictory));
  CloseCurrentLevel();
}

void GameController::OpenFailMenu() {
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
  level_number_ = level_number;
  player_ = std::dynamic_pointer_cast<Player>(map_->GetPlayer());
  player_->SetAnimationName(player_animation_name_);
  interface_ = std::make_shared<GameInterface>(this);
  view_->takeCentralWidget();
  view_->setCentralWidget(interface_.get());
}

QString GameController::GetPlayerAnimation() const {
  return player_animation_name_;
}

void GameController::SetPlayerAnimation(const QString& animation_name) {
  player_animation_name_ = animation_name;
  settings_->setValue("animation_name", player_animation_name_);
}

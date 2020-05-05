#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      menu_(std::make_shared<MainMenu>(this)) {
  view_->show();
  view_->setCentralWidget(menu_.get());
}

void GameController::Update(int time) {
  if (map_ == nullptr) {
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
    OpenPauseMenu();
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

void GameController::OpenChooseLevelMenu() {
  OpenMenu(std::make_shared<ChooseLevelMenu>(this));
  interface_ = nullptr;
  map_ = nullptr;
  player_ = nullptr;
}

void GameController::OpenMainMenu() {
  OpenMenu(std::make_shared<MainMenu>(this));
  interface_ = nullptr;
  map_ = nullptr;
  player_ = nullptr;
}

void GameController::OpenPauseMenu() {
  OpenMenu(std::make_shared<PauseMenu>(this));
  interface_ = nullptr;
  map_ = nullptr;
  player_ = nullptr;
}

void GameController::OpenVictoryMenu() {
  OpenMenu(std::make_shared<VictoryMenu>(this));
  interface_ = nullptr;
  map_ = nullptr;
  player_ = nullptr;
}

void GameController::OpenFailMenu() {
  OpenMenu(std::make_shared<FailMenu>(this));
  interface_ = nullptr;
  map_ = nullptr;
  player_ = nullptr;
}

void GameController::OpenMenu(std::shared_ptr<Menu> menu) {
  menu_ = std::move(menu);
  if (menu_ != nullptr) {
    view_->setCentralWidget(menu_.get());
  }
}

void GameController::StartLevel(const QString& level_number) {
  level_number_ = level_number.toInt();
  map_ = MapLoader::LoadMap("level_" + level_number);
  player_ = std::dynamic_pointer_cast<Player>(map_->GetPlayer());
  interface_ = std::make_shared<GameInterface>(this);
  OpenMenu(interface_);
}

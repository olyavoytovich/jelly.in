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
}

void GameController::Draw(QPainter* painter) const {
  if (map_ == nullptr) {
    return;
  }
  map_->Draw(painter);
}

void GameController::PressKey(int key_code) {
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
}

void GameController::OpenMainMenu() {
  OpenMenu(std::make_shared<MainMenu>(this));
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
  menu_ = nullptr;
}

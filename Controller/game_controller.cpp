#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)) {
  auto menu = std::make_shared<MainMenu>(this);
  OpenMenu(menu);
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
  return Key::kAnyKey;
}

void GameController::OpenChooseLevelMenu() {
  auto menu = std::make_shared<ChooseLevelMenu>(this);
  OpenMenu(menu);
}

void GameController::OpenMainMenu() {
  auto menu = std::make_shared<MainMenu>(this);
  OpenMenu(menu);
}

void GameController::OpenMenu(const std::shared_ptr<Menu>& menu) {
  QRect boundary_rectangle;
  if (menu_ == nullptr) {
    boundary_rectangle = view_->geometry();
  } else {
    boundary_rectangle = menu_->geometry();
    menu_->close();
  }
  menu->setGeometry(boundary_rectangle);
  menu_ = menu;
  if (menu != nullptr) {
    menu_->show();
  }
}

void GameController::StartLevel(int level_number) {
  level_number_ = level_number;
  map_ = MapLoader::LoadMap("level_" + QString::number(level_number_));
  view_->show();
}

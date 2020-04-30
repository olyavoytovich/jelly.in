#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("level_1")) {
  view_->show();
  menu_ = std::make_shared<MainMenu>(view_->geometry(), this);
  menu_->show();
  view_->hide();
}

void GameController::Update(int time) {
  map_->Update(time);
  view_->repaint();
}

void GameController::Draw(QPainter* painter) const {
  map_->Draw(painter);
}

void GameController::PressKey(int key_code) {
  map_->SetPressedKeyStatus(GetKeyFromCode(key_code), true);
}

void GameController::ClampKey(int key_code) {
  map_->SetClampedKeyStatus(GetKeyFromCode(key_code), true);
}

void GameController::ReleaseKey(int key_code) {
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
  if (menu_ != nullptr) {
    menu_->hide();
  }
  QRect boundary_rectangle = menu_->geometry();
  menu_->close();
  menu_ = std::make_shared<ChooseLevelMenu>(boundary_rectangle, this);
  menu_->show();
}

void GameController::CloseChooseLevelMenu() {
  if (menu_ != nullptr) {
    menu_->hide();
  }
  QRect boundary_rectangle = menu_->geometry();
  menu_ = std::make_shared<MainMenu>(boundary_rectangle, this);
  menu_->show();
}

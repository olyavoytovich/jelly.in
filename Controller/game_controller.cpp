#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("level_1", this)),
      is_key_pressed_(3, false),
      is_key_clamped_(3, false) {
  view_->show();
}

void GameController::Update(int time) {
  map_->Update(time);
  view_->repaint();
  is_key_pressed_.assign(is_key_pressed_.size(), false);
}

void GameController::Draw(QPainter* painter) const {
  map_->Draw(painter);
}

void GameController::PressKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    is_key_pressed_[static_cast<int>(Key::LEFT)] = true;
  }
  if (key_code == Qt::Key_Right) {
    is_key_pressed_[static_cast<int>(Key::RIGHT)] = true;
  }
  if (key_code == Qt::Key_Up) {
    is_key_pressed_[static_cast<int>(Key::UP)] = true;
  }
}

void GameController::ClampKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    is_key_clamped_[static_cast<int>(Key::LEFT)] = true;
    is_key_clamped_[static_cast<int>(Key::RIGHT)] = false;
  }
  if (key_code == Qt::Key_Right) {
    is_key_clamped_[static_cast<int>(Key::LEFT)] = false;
    is_key_clamped_[static_cast<int>(Key::RIGHT)] = true;
  }
  if (key_code == Qt::Key_Up) {
    is_key_clamped_[static_cast<int>(Key::UP)] = true;
  }
}

void GameController::ReleaseKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    is_key_pressed_[static_cast<int>(Key::LEFT)] = false;
    is_key_clamped_[static_cast<int>(Key::LEFT)] = false;
  }
  if (key_code == Qt::Key_Right) {
    is_key_pressed_[static_cast<int>(Key::RIGHT)] = false;
    is_key_clamped_[static_cast<int>(Key::RIGHT)] = false;
  }
  if (key_code == Qt::Key_Up) {
    is_key_pressed_[static_cast<int>(Key::UP)] = false;
    is_key_clamped_[static_cast<int>(Key::UP)] = false;
  }
}

bool GameController::GetPressedKeyStatus(Key key) {
  return is_key_pressed_[static_cast<unsigned int>(key)];
}

bool GameController::GetClampedKeyStatus(Key key) {
  return is_key_clamped_[static_cast<unsigned int>(key)];
}

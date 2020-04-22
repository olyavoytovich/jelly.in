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
    is_key_pressed_[static_cast<int>(Key::kLeft)] = true;
  }
  if (key_code == Qt::Key_Right) {
    is_key_pressed_[static_cast<int>(Key::kRight)] = true;
  }
  if (key_code == Qt::Key_Up) {
    is_key_pressed_[static_cast<int>(Key::kUp)] = true;
  }
}

void GameController::ClampKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    is_key_clamped_[static_cast<int>(Key::kLeft)] = true;
    is_key_clamped_[static_cast<int>(Key::kRight)] = false;
  }
  if (key_code == Qt::Key_Right) {
    is_key_clamped_[static_cast<int>(Key::kLeft)] = false;
    is_key_clamped_[static_cast<int>(Key::kRight)] = true;
  }
  if (key_code == Qt::Key_Up) {
    is_key_clamped_[static_cast<int>(Key::kUp)] = true;
  }
}

void GameController::ReleaseKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    is_key_pressed_[static_cast<int>(Key::kLeft)] = false;
    is_key_clamped_[static_cast<int>(Key::kLeft)] = false;
  }
  if (key_code == Qt::Key_Right) {
    is_key_pressed_[static_cast<int>(Key::kRight)] = false;
    is_key_clamped_[static_cast<int>(Key::kRight)] = false;
  }
  if (key_code == Qt::Key_Up) {
    is_key_pressed_[static_cast<int>(Key::kUp)] = false;
    is_key_clamped_[static_cast<int>(Key::kUp)] = false;
  }
}

bool GameController::IsKeyPressed(Key key) {
  return is_key_pressed_[static_cast<int>(key)];
}

bool GameController::IsKeyClamped(Key key) {
  return is_key_clamped_[static_cast<int>(key)];
}

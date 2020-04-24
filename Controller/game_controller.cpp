#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("level_1")) {
  view_->show();
}

void GameController::Update(int time) {
  map_->Update(time);
  view_->repaint();
}

void GameController::Draw(QPainter* painter) const {
  map_->Draw(painter);
}

void GameController::PressKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    map_->SetPressedKeyStatus(Key::kLeft, true);
  } else if (key_code == Qt::Key_Right) {
    map_->SetPressedKeyStatus(Key::kRight, true);
  } else if (key_code == Qt::Key_Up) {
    map_->SetPressedKeyStatus(Key::kUp, true);
  }
}

void GameController::ClampKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    map_->SetClampedKeyStatus(Key::kLeft, true);
    map_->SetClampedKeyStatus(Key::kRight, false);
  } else if (key_code == Qt::Key_Right) {
    map_->SetClampedKeyStatus(Key::kLeft, false);
    map_->SetClampedKeyStatus(Key::kRight, true);
  } else if (key_code == Qt::Key_Up) {
    map_->SetClampedKeyStatus(Key::kUp, true);
  }
}

void GameController::ReleaseKey(Key key) {
  map_->SetPressedKeyStatus(key, false);
  map_->SetClampedKeyStatus(key, false);
}

void GameController::ReleaseKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    ReleaseKey(Key::kLeft);
  } else if (key_code == Qt::Key_Right) {
    ReleaseKey(Key::kRight);
  } else if (key_code == Qt::Key_Up) {
    ReleaseKey(Key::kUp);
  }
}

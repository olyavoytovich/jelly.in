#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      game_object_(std::make_shared<GameObject>()) {
  view_->show();
}

void GameController::Update() {
  game_object_->Update();
  view_->repaint();
}

void GameController::Draw(QPainter* painter) const {
  game_object_->Draw(painter);
}

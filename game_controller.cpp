#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("test_map")) {
  view_->show();
}

void GameController::Update() {
  map_->Update();
  view_->repaint();
}

void GameController::Draw(QPainter* painter) const {
  map_->Draw(painter);
}

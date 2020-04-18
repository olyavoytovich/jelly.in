#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("level_1")) {
  view_->show();
}

void GameController::Update(int time) {
  // Функция Step() обновляет Box2D.
  // Первый передаваемый параметр - время. Время передается в миллисекундах, а
  // для Box2D время должно измеряться в секундах.
  map_->Update(time);
  view_->repaint();
}

void GameController::Draw(QPainter* painter) const {
  map_->Draw(painter);
}

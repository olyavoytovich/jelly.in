#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)) {
  world_ = new b2World(b2Vec2(0, -10));
  QPolygon pol = QRect(10, 10, 10, 10);

  dynamic_entity_ = new Entity(pol, 2, world_, b2_dynamicBody);
  static_entity_ = new Entity(pol, 2, world_, b2_staticBody);

  view_->show();
}

void GameController::Update(int time) {
  world_->Step((float) (time / 1000.), 6, 2);
  view_->repaint();
}

void GameController::Draw(QPainter* painter) const {
  painter->setBrush(Qt::BDiagPattern);
  static_entity_->Draw(painter);
  dynamic_entity_->Draw(painter);
}

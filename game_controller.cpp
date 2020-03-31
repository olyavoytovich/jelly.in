#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("test_map")) {
  world_ = new b2World(b2Vec2(0, -10));

  QPoint position = QPoint(300, -300);
  QPoint position2 = QPoint(200, -100);
  QPoint position3 = QPoint(500, -100);

  QPolygon polygon = QRect(10, 10, 10, 10);
  QPolygon p({QPoint(15, 0), QPoint(30, 0), QPoint(50, 30), QPoint(0, 30)});

  std::vector<std::pair<int, QPoint>> vec_of_circs;
  std::vector<std::pair<QPolygon, QPoint>> vec_of_polygons;

  vec_of_circs.emplace_back(30, QPoint(10, 30));
  vec_of_polygons.emplace_back(polygon, QPoint(60, 10));

  entity_ = new Entity(p, world_, b2_staticBody, position);
  entity2_ = new Entity(50, world_, b2_dynamicBody, position2);
  entity3_ = new Entity(world_,
                        b2_dynamicBody,
                        position3,
                        vec_of_circs,
                        vec_of_polygons);

  view_->show();
}

void GameController::Update(int time) {
  world_->Step(static_cast<float> (time / 1000.), 6, 2);
  map_->Update();
  view_->repaint();
}

void GameController::Draw(QPainter* painter) const {
  map_->Draw(painter);

  painter->setBrush(Qt::BDiagPattern);
  entity_->Draw(painter);
  entity2_->Draw(painter);
  entity3_->Draw(painter);
}

GameController::~GameController() {
  delete entity_;
  delete entity2_;
  delete entity3_;
}

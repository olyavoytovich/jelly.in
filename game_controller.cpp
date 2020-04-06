#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("test_map")),
      world_(std::make_shared<b2World>(b2Vec2(0, -10))) {

  std::vector<CircleShape> circles;
  std::vector<PolygonShape> polygons;

  circles.emplace_back(CircleShape({30, Point(80, 30)}));
  polygons.emplace_back(PolygonShape({QRect(10, 10, 10, 10), Point(0, 0)}));

  entity_ = std::make_shared<Entity>(world_,
                                     b2_staticBody,
                                     Point(300, -300),
                                     QPolygon({QPoint(15, 0), QPoint(30, 60),
                                               QPoint(50, 30), QPoint(0, 30)}));

  entity2_ = std::make_shared<Entity>(world_,
                                      b2_dynamicBody,
                                      Point(200, -100),
                                      50);

  entity3_ = std::make_shared<Entity>(world_,
                                      b2_dynamicBody,
                                      Point(500, -100),
                                      circles,
                                      polygons);

  view_->show();
}

void GameController::Update(int time) {
  // Функция Step() обновляет Box2D.
  // Первый передаваемый параметр - время. Время передается в миллисекундах, а
  // для Box2D время должно измеряться в секундах.
  world_->Step(static_cast<float>(time / 1000.0),
               kVelocityAccuracy,
               kPositionAccuracy);
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

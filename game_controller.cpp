#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("test_map")),
      world_(std::make_shared<b2World>(b2Vec2(0, -10))) {

  std::vector<CircleShape> circles;
  std::vector<PolygonShape> polygons;

  circles.emplace_back(CircleShape({30, QPoint(80, 30)}));
  polygons.emplace_back(PolygonShape({QRect(10, 10, 10, 10), QPoint(0, 0)}));

  entity_ = std::make_shared<Entity>(world_,
                                     b2_staticBody,
                                     QPoint(300, -300),
                                     QPolygon({QPoint(15, 0), QPoint(30, 60),
                                               QPoint(50, 30), QPoint(0, 30)}));

  entity2_ = std::make_shared<Entity>(world_,
                                      b2_dynamicBody,
                                      QPoint(200, -100),
                                      50);

  entity3_ = std::make_shared<Entity>(world_,
                                      b2_dynamicBody,
                                      QPoint(500, -100),
                                      circles,
                                      polygons);

  view_->show();
}

void GameController::Update(int time) {
  // Первый передаваемый параметр - время. Для Box2D время должно измеряться
  // в секундах, в то время как QTimer измеряет его в миллисекундах, поэтому
  // изменяем единицу измерения времени.
  // Второй параметр- velocity iterations, третий - position iterations.
  // Предлагаемое количество итераций для Box2D равно 6 и 2 соответственно.
  // Использование меньшего количества повышают производительность, но страдает
  // точность. Аналогично, использование большего количества итераций снижает
  // производительность, но улучшает качество вашей симуляции.
  world_->Step(static_cast<float>(time / 1000.0),
               kVelocityIterations,
               kPositionIterations);
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

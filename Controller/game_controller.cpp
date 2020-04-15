#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("test_map")),
      world_(std::make_shared<b2World>(b2Vec2(0, -10))) {

  platform1_ = std::make_shared<Entity>(world_,
                                        b2_staticBody,
                                        Point(400, -120),
                                        QPolygon(QRect(-400, 0, 800, 20)));

  platform2_ = std::make_shared<Entity>(world_,
                                        b2_staticBody,
                                        Point(400, -220),
                                        QPolygon(QRect(-400, 0, 800, 20)));

  platform3_ = std::make_shared<Entity>(world_,
                                        b2_staticBody,
                                        Point(400, -320),
                                        QPolygon(QRect(-400, 0, 800, 20)));

  std::vector<Point> way_points_for_patroller;
  way_points_for_patroller.emplace_back(150, -100);
  way_points_for_patroller.emplace_back(350, -100);
  patroller_ = std::make_shared<Patroller>(world_,
                                           b2_dynamicBody,
                                           Point(150, -100),
                                           QPolygon({{-20, 50}, {30, 50},
                                                     {30, 0}, {-20, 0}}),
                                           way_points_for_patroller, 80);

  std::vector<Point> way_points_for_shooter1;
  way_points_for_shooter1.emplace_back(150, -200);
  way_points_for_shooter1.emplace_back(350, -200);
  shooter1_ = std::make_shared<Shooter>(world_,
                                        b2_dynamicBody,
                                        Point(150, -200),
                                        QPolygon(QRect(-20, 0, 50, 50)),
                                        way_points_for_shooter1,
                                        BulletDirection::kLeftRight,
                                        1008, 100, 5, 40);

  std::vector<Point> way_points_for_shooter2;
  way_points_for_shooter2.emplace_back(50, -400);
  way_points_for_shooter2.emplace_back(250, -400);
  shooter2_ = std::make_shared<Shooter>(world_,
                                        b2_kinematicBody,
                                        Point(50, -400),
                                        QPolygon(QRect(-20, 20, 80, 30)),
                                        way_points_for_shooter2,
                                        BulletDirection::kBottom,
                                        600, 80, 5, 80);

  std::vector<Point> way_points_for_shooter3;
  way_points_for_shooter3.emplace_back(150, -275);
  way_points_for_shooter3.emplace_back(350, -275);
  shooter3_ = std::make_shared<Shooter>(world_,
                                        b2_dynamicBody,
                                        Point(150, -275),
                                        25,
                                        way_points_for_shooter3,
                                        BulletDirection::kLeftRight,
                                        1018, 120, 5, 100);

  std::vector<Point> way_points_for_shooter4;
  way_points_for_shooter4.emplace_back(350, -400);
  way_points_for_shooter4.emplace_back(550, -400);
  std::vector<CircleShape> circle_shapes = {CircleShape(25, Point(10, 20))};
  std::vector<PolygonShape> polygon_shapes =
      {PolygonShape(QPolygon(QRect(-20, 0, 50, 50)), Point(0, 0))};
  shooter4_ = std::make_shared<Shooter>(world_,
                                        b2_kinematicBody,
                                        Point(350, -400),
                                        circle_shapes,
                                        polygon_shapes,
                                        way_points_for_shooter4,
                                        BulletDirection::kBottom,
                                        500, 120, 5, 80);

  view_->show();
}

void GameController::Update(int time) {
  // Функция Step() обновляет Box2D.
  // Первый передаваемый параметр - время. Время передается в миллисекундах, а
  // для Box2D время должно измеряться в секундах.
  world_->Step(static_cast<float>(time / 1000.0),
               kVelocityAccuracy,
               kPositionAccuracy);
  map_->Update(time);

  patroller_->Update(time);
  shooter1_->Update(time);
  shooter2_->Update(time);
  shooter3_->Update(time);
  shooter4_->Update(time);
  view_->repaint();
}

void GameController::Draw(QPainter* painter) const {
  map_->Draw(painter);

  painter->setBrush(Qt::BDiagPattern);

  platform1_->Draw(painter);
  platform2_->Draw(painter);
  platform3_->Draw(painter);

  patroller_->Draw(painter);
  shooter1_->Draw(painter);
  shooter2_->Draw(painter);
  shooter3_->Draw(painter);
  shooter4_->Draw(painter);
}

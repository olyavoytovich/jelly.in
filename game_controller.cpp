#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("test_map")),
      world_(std::make_shared<b2World>(b2Vec2(0, -200))),
      is_key_pressed_(3, 0) {

  std::vector<CircleShape> circles;
  std::vector<PolygonShape> polygons;

  circles.emplace_back(30, Point(80, 60));
  polygons.emplace_back(QRect(10, 10, 10, 10), Point(0, 0));

  entity_ = std::make_shared<Entity>(world_,
                                     b2_staticBody,
                                     Point(400, -220),
                                     QPolygon(QRect(-400, 0, 800, 20)));

//  entity2_ = std::make_shared<Entity>(world_,
//                                      b2_dynamicBody,
//                                      Point(200, -100),
//                                      50);

//  entity3_ = std::make_shared<Player>(world_,
//                                     b2_dynamicBody,
//                                     Point(200, -220),
//                                     QPolygon(QRect(10, 0, -10, 20)),
//                                     this);

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
  //entity3_->Update();
}

void GameController::Draw(QPainter* painter) const {
  map_->Draw(painter);

  painter->setBrush(Qt::BDiagPattern);
  entity_->Draw(painter);
 // entity2_->Draw(painter);
 // entity3_->Draw(painter);
 // player_->Draw(painter);
}

void GameController::PressKey(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Left: {
      is_key_pressed_[static_cast<int>(Keys::LEFT)] = 1;
      is_key_pressed_[static_cast<int>(Keys::RIGHT)] = 0;
      break;
    }
    case Qt::Key_Right: {
      is_key_pressed_[static_cast<int>(Keys::LEFT)] = 0;
      is_key_pressed_[static_cast<int>(Keys::RIGHT)] = 1;
      break;
    }
  }
  if (event->key() == Qt::Key_Up) {
    is_key_pressed_[static_cast<int>(Keys::UP)] = 1;
  }
}

void GameController::ReleaseKey(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Left: {
      is_key_pressed_[static_cast<int>(Keys::LEFT)] = 0;
      break;
    }
    case Qt::Key_Right: {
      is_key_pressed_[static_cast<int>(Keys::RIGHT)] = 0;
      break;
    }
    case Qt::Key_Up: {
      is_key_pressed_[static_cast<int>(Keys::UP)] = 0;
      break;
    }
  }
}

bool GameController::GetKey(Keys key) {
  return is_key_pressed_[static_cast<int>(key)];
}

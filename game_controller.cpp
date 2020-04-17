#include "game_controller.h"

GameController::GameController()
    : view_(std::make_shared<View>(this)),
      map_(MapLoader::LoadMap("test_map")),
      world_(std::make_shared<b2World>(b2Vec2(0, -10))),
      is_key_pressed_(3, 0), is_key_clamped_(3, 0) {

  std::vector<CircleShape> circles;
  std::vector<PolygonShape> polygons;

  circles.emplace_back(30, Point(80, 60));
  polygons.emplace_back(QRect(10, 10, 10, 10), Point(0, 0));

  entity_ = std::make_shared<Entity>(world_,
                                     b2_staticBody,
                                     Point(400, -220),
                                     QPolygon(QRect(-400, 0, 800, 20)));

  player_ = std::make_shared<Player>(world_,
                                     b2_dynamicBody,
                                     Point(200, -120),
                                     QPolygon(QRect(15, 0, -15, 15)),
                                     this);

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
  player_->Update();
  is_key_pressed_.assign(is_key_pressed_.size(), 0);
}

void GameController::Draw(QPainter* painter) const {
  map_->Draw(painter);

  painter->setBrush(Qt::BDiagPattern);
  entity_->Draw(painter);
  player_->Draw(painter);
}

void GameController::PressKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    is_key_pressed_[static_cast<int>(Key::LEFT)] = true;
  }
  if (key_code == Qt::Key_Right) {
    is_key_pressed_[static_cast<int>(Key::RIGHT)] = true;
  }
  if (key_code == Qt::Key_Up) {
    is_key_pressed_[static_cast<int>(Key::UP)] = true;
  }
}

void GameController::ClampKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    is_key_clamped_[static_cast<int>(Key::LEFT)] = true;
    is_key_clamped_[static_cast<int>(Key::RIGHT)] = false;
  }
  if (key_code == Qt::Key_Right) {
    is_key_clamped_[static_cast<int>(Key::LEFT)] = false;
    is_key_clamped_[static_cast<int>(Key::RIGHT)] = true;
  }
  if (key_code == Qt::Key_Up) {
    is_key_clamped_[static_cast<int>(Key::UP)] = true;
  }
}

void GameController::ReleaseKey(int key_code) {
  if (key_code == Qt::Key_Left) {
    is_key_pressed_[static_cast<int>(Key::LEFT)] = false;
    is_key_clamped_[static_cast<int>(Key::LEFT)] = false;
  }
  if (key_code == Qt::Key_Right) {
    is_key_pressed_[static_cast<int>(Key::RIGHT)] = false;
    is_key_clamped_[static_cast<int>(Key::RIGHT)] = false;
  }
  if (key_code == Qt::Key_Up) {
    is_key_pressed_[static_cast<int>(Key::UP)] = false;
    is_key_clamped_[static_cast<int>(Key::UP)] = false;
  }
}

bool GameController::GetPressedKeyStatus(Key key) {
  return is_key_pressed_[static_cast<int>(key)];
}

bool GameController::GetClampedKeyStatus(Key key) {
  return is_key_clamped_[static_cast<int>(key)];
}

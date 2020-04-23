#include "map.h"

Map::Map(const QImage& map_image)
    : world_(std::make_shared<b2World>(b2Vec2(0, 10))),
      current_camera_(0, 0, kVisibleSize.x(), kVisibleSize.y()),
      map_image_(map_image),
      scaled_map_image_(map_image),
      is_key_pressed_(3, false),
      is_key_clamped_(3, false) {}

void Map::Update(int time) {
  player_->Update(time);

  while (!game_objects_to_add_.empty()) {
    game_objects_.push_back(game_objects_to_add_.back());
    game_objects_to_add_.pop_back();
  }

  for (auto& object : game_objects_) {
    if (object->IsDeleted()) {
      if (object->GetB2Body() != nullptr) {
        world_->DestroyBody(object->GetB2Body());
      }
      object.reset();
    } else {
      object->Update(time);
    }
  }
  game_objects_.erase(std::remove_if(game_objects_.begin(),
                                     game_objects_.end(),
                                     [](const auto& object) {
                                       return object == nullptr;
                                     }), game_objects_.end());

  // Функция Step() обновляет Box2D.
  // Первый передаваемый параметр - время. Время передается в миллисекундах, а
  // для Box2D время должно измеряться в секундах.
  world_->Step(static_cast<float>(time / 1000.0),
               kVelocityAccuracy,
               kPositionAccuracy);
  is_key_pressed_.assign(is_key_pressed_.size(), false);
}

void Map::Draw(QPainter* painter) {
  painter->save();

  double scale = std::max(
      static_cast<double>(painter->window().width()) / kVisibleSize.x(),
      static_cast<double>(painter->window().height()) / kVisibleSize.y());

  UpdateCameraPosition();
  painter->translate(
      (painter->window().width() - kVisibleSize.x() * scale) / 2.0,
      (painter->window().height() - kVisibleSize.y() * scale) / 2.0);
  painter->translate(-current_camera_.topLeft() * scale);

  UpdateImageScale(static_cast<int>(map_image_.width() * scale),
                   static_cast<int>(map_image_.height() * scale));
  painter->drawImage(0, 0, scaled_map_image_);

  painter->scale(scale, scale);
  painter->setBrush(QBrush(Qt::black, Qt::BrushStyle::BDiagPattern));

  for (const auto& object : game_objects_) {
    object->Draw(painter);
  }

  painter->setBrush(QBrush(Qt::green, Qt::BrushStyle::SolidPattern));
  player_->Draw(painter);

  painter->restore();
}

void Map::AddGameObject(std::shared_ptr<GameObject> object) {
  game_objects_to_add_.push_back(std::move(object));
}

void Map::SetPlayerObject(std::shared_ptr<GameObject> player) {
  player_ = std::move(player);
}

void Map::SetPressedKeyStatus(Key key, bool value) {
  is_key_pressed_[static_cast<int>(key)] = value;
}

void Map::SetClampedKeyStatus(Key key, bool value) {
  is_key_clamped_[static_cast<int>(key)] = value;
}

bool Map::IsKeyPressed(Key key) {
  return is_key_pressed_[static_cast<int>(key)];
}

bool Map::IsKeyClamped(Key key) {
  return is_key_clamped_[static_cast<int>(key)];
}

void Map::UpdateImageScale(int width, int height) {
  if (scaled_map_image_.width() != width
      && scaled_map_image_.height() != height) {
    scaled_map_image_ = map_image_.scaled(width, height, Qt::KeepAspectRatio);
  }
}

b2Body* Map::CreateBody(b2BodyDef* body_definition) {
  return world_->CreateBody(body_definition);
}

void Map::UpdateCameraPosition() {
  QPoint target = player_->GetPositionInPixels() - current_camera_.center();
  if (kPlayerBoundary.x() / 2 < target.x()) {
    current_camera_.translate(target.x() - kPlayerBoundary.x() / 2, 0);
  }
  if (-kPlayerBoundary.x() / 2 > target.x()) {
    current_camera_.translate(target.x() + kPlayerBoundary.x() / 2, 0);
  }
  if (kPlayerBoundary.y() / 2 < target.y()) {
    current_camera_.translate(0, target.y() - kPlayerBoundary.y() / 2);
  }
  if (-kPlayerBoundary.y() / 2 > target.y()) {
    current_camera_.translate(0, target.y() + kPlayerBoundary.y() / 2);
  }
}

#include "map.h"

Map::Map(const QImage& map_image)
    : map_image_(map_image),
      scaled_map_image_(map_image),
      world_(std::make_shared<b2World>(b2Vec2(0, 10))) {}

void Map::Update(int time) {
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

  world_->Step(static_cast<float>(time / 1000.0),
               kVelocityAccuracy,
               kPositionAccuracy);
}

void Map::Draw(QPainter* painter) {
  painter->save();

  UpdateImageScale(painter->window().width(), painter->window().height());
  painter->drawImage(0, 0, scaled_map_image_);

  double scale = std::min(
      static_cast<double>(painter->window().width()) / map_image_.width(),
      static_cast<double>(painter->window().height()) / map_image_.height());
  painter->scale(scale, scale);
  painter->setBrush(QBrush(Qt::black, Qt::BrushStyle::BDiagPattern));

  for (const auto& object : game_objects_) {
    object->Draw(painter);
  }

  painter->restore();
}

void Map::AddGameObject(const std::shared_ptr<GameObject>& object) {
  game_objects_to_add_.push_back(object);
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

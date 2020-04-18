#include "map.h"

Map::Map(const QImage& map_image)
    : map_image_(map_image),
      scaled_map_image_(map_image),
      world_(std::make_shared<b2World>(b2Vec2(0, 0.1))) {}

void Map::Update(int time) {
  world_->Step(static_cast<float>(time / 1000.0),
               kVelocityAccuracy,
               kPositionAccuracy);

  for (const auto& object : game_objects_) {
    object->Update(time);
  }
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
  game_objects_.push_back(object);
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

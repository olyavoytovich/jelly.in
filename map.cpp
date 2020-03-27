#include "map.h"

Map::Map(QImage* map_image) : map_image_(map_image) {}

void Map::Update() {
  for (const auto& object : game_objects_) {
    object->Update();
  }
}

void Map::Draw(QPainter* painter) const {
  painter->save();
  double scale = std::min(
      static_cast<double>(painter->window().width()) / map_image_->width(),
      static_cast<double>(painter->window().height()) / map_image_->height());
  painter->scale(scale, scale);

  painter->drawImage(map_image_->rect(), *map_image_);
  for (const auto& object : game_objects_) {
    object->Draw(painter);
  }

  painter->restore();
}

void Map::AddGameObject(const std::shared_ptr<GameObject>& object) {
  game_objects_.push_back(object);
}

Map::~Map() {
  delete map_image_;
}

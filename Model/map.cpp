#include "map.h"

Map::Map(const QImage& map_image)
    : map_image_(map_image), scaled_map_image_(map_image) {}

void Map::Update(int time) {
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

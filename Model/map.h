#ifndef MAP_H_
#define MAP_H_

#include <QPainter>
#include <memory>
#include <vector>

#include "game_object.h"

class Map {
 public:
  explicit Map(const QImage& map_image);
  ~Map() = default;

  void Update(int time);
  void Draw(QPainter* painter);

  void AddGameObject(const std::shared_ptr<GameObject>& object);

 private:
  void UpdateImageScale(int width, int height);

 private:
  std::vector<std::shared_ptr<GameObject>> game_objects_;
  QImage map_image_;
  QImage scaled_map_image_;
};

#endif  // MAP_H_

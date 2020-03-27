#ifndef MAP_H_
#define MAP_H_

#include <memory>
#include <QPainter>
#include <vector>

#include "game_object.h"

class Map {
 public:
  explicit Map(QImage* map_image);
  ~Map();

  void Update();
  void Draw(QPainter* painter) const;

  void AddGameObject(const std::shared_ptr<GameObject>& object);

 private:
  std::vector<std::shared_ptr<GameObject>> game_objects_;
  QImage* map_image_;
};

#endif  // MAP_H_

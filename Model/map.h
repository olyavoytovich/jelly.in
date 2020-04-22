#ifndef MODEL_MAP_H_
#define MODEL_MAP_H_

#include <QPainter>
#include <memory>
#include <vector>

#include "box2d/box2d.h"
#include "game_object.h"

class Map {
 public:
  explicit Map(const QImage& map_image);
  ~Map() = default;

  void Update(int time);
  void Draw(QPainter* painter);

  void AddGameObject(std::shared_ptr<GameObject> object);
  void SetPlayerObject(std::shared_ptr<GameObject> player);

  b2Body* CreateBody(b2BodyDef* body_definition);

 private:
  void UpdateImageScale(int width, int height);
  void UpdateCameraPosition();

 private:
  // Данные константы передаются в функцию Step(), которая используется при
  // реализации метода Update(int time).
  // kVelocityAccuracy и kPositionAccuracy отвечают за точность вычисления
  // импульсов, необходимых для правильного движения и положения тел.
  // Предлагаемое количество итераций для Box2D равно 6 и 2 соответственно.
  // Использование меньшего количества повышают производительность, но страдает
  // точность. Аналогично, использование большего количества итераций снижает
  // производительность, но улучшает качество симуляции.
  const int kVelocityAccuracy = 6;
  const int kPositionAccuracy = 2;

  // Width and Height of the Camera
  const QPoint kVisibleSize = QPoint(800, 600);
  // Width and Height of the Player's rectangle (from Camera's center)
  const QPoint kPlayerBoundary = QPoint(200, 150);

 private:
  std::shared_ptr<b2World> world_;
  std::shared_ptr<GameObject> player_;

  QRect current_camera_;

  std::vector<std::shared_ptr<GameObject>> game_objects_;
  std::vector<std::shared_ptr<GameObject>> game_objects_to_add_;
  QImage map_image_;
  QImage scaled_map_image_;
};

#endif  // MODEL_MAP_H_

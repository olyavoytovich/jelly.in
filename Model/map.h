#ifndef MODEL_MAP_H_
#define MODEL_MAP_H_

#include <QPainter>
#include <memory>
#include <utility>
#include <vector>

#include "Controller/abstract_game_controller.h"
#include "audio_manager.h"
#include "box2d/box2d.h"
#include "game_object.h"

enum class EntityType {
  kPlayer = 1,
  kBullet = 2,
  kChestnut = 4,
  kCloud = 8,
  kBurdock = 16,
  kSunflower = 32,
  kGround = 64,
  kPlayerPart = 128,
  kSpikes = 256,
  kExit = 512,
  kPlate = 1024,
  kMushroom = 2048,
  kDefault
};

class Map {
 public:
  explicit Map(const QImage& map_image);
  ~Map() = default;

  void Update(int time);
  void Draw(QPainter* painter);

  double GetScale() const;
  std::shared_ptr<GameObject> GetPlayer() const;
  int GetPickedMushroomsCount() const;

  void AddGameObject(std::shared_ptr<GameObject> object);
  void SetPlayerObject(std::shared_ptr<GameObject> player);
  void SetContactListener(std::shared_ptr<b2ContactListener> listener);

  b2Body* CreateBody(b2BodyDef* body_definition);

  void SetPressedKeyStatus(Key key, bool is_pressed);
  void SetClampedKeyStatus(Key key, bool is_clamped);

  bool IsKeyPressed(Key key);
  bool IsKeyClamped(Key key);

  void PickUpMushroom();

  std::shared_ptr<AudioManager> GetAudioManager() const;
  void SetGeneralVolume(int general_volume);
  void SetCurrentVolume(int current_volume);

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

  // Ширина и Высота камеры
  const QPoint kVisibleSize = QPoint(1600, 900);
  // Ширина и Высота прямоугольника игрока (из центра камеры)
  const QPoint kPlayerBoundary = QPoint(400, 225);

 private:
  std::shared_ptr<b2World> world_;
  std::shared_ptr<GameObject> player_;
  std::shared_ptr<b2ContactListener> contact_listener_;

  QRect current_camera_;

  std::vector<std::shared_ptr<GameObject>> game_objects_;
  std::vector<std::shared_ptr<GameObject>> game_objects_to_add_;
  QImage map_image_;
  QImage scaled_map_image_;

  double scale_ = 1;
  QPoint shift_ = QPoint();

  int picked_mushrooms_ = 0;

  std::vector<bool> is_key_pressed_;
  std::vector<bool> is_key_clamped_;

  std::shared_ptr<AudioManager> audio_manager_;
};

#endif  // MODEL_MAP_H_

#ifndef MODEL_MAP_H_
#define MODEL_MAP_H_

#include <QPainter>
#include <memory>
#include <utility>
#include <vector>

#include "audio_manager.h"
#include "box2d/box2d.h"
#include "camera.h"
#include "game_object.h"

// kAnyKey должен быть в enum последним
enum class Key {
  kLeft,
  kUp,
  kRight,
  kSpace,
  kAnyKey
};

class Map {
 public:
  explicit Map(const std::shared_ptr<QImage>& map_image);
  ~Map() = default;

  void Update(int time);
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

  void UpdateCamera(QPainter* painter);

  std::shared_ptr<Camera> GetCurrentCamera() const;
  std::shared_ptr<QImage> GetScaledMapImage() const;
  std::vector<std::shared_ptr<GameObject>>* GetGameObjects();

 private:
  void UpdateImageScale(const QSize& image_size);

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

 private:
  std::shared_ptr<b2World> world_;
  std::shared_ptr<GameObject> player_;
  std::shared_ptr<b2ContactListener> contact_listener_;

  std::shared_ptr<Camera> camera_;

  std::vector<std::shared_ptr<GameObject>> game_objects_;
  std::vector<std::shared_ptr<GameObject>> game_objects_to_add_;
  std::shared_ptr<QImage> map_image_;
  std::shared_ptr<QImage> scaled_map_image_;

  int picked_mushrooms_ = 0;

  std::vector<bool> is_key_pressed_;
  std::vector<bool> is_key_clamped_;

  std::shared_ptr<AudioManager> audio_manager_;
};

#endif  // MODEL_MAP_H_

#include "map.h"

Map::Map(const std::shared_ptr<QImage>& map_image,
         std::shared_ptr<std::vector<std::shared_ptr<Sound>>> sound_data)
    : world_(std::make_shared<b2World>(b2Vec2(0, 20))),
      camera_(std::make_shared<Camera>(map_image->size())),
      map_image_(map_image),
      scaled_map_image_(std::make_shared<QImage>(*map_image)),
      is_key_pressed_(static_cast<int>(Key::kAnyKey) + 1, false),
      is_key_clamped_(static_cast<int>(Key::kAnyKey) + 1, false),
      sound_data_(std::move(sound_data)),
      mushroom_sound_(std::make_shared<Sound>(QUrl(
          "qrc:/audio/player/mushroom.mp3"))) {
  sound_data_->push_back(mushroom_sound_);
}

void Map::Update(int time) {
  if (player_->IsDeleted()) {
    return;
  }
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
  game_objects_.erase(
      std::remove_if(game_objects_.begin(),
                     game_objects_.end(),
                     [](const auto& object) {
                       return object == nullptr;
                     }),
      game_objects_.end());

  // Функция Step() обновляет Box2D.
  // Первый передаваемый параметр - время. Время передается в миллисекундах, а
  // для Box2D время должно измеряться в секундах.
  world_->Step(static_cast<float>(time / 1000.0),
               kVelocityAccuracy,
               kPositionAccuracy);
  is_key_pressed_.assign(is_key_pressed_.size(), false);
}

std::shared_ptr<GameObject> Map::GetPlayer() const {
  return player_;
}

int Map::GetPickedMushroomsCount() const {
  return picked_mushrooms_;
}

void Map::AddGameObject(std::shared_ptr<GameObject> object) {
  game_objects_to_add_.push_back(std::move(object));
}

void Map::SetPlayerObject(std::shared_ptr<GameObject> player) {
  player_ = std::move(player);
}

void Map::SetContactListener(std::shared_ptr<b2ContactListener> listener) {
  contact_listener_ = std::move(listener);
  world_->SetContactListener(contact_listener_.get());
}

b2Body* Map::CreateBody(b2BodyDef* body_definition) {
  return world_->CreateBody(body_definition);
}

void Map::SetPressedKeyStatus(Key key, bool is_pressed) {
  is_key_pressed_[static_cast<int>(key)] = is_pressed;
}

void Map::SetClampedKeyStatus(Key key, bool is_clamped) {
  is_key_clamped_[static_cast<int>(key)] = is_clamped;
  if (key == Key::kLeft && is_clamped) {
    is_key_clamped_[static_cast<int>(Key::kRight)] = false;
  }
  if (key == Key::kRight && is_clamped) {
    is_key_clamped_[static_cast<int>(Key::kLeft)] = false;
  }
}

bool Map::IsKeyPressed(Key key) {
  return is_key_pressed_[static_cast<int>(key)];
}

bool Map::IsKeyClamped(Key key) {
  return is_key_clamped_[static_cast<int>(key)];
}

void Map::PickUpMushroom() {
  picked_mushrooms_++;
  mushroom_sound_->Replay();
}

void Map::SetDefaultVolume(int volume) {
  for (auto sound : *sound_data_) {
    sound->SetDefaultVolume(volume);
  }
}

void Map::UpdateImageScale(const QSize& image_size) {
  if (scaled_map_image_->width() != image_size.width()
      && scaled_map_image_->height() != image_size.height()) {
    *scaled_map_image_ = map_image_->scaled(image_size, Qt::KeepAspectRatio);
  }
}

void Map::UpdateCamera(QPainter* painter) {
  camera_->UpdateCamera(painter->window(), player_->GetPositionInPixels());
  UpdateImageScale(camera_->GetCameraSize());
}

std::shared_ptr<Camera> Map::GetCurrentCamera() const {
  return camera_;
}

std::shared_ptr<QImage> Map::GetScaledMapImage() const {
  return scaled_map_image_;
}

std::vector<std::shared_ptr<GameObject>>* Map::GetGameObjects() {
  return &game_objects_;
}

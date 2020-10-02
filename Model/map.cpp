#include "map.h"

Map::Map(const QImage& map_image)
    : world_(std::make_shared<b2World>(b2Vec2(0, 20))),
      current_camera_(0, 0, kVisibleSize.x(), kVisibleSize.y()),
      map_image_(map_image),
      scaled_map_image_(map_image),
      is_key_pressed_(static_cast<int>(Key::kAnyKey) + 1, false),
      is_key_clamped_(static_cast<int>(Key::kAnyKey) + 1, false),
      audio_manager_(std::make_shared<AudioManager>()) {}

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

void Map::SetScale(double scale) {
  scale_ = scale;
}

double Map::GetScale() const {
  return scale_;
}

void Map::SetShift(QPoint shift) {
  shift_ = (shift / scale_ - kVisibleSize) / 2.0;
}

QPoint Map::GetShift() const {
  return shift_;
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
}

std::shared_ptr<AudioManager> Map::GetAudioManager() const {
  return audio_manager_;
}

void Map::SetGeneralVolume(int general_volume) {
  audio_manager_->SetGeneralVolume(general_volume);
}

void Map::SetCurrentVolume(int current_volume) {
  audio_manager_->SetCurrentVolume(current_volume);
}

void Map::UpdateImageScale(int width, int height) {
  if (scaled_map_image_.width() != width
      && scaled_map_image_.height() != height) {
    scaled_map_image_ = map_image_.scaled(width, height, Qt::KeepAspectRatio);
  }
}

void Map::UpdateCameraPosition() {
  QPoint target = player_->GetPositionInPixels() - current_camera_.center();
  if (kPlayerBoundary.x() / 2 < target.x()) {
    current_camera_.translate(target.x() - kPlayerBoundary.x() / 2, 0);
  }
  if (-kPlayerBoundary.x() / 2 > target.x()) {
    current_camera_.translate(target.x() + kPlayerBoundary.x() / 2, 0);
  }
  if (kPlayerBoundary.y() / 2 < target.y()) {
    current_camera_.translate(0, target.y() - kPlayerBoundary.y() / 2);
  }
  if (-kPlayerBoundary.y() / 2 > target.y()) {
    current_camera_.translate(0, target.y() + kPlayerBoundary.y() / 2);
  }

  if (current_camera_.left() < shift_.x()) {
    current_camera_.moveLeft(shift_.x());
  }
  if (current_camera_.right() > map_image_.width() - shift_.x()) {
    current_camera_.moveRight(map_image_.width() - shift_.x());
  }
  if (current_camera_.top() < shift_.y()) {
    current_camera_.moveTop(shift_.y());
  }
  if (current_camera_.bottom() > map_image_.height() - shift_.y()) {
    current_camera_.moveBottom(map_image_.height() - shift_.y());
  }
}

QPoint Map::GetVisibleSize() const {
  return kVisibleSize;
}

QRect Map::GetCurrentCamera() const {
  return current_camera_;
}

std::shared_ptr<QImage> Map::GetMapImage() const {
  return std::make_shared<QImage>(map_image_);
}

std::shared_ptr<QImage> Map::GetScaledMapImage() const {
  return std::make_shared<QImage>(scaled_map_image_);
}

std::shared_ptr<std::vector<std::shared_ptr<GameObject>>>
    Map::GetGameObjects() const {
  return std::make_shared<std::vector<std::shared_ptr<GameObject>>>(
      game_objects_);
}

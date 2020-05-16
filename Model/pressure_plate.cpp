#include "pressure_plate.h"

PressurePlate::PressurePlate(std::weak_ptr<Map> map,
                             b2BodyType type,
                             const QPoint& body_position,
                             const QPolygon& polygon,
                             std::shared_ptr<Animator> animator)
    : Entity(std::move(map), type, body_position, polygon, EntityType::kPlate) {
  SetAnimator(std::move(animator));
}

void PressurePlate::AddPlatform(const std::shared_ptr<Entity>& platform) {
  platforms_.push_back(platform);
  platform->Stop();
}

void PressurePlate::ActivatePlatforms() {
  for (const auto& platform : platforms_) {
    platform->Activate();
  }
}

void PressurePlate::StopPlatforms() {
  for (const auto& platform : platforms_) {
    platform->Stop();
  }
}

void PressurePlate::BeginCollision(b2Fixture*,
                                   EntityType,
                                   EntityType other_type) {
  if (other_type == EntityType::kPlayerPart
      || other_type == EntityType::kPlayer) {
    collisions_count_++;
    ActivatePlatforms();
    if (collisions_count_ == 1) {
      animator_->SetCurrentAnimation(kPressed);
    }
  }
}

void PressurePlate::EndCollision(b2Fixture*, EntityType other_type) {
  if (other_type != EntityType::kPlayer
      && other_type != EntityType::kPlayerPart) {
    return;
  }
  collisions_count_--;
  if (collisions_count_ == 0) {
    StopPlatforms();
    animator_->SetCurrentAnimation(kReleased);
  }
}

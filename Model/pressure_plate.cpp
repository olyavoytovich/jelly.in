#include "pressure_plate.h"

PressurePlate::PressurePlate(std::shared_ptr<Map> map,
                             b2BodyType type,
                             const QPoint& body_position,
                             const QPolygon& polygon,
                             std::shared_ptr<Animator> animator,
                             EntityType entity_type)
    : Entity(std::move(map), type, body_position, polygon, entity_type) {
  SetAnimator(std::move(animator));
}

void PressurePlate::AddPlatform(const std::shared_ptr<Entity>& platform) {
  platforms_.push_back(platform);
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
    animator_->Play();
    collisions_count_++;
    ActivatePlatforms();
    if (collisions_count_ == 1) {
      animator_->SetCurrentAnimation("pressed");
      animator_->Play();
    }
  }
}

void PressurePlate::EndCollision(b2Fixture*) {
  collisions_count_--;
  if (!collisions_count_) {
    StopPlatforms();
    animator_->SetCurrentAnimation("released");
    animator_->Play();
  }
}

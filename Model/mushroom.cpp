#include "mushroom.h"

Mushroom::Mushroom(std::weak_ptr<Map> map,
                   b2BodyType type,
                   const QPoint& body_position,
                   const QPolygon& polygon)
    : Entity(std::move(map),
             type,
             body_position,
             polygon,
             EntityType::kMushroom), is_picked_(false) {
  // Грибы сталкиваются только с игроком или его частью
  SetNoCollisionMask(~(static_cast<uint16_t>(EntityType::kPlayer)
      + static_cast<uint16_t>(EntityType::kPlayerPart)));
  player_get_mushroom_audio_key_ = map_.lock()->GetAudioManager()->
      CreateAudioPlayer(AudioName::kPlayerGettingMushroom);
}

void Mushroom::BeginCollision(b2Fixture*, EntityType, EntityType) {
  if (!IsDeleted()) {
    is_picked_ = true;
    map_.lock()->GetAudioManager()->
        PlayAudioPlayer(player_get_mushroom_audio_key_);
    MarkAsDeleted();
  }
}

bool Mushroom::IsPicked() const {
  return is_picked_;
}

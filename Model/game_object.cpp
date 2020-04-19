#include "game_object.h"

void GameObject::Update(int) {}

void GameObject::Draw(QPainter*) const {}

void GameObject::MarkAsDeleted() {
  is_object_deleted_ = true;
}

bool GameObject::IsDeleted() const {
  return is_object_deleted_;
}

b2Body* GameObject::GetB2Body() const {
  return nullptr;
}

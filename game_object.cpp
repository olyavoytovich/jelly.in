#include "game_object.h"

GameObject::GameObject() : rect_(300, 300, 100, 100), dx_(2) {}

void GameObject::Update() {
  if (rect_.x() + dx_ + rect_.width() > 800) {
    dx_ = -2;
  } else if (rect_.x() + dx_ < 0) {
    dx_ = 2;
  }
  rect_.translate(dx_, 0);
}

void GameObject::Draw(QPainter* painter) const {
  painter->setBrush(QBrush("#c9a0dc"));
  painter->drawEllipse(rect_);
}

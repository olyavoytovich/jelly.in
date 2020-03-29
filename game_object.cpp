#include "game_object.h"

#include <utility>

GameObject::GameObject(QPolygon polygon, int dx)
    : polygon_(std::move(polygon)), dx_(dx) {}

void GameObject::Update() {
  polygon_.translate(dx_, 0);
}

void GameObject::Draw(QPainter* painter) const {
  painter->save();
  painter->setBrush(QBrush("#c9a0dc"));
  painter->drawPolygon(polygon_);
  painter->restore();
}

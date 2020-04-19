#include "game_object.h"

GameObject::GameObject(QPolygon polygon, int dx)
    : polygon_(std::move(polygon)), dx_(dx) {}

void GameObject::Update(int) {
  polygon_.translate(dx_, 0);
}

void GameObject::Draw(QPainter* painter) const {
  painter->save();
  painter->setBrush(QBrush(QColor(201, 160, 220, 50),
                           Qt::BrushStyle::BDiagPattern));
  painter->drawPolygon(polygon_);
  painter->restore();
}

void GameObject::DeleteMe() {
  is_object_deleted_ = true;
}

bool GameObject::IsDeleted() const {
  return is_object_deleted_;
}

b2Body* GameObject::GetB2Body() const {
  return nullptr;
}

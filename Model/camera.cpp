#include "camera.h"

Camera::Camera(const QSize& map_size)
    : current_camera_(0, 0, kVisibleSize.x(), kVisibleSize.y()),
      map_size_(map_size) {}

void Camera::UpdateCamera(const QRect& window_rectangle,
                          const QPoint& player_position) {
  if (last_window_size != window_rectangle.size()) {
    last_window_size = window_rectangle.size();

    scale_ = std::max(
        static_cast<double>(window_rectangle.width()) / kVisibleSize.x(),
        static_cast<double>(window_rectangle.height()) / kVisibleSize.y());
    shift_ = QPoint(window_rectangle.width(), window_rectangle.height());
    shift_ = (shift_ / scale_ - kVisibleSize) / 2.0;
  }
  MoveCameraAfterPlayer(player_position);
}

void Camera::MovePainterToCamera(QPainter* painter) const {
  painter->translate(shift_ * scale_);
  painter->translate(-current_camera_.topLeft() * scale_);
}

QSize Camera::GetCameraSize() const {
  return map_size_ * scale_;
}

double Camera::GetScale() const {
  return scale_;
}

void Camera::MoveCameraAfterPlayer(const QPoint& player_position) {
  QPoint target = player_position - current_camera_.center();
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
  if (current_camera_.right() > map_size_.width() - shift_.x()) {
    current_camera_.moveRight(map_size_.width() - shift_.x());
  }
  if (current_camera_.top() < shift_.y()) {
    current_camera_.moveTop(shift_.y());
  }
  if (current_camera_.bottom() > map_size_.height() - shift_.y()) {
    current_camera_.moveBottom(map_size_.height() - shift_.y());
  }
}

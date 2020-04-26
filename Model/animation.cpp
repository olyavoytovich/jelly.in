#include "animation.h"

#include <utility>

Animation::Animation(std::vector<std::shared_ptr<QImage>> frames,
                     int duration)
    : frames_(std::move(frames)), duration_(duration) {}

std::shared_ptr<QImage> Animation::GetCurrentFrame(int index) const {
  return frames_[index];
}

int Animation::GetFrameDuration(bool is_repeated_in_reverse_order_) const {
  if (is_repeated_in_reverse_order_) {
    return duration_ / frames_.size() / 2;
  } else {
    return duration_ / frames_.size();
  }
}

int Animation::GetFramesCount() const {
  return frames_.size();
}


#include "animation.h"

Animation::Animation(std::vector<std::shared_ptr<QImage>> frames,
                     int duration)
    : frames_(std::move(frames)),
      duration_(duration), scaled_frames_(frames_.size()) {
  for (int i = 0; i < frames_.size(); i++) {
    scaled_frames_[i] = std::make_shared<QImage>(*frames_[i]);
  }
}

std::shared_ptr<QImage> Animation::GetCurrentFrame(
    int index, int width, int height) {
  if (scaled_frames_[index]->width() != width
      && scaled_frames_[index]->height() != height) {
    for (int i = 0; i < frames_.size(); i++) {
      scaled_frames_[i] = std::make_shared<QImage>(
          frames_[i]->scaled(width, height, Qt::IgnoreAspectRatio));
    }
  }
  return scaled_frames_[index];
}

int Animation::GetFrameDuration(bool is_repeated_back) const {
  if (is_repeated_back) {
    return duration_ / frames_.size() / 2;
  } else {
    return duration_ / frames_.size();
  }
}

int Animation::GetFramesCount() const {
  return frames_.size();
}

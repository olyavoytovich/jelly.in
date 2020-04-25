#include "animator.h"

Animator::Animator(std::shared_ptr<Animation> animation)
    : animation_(std::move(animation)) {}

std::shared_ptr<QImage> Animator::GetImage() {
  return animation_->GetCurrentFrame(current_frame_);
}

void Animator::Update(int time) {
    if (!is_playing_) {
      return;
    }
    time_since_last_frame_ += time;
    if (time_since_last_frame_ > animation_->GetFrameDuration()) {
      time_since_last_frame_ = 0;
      if (current_frame_ + direction_ == animation_->GetFramesCount()) {
        direction_ = -1;
      }
      current_frame_ += direction_;
      if (current_frame_ == -1) {
        Finish();
        return;
      }
    }
}

void Animator::Play() {
  is_playing_ = true;
}

void Animator::Finish() {
  is_playing_ = false;
  current_frame_ = 0;
  time_since_last_frame_ = 0;
  direction_ = 1;
}


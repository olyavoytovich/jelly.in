#include "animator.h"

Animator::Animator(std::shared_ptr<Animation> animation)
    : animation_(std::move(animation)) {
  frame_duration_ = animation_->GetFrameDuration(is_repeated_back_);
}

std::shared_ptr<QImage> Animator::GetCurrentImage() {

  return animation_->GetCurrentFrame(current_frame_);
}

void Animator::RepeatInReverseOrder() {
  is_repeated_back_ = true;
  frame_duration_ = animation_->GetFrameDuration(is_repeated_back_);
}

void Animator::LoopAnimation() {
  is_looped_ = true;
}

void Animator::Play() {
  is_playing_ = true;
}

void Animator::Update(int time) {
  if (!is_playing_) {
    return;
  }
  time_since_last_frame_ += time;
  if (time_since_last_frame_ > frame_duration_) {
    time_since_last_frame_ = 0;
    if (current_frame_ + direction_ == animation_->GetFramesCount()) {
      if (!is_repeated_back_) {
        if (!is_looped_) {
          Finish();
          return;
        }
        direction_ = 1;
        current_frame_ = -1;
      } else {
        direction_ = -1;
      }
    }
    current_frame_ += direction_;
    if (current_frame_ == -1) {
      if (!is_looped_) {
        Finish();
        return;
      }
      direction_ = 1;
      current_frame_ += 1;
    }
  }
}

void Animator::Finish() {
  is_playing_ = false;
  current_frame_ = 0;
  time_since_last_frame_ = 0;
  direction_ = 1;
}

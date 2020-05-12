#include "animator.h"

Animator::Animator(
    std::map<QString, std::shared_ptr<Animation>> name_to_animation,
    const QString& current_animation_name)
    : name_to_animation_(std::move(name_to_animation)) {
  current_animation_ = name_to_animation_[current_animation_name];
  frame_duration_ = current_animation_->GetFrameDuration(is_repeated_back_);
}

std::shared_ptr<QImage> Animator::GetCurrentImage(int width, int height) {
  return current_animation_->GetCurrentFrame(current_frame_, width, height);
}

void Animator::RepeatInReverseOrder() {
  is_repeated_back_ = true;
  frame_duration_ = current_animation_->GetFrameDuration(is_repeated_back_);
}

void Animator::LoopAnimation() {
  is_looped_ = true;
}

void Animator::Play() {
  is_playing_ = true;
}

void Animator::SetCurrentAnimation(const QString& current_animation_name) {
  current_animation_ = name_to_animation_[current_animation_name];
  frame_duration_ = current_animation_->GetFrameDuration(is_repeated_back_);
  Reset();
}

void Animator::Update(int time) {
  if (!is_playing_) {
    return;
  }
  time_since_last_frame_ += time;
  if (time_since_last_frame_ > frame_duration_) {
    time_since_last_frame_ = 0;
    if (current_frame_ + direction_ == current_animation_->GetFramesCount()) {
      if (!is_repeated_back_) {
        if (!is_looped_) {
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
        Reset();
        return;
      }
      direction_ = 1;
      current_frame_ += 1;
    }
  }
}

void Animator::Reset() {
  is_playing_ = false;
  current_frame_ = 0;
  time_since_last_frame_ = 0;
  direction_ = 1;
}

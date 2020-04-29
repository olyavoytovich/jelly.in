#ifndef MODEL_ANIMATOR_H_
#define MODEL_ANIMATOR_H_

#include <QImage>
#include <memory>
#include <utility>

#include "animation.h"

class Animator {
 public:
  explicit Animator(std::shared_ptr<Animation> animation_);

  ~Animator() = default;

  void RepeatInReverseOrder();
  void LoopAnimation();
  void Play();

  void Update(int time);
  std::shared_ptr<QImage> GetCurrentImage(int width, int height);

 private:
  void Finish();

 private:
  std::shared_ptr<Animation> animation_;
  bool is_repeated_back_ = false;
  int current_frame_ = 0;
  int frame_duration_;
  int direction_ = 1;
  int time_since_last_frame_ = 0;
  bool is_looped_ = false;
  bool is_playing_ = false;
};

#endif  // MODEL_ANIMATOR_H_

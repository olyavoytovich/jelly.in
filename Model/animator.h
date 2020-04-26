#ifndef MODEL_ANIMATOR_H_
#define MODEL_ANIMATOR_H_

#include <QImage>
#include <memory>
#include <utility>

#include "animation.h"

class Animator {
 public:
  explicit Animator(std::shared_ptr<Animation> animation_);

  void RepeatInReverseOrder();

  void Update(int time);
  std::shared_ptr<QImage> GetCurrentImage();
  void Play();

 private:
  std::shared_ptr<Animation> animation_;
  bool is_repeated_in_reverse_order_ = false;
  int current_frame_ = 0;
  int direction_ = 1;
  int time_since_last_frame_ = 0;
  bool is_playing_ = false;

 private:
  void Finish();
};

#endif //  MODEL_ANIMATOR_H_

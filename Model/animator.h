#ifndef ANIMATOR_H_
#define ANIMATOR_H_

#include <QImage>
#include <memory>

#include "animation.h"


class Animator {
 public:
  explicit Animator(std::shared_ptr<Animation> animation_);

  void Update(int time);
  std::shared_ptr<QImage> GetImage();
  void Play();

 private:
  std::shared_ptr<Animation> animation_;
 int current_frame_ = 0;
 int direction_ = 1;
 int time_since_last_frame_ = 0;
 bool is_playing_ = false;

 private:
  void Finish();
};

#endif //ANIMATOR_H_

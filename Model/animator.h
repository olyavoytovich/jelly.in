#ifndef MODEL_ANIMATOR_H_
#define MODEL_ANIMATOR_H_

#include <QImage>
#include <map>
#include <memory>
#include <utility>

#include "animation.h"

class Animator {
 public:
  Animator(std::map<QString, std::shared_ptr<Animation>> name_to_animation,
           const QString& current_animation);

  ~Animator() = default;

  void RepeatInReverseOrder();
  void LoopAnimation();
  void Play();

  void SetCurrentAnimation(const QString& current_animation_name,
                           bool stop_current_animation = true);

  void Update(int time);
  std::shared_ptr<QImage> GetCurrentImage(int width, int height);

 private:
  void Reset();
  void StartNextAnimation();

 private:
  std::map<QString, std::shared_ptr<Animation>> name_to_animation_;
  std::shared_ptr<Animation> current_animation_;

  bool is_repeated_back_ = false;
  bool is_looped_ = false;
  bool is_playing_ = false;

  int current_frame_ = 0;
  int frame_duration_;
  int direction_ = 1;
  int time_since_last_frame_ = 0;

  QString next_animation_;
};

#endif  // MODEL_ANIMATOR_H_

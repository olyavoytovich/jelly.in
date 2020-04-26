#ifndef MODEL_ANIMATION_H_
#define MODEL_ANIMATION_H_

#include <QImage>
#include <memory>
#include <utility>
#include <vector>

class Animation {
 public:
  Animation(std::vector<std::shared_ptr<QImage>> frames, int duration);

  std::shared_ptr<QImage> GetCurrentFrame(int index) const;
  int GetFrameDuration(bool is_repeated_back) const;
  int GetFramesCount() const;

 private:
  std::vector<std::shared_ptr<QImage>> frames_;
  int duration_;
};

#endif  // MODEL_ANIMATION_H_

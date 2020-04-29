#ifndef MODEL_ANIMATION_H_
#define MODEL_ANIMATION_H_

#include <QImage>
#include <memory>
#include <utility>
#include <vector>

#include "map.h"

class Animation {
 public:
  Animation(std::vector<std::shared_ptr<QImage>> frames,
            int duration);

  ~Animation() = default;

  std::shared_ptr<QImage> GetCurrentFrame(int index, int width, int height);
  int GetFrameDuration(bool is_repeated_back) const;
  int GetFramesCount() const;

 private:
  std::vector<std::shared_ptr<QImage>> frames_;
  std::vector<std::shared_ptr<QImage>> scaled_frames_;
  int duration_;
};

#endif  // MODEL_ANIMATION_H_

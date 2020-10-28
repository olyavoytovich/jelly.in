#ifndef MODEL_SOUND_H_
#define MODEL_SOUND_H_

#include <QMediaContent>
#include <QMediaPlayer>
#include <memory>

class Sound {
 public:
  explicit Sound(QUrl media);

  void Play();
  void Replay();
  void SetVolume(int volume);
  void SetDefaultVolume(int volume);

 private:
  int volume_;
  int default_volume_;
  std::shared_ptr<QMediaPlayer> audio_;
};

#endif  // MODEL_SOUND_H_

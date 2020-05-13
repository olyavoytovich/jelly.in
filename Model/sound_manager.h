#ifndef MODEL_SOUND_MANAGER_H_
#define MODEL_SOUND_MANAGER_H_

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSoundEffect>
#include <algorithm>
#include <memory>
#include <vector>

class SoundManager {
 public:
  SoundManager();

  void AddSon(SoundManager* son);

  void AddMedia(const QMediaContent& content);

  void Play();
  void Replay();
  void Pause();
  void Stop();
  void StopAll();

  void ChangeVolume(int volume);
  void SetVolume(int volume);
  void SetVolumeByDistance(int distance);

  void SetPlayBackMode(QMediaPlaylist::PlaybackMode play_back_mode);

  ~SoundManager() = default;

 private:
  int pure_volume_;
  int parent_volume_;
  std::vector<std::shared_ptr<QMediaContent>> test;
  std::shared_ptr<QMediaPlaylist> playlist_;
  std::shared_ptr<QMediaPlayer> player_;
  std::vector<SoundManager*> sons_;
};

#endif  // MODEL_SOUND_MANAGER_H_

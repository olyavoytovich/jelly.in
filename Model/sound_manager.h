#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSoundEffect>
#include <memory>
#include <vector>

class SoundManager {
 public:
  SoundManager();

  void AddSon(std::shared_ptr<SoundManager> son);

  void AddMedia(QString path);

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
  std::shared_ptr<QMediaPlaylist> playlist_;
  std::shared_ptr<QMediaPlayer> player_;
  std::vector<std::shared_ptr<SoundManager>> sons_;
};

#endif  // SOUND_MANAGER_H_

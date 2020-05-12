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

  void AddMedia(QString path);

  void Play();
  void Replay();
  void Pause();
  void Stop();

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
};

#endif  // SOUND_MANAGER_H_

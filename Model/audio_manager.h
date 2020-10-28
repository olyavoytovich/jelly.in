#ifndef MODEL_AUDIO_MANAGER_H_
#define MODEL_AUDIO_MANAGER_H_

#include <QMediaContent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <memory>
#include <vector>

enum class AudioName {
  kButtonClick,
  kBackground,
  kMenuAudio,
  kAnyAudio
};

class AudioManager {
 public:
  AudioManager();
  ~AudioManager() = default;

  void Play(AudioName name);
  void Replay(AudioName name);
  void Pause(AudioName name);
  void Stop(AudioName name);
  void SetPlayBackMode(AudioName name, QMediaPlaylist::PlaybackMode mode);

  void SetVolume(AudioName name, int volume);

 private:
  std::vector<std::shared_ptr<QMediaPlayer>> audios_;
};

#endif  // MODEL_AUDIO_MANAGER_H_

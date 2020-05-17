#ifndef MODEL_AUDIO_MANAGER_H_
#define MODEL_AUDIO_MANAGER_H_

#include <QMediaContent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <algorithm>
#include <memory>
#include <random>
#include <vector>
#include <unordered_map>

enum class AudioName {
  kButtonClick,
  kThorn,
  kDrop,
  kPlayerJump,
  kPlayerLanding,
  kPlayerSeparation,
  kPlayerTakingDamage,
  kBackground,
  kMenuAudio,
  kAnyAudio
};

class AudioManager {
 public:
  AudioManager();
  ~AudioManager() = default;

  void SetVolume(int key, int volume);

  int CreateAudioPlayer(AudioName audio_name);
  int CreateAudioPlayerByPlayList(AudioName audio_name);
  void PlayAudioPlayer(int key);
  void ReplayAudioPlayer(int key);
  void PauseAudioPlayer(int key);
  void StopAudioPlayer(int key);
  void PlayAudio(AudioName audio_name, int volume = 100);

  void SetPlayBackMode(int key, QMediaPlaylist::PlaybackMode mode);

  void ReVolume();
  void SetCurrentVolume(int volume);
  void SetGeneralVolume(int volume);

 private:
  int general_volume_ = 100;
  int current_volume_ = 100;

  std::vector<std::shared_ptr<QMediaContent>> audio_files_;
  std::unordered_map<int, std::shared_ptr<QMediaPlayer>> audio_players_;

  std::mt19937 random_generator_;
  std::uniform_int_distribution<> range_;
};

#endif  // MODEL_AUDIO_MANAGER_H_

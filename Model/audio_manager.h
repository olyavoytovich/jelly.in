#ifndef MODEL_SOUND_MANAGER_H_
#define MODEL_SOUND_MANAGER_H_

#include <QMediaContent>
#include <QMediaPlayer>
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

enum class AudioName {
  kButtonClick,  // menu
  kThorn, // enemy
  kDrop,
  kPlayerJump, // player
  kPlayerLanding,
  kPlayerSeparation,
  kPlayerTakingDamage,
  kBackground,
  kAnyAudio
};

class AudioManager {
 public:
  AudioManager();
  ~AudioManager() = default;

  void LoadAudio(AudioName audio_name, const QString& path);

  void SetVolume(int key, int volume);

  int CreateAudioPlayer(AudioName audio_name);
  void PlayAudioPlayer(int key);
  void ReplayAudioPlayer(int key);
  void PauseAudioPlayer(int key);
  void StopAudioPlayer(int key);
  void PlayAudio(AudioName audio_name, int volume = 100);

 private:
  std::vector<std::shared_ptr<QMediaContent>> audio_files_;
  std::unordered_map<int, std::shared_ptr<QMediaPlayer>> audio_players_;

  std::mt19937 random_generator_ = std::mt19937(
              std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<> range_;
};

#endif  // MODEL_SOUND_MANAGER_H_

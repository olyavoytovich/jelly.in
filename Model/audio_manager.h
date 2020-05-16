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
  kBurdockBullet, // enemy
  kCloudBullet,
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

  void LoadAudio(AudioName audio_name, QString path);

  void SetVolume(int key, int volume);

  int CreatePlayer(AudioName audio_name);
    void PlayAudio(AudioName audio_name, int volume = 100);
    void ReplayPlayer(int key);
  void PlayPlayer(int key);
  void PausePlayer(int key);
  void StopPlayer(int key);
  void DeletePlayer(int key);
  void PlayAllPlayers();
  void PauseAllPlayers();
  void RestartAllPlayers();
  void StopAllPlayers();

 private:
  int parent_volume = 100;
  std::vector<std::shared_ptr<QMediaContent>> audio_files_;
  std::unordered_map<int, std::shared_ptr<QMediaPlayer>> audio_players_;

  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> dis;
};

#endif  // MODEL_SOUND_MANAGER_H_

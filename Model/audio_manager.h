#ifndef MODEL_SOUND_MANAGER_H_
#define MODEL_SOUND_MANAGER_H_

#include <QMediaContent>
#include <QMediaPlayer>
#include <algorithm>
#include <memory>
#include <vector>

enum class AudioName {
  kButtonClick,  // menu
  kBurdockBullet, // enemy
  kPlayerJump, // player
  kPlayerLanding,
  kPlayerSeparation,
  kPlayerTakingDamage,
  kBackground,
  kAnyAudio
};

class AudioManager {
 public:
  void LoadAudio(AudioName audio_name, QString path);

  void PlayAudio(AudioName audio_name);

  // int CreatePlayer();
  // void PlayPlayer();
  // void SetVolume();
  // void SetVolumeByDisatance();

 private:
  // int parent_volume = 100;
  std::map<AudioName, std::shared_ptr<QMediaContent>> audio_files_;
  std::map<int, std::shared_ptr<QMediaPlayer>> audio_players_;
};

#endif  // MODEL_SOUND_MANAGER_H_

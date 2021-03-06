#include "audio_manager.h"

AudioManager::AudioManager()
    : audio_files_(static_cast<int>(AudioName::kAnyAudio)),
      range_(1, 1000000),
      random_generator_(std::mt19937(
          std::chrono::system_clock::now().time_since_epoch().count())) {
  audio_files_[static_cast<int>(AudioName::kButtonClick)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/menu/button_click.mp3"));
  audio_files_[static_cast<int>(AudioName::kThorn)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/enemy/burdock_bullet.mp3"));
  audio_files_[static_cast<int>(AudioName::kDrop)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/enemy/cloud_bullet.mp3"));
  audio_files_[static_cast<int>(AudioName::kChestnut)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/enemy/chestnut.mp3"));
  audio_files_[static_cast<int>(AudioName::kPlayerJump)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/player/jump.mp3"));
  audio_files_[static_cast<int>(AudioName::kPlayerSeparation)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/player/separation.mp3"));
  audio_files_[static_cast<int>(AudioName::kPlayerTakingDamage)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/player/taking_damage.mp3"));
  audio_files_[static_cast<int>(AudioName::kPlayerGettingMushroom)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/player/mushroom.mp3"));
  audio_files_[static_cast<int>(AudioName::kBackground)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/music/background.mp3"));
  audio_files_[static_cast<int>(AudioName::kMenuAudio)] =
      std::make_shared<QMediaContent>(
          QUrl("qrc:/audio/music/menu.mp3"));
}

int AudioManager::CreateAudioPlayer(AudioName audio_name) {
  int key = range_(random_generator_);
  audio_players_[key] = std::make_shared<QMediaPlayer>();
  audio_players_[key]->setAudioRole(QAudio::GameRole);
  audio_players_[key]->setMedia(*audio_files_[static_cast<int>(audio_name)]);
  audio_players_[key]->setVolume(general_volume_ * current_volume_ / 100);

  audio_players_[key]->play();
  audio_players_[key]->stop();
  return key;
}

int AudioManager::CreateAudioPlayerByPlayList(AudioName audio_name) {
  int key = range_(random_generator_);
  audio_players_[key] = std::make_shared<QMediaPlayer>();
  QMediaPlaylist* playlist = new QMediaPlaylist;
  playlist->addMedia(*audio_files_[static_cast<int>(audio_name)]);
  audio_players_[key]->setAudioRole(QAudio::GameRole);
  audio_players_[key]->setPlaylist(playlist);
  audio_players_[key]->setVolume(general_volume_ * current_volume_ / 100);

  audio_players_[key]->play();
  audio_players_[key]->stop();
  return key;
}

void AudioManager::PlayAudioPlayer(int key) {
  audio_players_[key]->play();
}

void AudioManager::ReplayAudioPlayer(int key) {
  StopAudioPlayer(key);
  PlayAudioPlayer(key);
}

void AudioManager::PauseAudioPlayer(int key) {
  audio_players_[key]->pause();
}

void AudioManager::StopAudioPlayer(int key) {
  audio_players_[key]->stop();
}

void AudioManager::PlayAudio(AudioName audio_name, int volume) {
  QMediaPlayer* audio_player = new QMediaPlayer;
  audio_player->setMedia(*audio_files_[static_cast<int>(audio_name)]);
  audio_player->setVolume(
      volume * general_volume_ * current_volume_ / 10000);
  audio_player->play();
  QObject::connect(audio_player,
                   &QMediaPlayer::mediaStatusChanged,
                   [audio_player](QMediaPlayer::MediaStatus status) {
                     if (status == QMediaPlayer::EndOfMedia) {
                       audio_player->setMedia(nullptr);
                       audio_player->deleteLater();
                     }
                   });
}

void AudioManager::SetVolume(int key, int volume) {
  audio_players_[key]->setVolume(
      volume * general_volume_ * current_volume_ / 10000);
}

void AudioManager::SetPlayBackMode(int key, QMediaPlaylist::PlaybackMode mode) {
  audio_players_[key]->playlist()->setPlaybackMode(mode);
}

void AudioManager::ReVolume() {
  for (auto& audio_player : audio_players_) {
    audio_player.second->setVolume(general_volume_ * current_volume_ / 100);
  }
}

void AudioManager::SetGeneralVolume(int volume) {
  general_volume_ = volume;
  ReVolume();
}

void AudioManager::SetCurrentVolume(int volume) {
  current_volume_ = volume;
  ReVolume();
}

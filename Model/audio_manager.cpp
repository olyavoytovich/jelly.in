#include "audio_manager.h"

AudioManager::AudioManager()
    : audio_files_(static_cast<int>(AudioName::kAnyAudio)),
      range_(1, 1000000) {
}

void AudioManager::LoadAudio(AudioName audio_name, const QString& path) {
  audio_files_[static_cast<int>(audio_name)] =
      std::make_shared<QMediaContent>(QUrl(path));
}

int AudioManager::CreateAudioPlayer(AudioName audio_name) {
    int key = range_(random_generator_);
    audio_players_[key] = std::make_shared<QMediaPlayer>();
    audio_players_[key]->setMedia(*audio_files_[static_cast<int>(audio_name)]);
    audio_players_[key]->setVolume(100);
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
  audio_player->setVolume(volume);
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
    audio_players_[key]->setVolume(volume);
}

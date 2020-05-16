#include "audio_manager.h"
#include <QMediaPlaylist>

AudioManager::AudioManager()
    : audio_files_(static_cast<int>(AudioName::kAnyAudio)),
      gen(rd()), dis(1, 1000000) {
}

void AudioManager::LoadAudio(AudioName audio_name, QString path) {
  audio_files_[static_cast<int>(audio_name)] =
      std::make_shared<QMediaContent>(QUrl(path));
}

int AudioManager::CreatePlayer(AudioName audio_name) {
    int key = dis(gen);
    audio_players_[key] = std::make_shared<QMediaPlayer>();
    audio_players_[key]->setMedia(*audio_files_[static_cast<int>(audio_name)]);
    audio_players_[key]->setVolume(100);
    audio_players_[key]->play();
    audio_players_[key]->stop();
    return key;
}

void AudioManager::PlayPlayer(int key) {
    audio_players_[key]->play();
}

void AudioManager::ReplayPlayer(int key) {
    audio_players_[key]->stop();
    audio_players_[key]->play();
}

void AudioManager::PausePlayer(int key) {
    audio_players_[key]->pause();
}

void AudioManager::StopPlayer(int key) {
    audio_players_[key]->stop();
}

void AudioManager::DeletePlayer(int key) {
    audio_players_[key]->deleteLater();
    audio_players_.erase(key);
}

void AudioManager::PlayAllPlayers() {
    for (const auto& player : audio_players_) {
        player.second->play();
    }
}

void AudioManager::RestartAllPlayers() {
    StopAllPlayers();
    PlayAllPlayers();
}

void AudioManager::PauseAllPlayers() {
    for (const auto& player : audio_players_) {
        player.second->pause();
    }
}

void AudioManager::StopAllPlayers() {
    for (const auto& player : audio_players_) {
        player.second->stop();
    }
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

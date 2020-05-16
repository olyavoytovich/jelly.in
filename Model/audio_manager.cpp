#include "audio_manager.h"
#include <QMediaPlaylist>

void AudioManager::LoadAudio(AudioName audio_name, QString path) {
  audio_files_[audio_name] =
      std::make_shared<QMediaContent>(QUrl(path));
}

void AudioManager::PlayAudio(AudioName audio_name) {
  QMediaPlayer* audio_player = new QMediaPlayer;
  audio_player->setMedia(*audio_files_[audio_name]);
  audio_player->setVolume(100);
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

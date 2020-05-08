#include "sound_manager.h"

SoundManager::SoundManager() : pure_volume_(100), parent_volume_(100),
    playlist_(std::make_shared<QMediaPlaylist>()),
    player_(std::make_shared<QMediaPlayer>()) {
  player_->setPlaylist(playlist_.get());
  player_->setVolume(pure_volume_);
}

void SoundManager::AddMedia(QString path) {
  playlist_->addMedia(QUrl(path));
}

void SoundManager::Play() {
  player_->play();
}

void SoundManager::Replay() {
  player_->stop();
  player_->play();
}

void SoundManager::Pause() {
  player_->pause();
}

void SoundManager::Stop() {
  player_->stop();
}

void SoundManager::ChangeVolume(int volume) {
  parent_volume_ = volume;
  player_->setVolume(parent_volume_ * pure_volume_ / 100);
}

void SoundManager::SetVolume(int volume) {
  pure_volume_ = volume;
  player_->setVolume(parent_volume_ * pure_volume_ / 100);
}

void SoundManager::SetPlayBackMode(
        QMediaPlaylist::PlaybackMode play_back_mode) {
  playlist_->setPlaybackMode(play_back_mode);
}

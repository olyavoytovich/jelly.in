#include "sound.h"

Sound::Sound(QUrl media) {
  audio_ = std::make_shared<QMediaPlayer>();
  audio_->setAudioRole(QAudio::GameRole);
  audio_->setMedia(media);
  default_volume_ = 100;
  volume_ = 100;
  audio_->setVolume(100);
}

void Sound::Play() {
  audio_->play();
}

void Sound::Replay() {
  audio_->setPosition(0);
  audio_->play();
}

void Sound::SetVolume(int volume) {
  volume_ = volume;
  audio_->setVolume(default_volume_ * volume_ / 100);
}

void Sound::SetDefaultVolume(int volume) {
  default_volume_ = volume;
  audio_->setVolume(default_volume_ * volume_ / 100);
}

